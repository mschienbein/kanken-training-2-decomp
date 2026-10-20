"""V13 immutable copies of historically live generator inputs.

Original path labels are provenance, not filesystem dependencies. Keeping those
labels and exact contents preserves the historical proposal commitments.
"""
from __future__ import annotations

import hashlib
import json
from pathlib import Path

MANIFEST = "research/catalogue_v13_frozen.json"
PREFIX = "research/catalogue_v13_frozen/"


def _manifest(root: Path) -> dict:
    manifest = json.loads((root / MANIFEST).read_bytes())
    if manifest["revision"] != "catalogue-v13" or not isinstance(manifest["inputs"], dict):
        raise ValueError("v13 requires a complete immutable generator input manifest")
    absent = manifest.get("absent_paths", [])
    if (not isinstance(absent, list) or any(not isinstance(label, str) for label in absent)
            or len(set(absent)) != len(absent)):
        raise ValueError("v13 declared absent paths must be a unique list")
    for label in absent:
        path = Path(label)
        if (not label or path.is_absolute() or ".." in path.parts or not path.parts
                or path.as_posix() != label or label in manifest["inputs"]):
            raise ValueError("v13 declared absent path is not a confined exclusive relative label")
        # No sentinel is ever returned. Check the historical sentinel as well
        # so a materialized file or dangling symlink cannot masquerade as absence.
        for candidate in (root / path, root / PREFIX / "absent" / path):
            if candidate.exists() or candidate.is_symlink():
                raise ValueError(f"v13 declared absent path is materialized: {label}")
    return manifest


def load(root: Path) -> dict:
    manifest = _manifest(root)
    seen = set()
    for label, descriptor in manifest["inputs"].items():
        path = descriptor["path"]
        if (not path.startswith(PREFIX) or ".." in Path(path).parts or Path(path).is_absolute()
                or path in seen or not (root / path).resolve().is_relative_to((root / PREFIX).resolve())):
            raise ValueError("v13 frozen input points at a live mutable or escaping path")
        seen.add(path)
        content = (root / path).read_bytes()
        if len(content) != descriptor["size"] or hashlib.sha256(content).hexdigest() != descriptor["sha256"]:
            raise ValueError(f"v13 frozen input differs: {label}")
    return manifest


def resolve(root: Path, label: str) -> Path:
    """Historical readers retain their labels; only v13 installs this manifest."""
    manifest_path = root / MANIFEST
    if not manifest_path.is_file():
        return root / label
    manifest = _manifest(root)
    descriptor = manifest["inputs"].get(label)
    if label in manifest.get("absent_paths", []):
        raise ValueError(f"v13 declared absent input cannot be consumed: {label}")
    if descriptor is None:
        raise ValueError(f"v13 has no immutable copy for frozen input: {label}")
    path = descriptor["path"]
    if not path.startswith(PREFIX) or ".." in Path(path).parts:
        raise ValueError("v13 frozen input points at a live mutable path")
    content = (root / path).read_bytes()
    if len(content) != descriptor["size"] or hashlib.sha256(content).hexdigest() != descriptor["sha256"]:
        raise ValueError(f"v13 frozen input differs: {label}")
    return root / path


def is_present(root: Path, label: str) -> bool:
    """Probe optional inputs without manufacturing a consumable absence path."""
    if not (root / MANIFEST).is_file():
        return (root / label).is_file()
    manifest = _manifest(root)
    if label in manifest.get("absent_paths", []):
        return False
    return resolve(root, label).is_file()
