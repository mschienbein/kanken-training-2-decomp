"""Run long jobs detached from the agent or terminal that started them, with an exit-code marker.

    python tools/jobs.py run --name c001-prepare -- python tools/accept.py prepare ...
    python tools/jobs.py status [--name c001-prepare]

A job runs in its own session (POSIX) or detached process group (Windows), so a dropped agent
turn, closed terminal or network outage does not kill it. Everything lives in scratch/jobs/<name>/:
`command.json`, `log.txt` (stdout+stderr), `pid`, and `exit` (written last, holding the exit code).
A job is finished exactly when `exit` exists. Names are unique: an existing job directory is refused.
"""
from __future__ import annotations

import argparse
import json
import os
import subprocess
import sys
import time
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
JOBS = ROOT / "scratch/jobs"


def now() -> str:
    return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


def start(name: str, command: list[str]) -> Path:
    job = JOBS / name
    job.mkdir(parents=True, exist_ok=False)
    (job / "command.json").write_text(
        json.dumps({"command": command, "cwd": str(ROOT), "started_utc": now()}, indent=1) + "\n",
        encoding="utf-8", newline="\n",
    )
    supervisor = [sys.executable, str(Path(__file__).resolve()), "_supervise", "--name", name]
    kwargs: dict = {"cwd": ROOT, "stdin": subprocess.DEVNULL, "stdout": subprocess.DEVNULL, "stderr": subprocess.DEVNULL}
    if os.name == "nt":
        kwargs["creationflags"] = subprocess.DETACHED_PROCESS | subprocess.CREATE_NEW_PROCESS_GROUP
    else:
        kwargs["start_new_session"] = True
    subprocess.Popen(supervisor, **kwargs)
    return job


def supervise(name: str) -> int:
    job = JOBS / name
    command = json.loads((job / "command.json").read_text(encoding="utf-8"))["command"]
    started = time.time()
    with (job / "log.txt").open("w", encoding="utf-8", newline="\n") as log:
        proc = subprocess.Popen(command, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL)
        (job / "pid").write_text(f"{proc.pid}\n", encoding="utf-8")
        code = proc.wait()
    marker = {"exit_code": code, "finished_utc": now(), "seconds": round(time.time() - started, 1)}
    tmp = job / "exit.tmp"
    tmp.write_text(json.dumps(marker) + "\n", encoding="utf-8", newline="\n")
    os.replace(tmp, job / "exit")
    return code


def status(name: str | None) -> list[dict]:
    rows = []
    for job in sorted(JOBS.iterdir()) if JOBS.is_dir() else []:
        if name and job.name != name:
            continue
        exit_file = job / "exit"
        info = json.loads(exit_file.read_text(encoding="utf-8")) if exit_file.is_file() else None
        rows.append({"name": job.name, "state": "finished" if info else "running", **(info or {})})
    return rows


def main(argv: list[str] | None = None) -> int:
    argv = sys.argv[1:] if argv is None else argv
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run")
    r.add_argument("--name", required=True)
    r.add_argument("command", nargs=argparse.REMAINDER)
    s = sub.add_parser("status")
    s.add_argument("--name")
    sv = sub.add_parser("_supervise")
    sv.add_argument("--name", required=True)
    args = parser.parse_args(argv)
    if args.cmd == "run":
        command = args.command[1:] if args.command[:1] == ["--"] else args.command
        if not command:
            parser.error("no command given")
        try:
            job = start(args.name, command)
        except FileExistsError:
            print(f"error: job {args.name} exists", file=sys.stderr)
            return 1
        print(f"started {args.name}: log {job / 'log.txt'}, finished when {job / 'exit'} exists")
        return 0
    if args.cmd == "_supervise":
        return supervise(args.name)
    for row in status(args.name):
        print(json.dumps(row))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
