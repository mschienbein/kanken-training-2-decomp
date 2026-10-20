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


def load(root: Path) -> dict:
    manifest = json.loads((root / MANIFEST).read_bytes())
    if manifest["revision"] != "catalogue-v13" or not manifest["inputs"]:
        raise ValueError("v13 requires a complete immutable generator input manifest")
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
    manifest = json.loads(manifest_path.read_bytes())
    descriptor = manifest["inputs"].get(label)
    if descriptor is None:
        if label in manifest.get("absent_paths", []):
            return root / PREFIX / "absent" / label
        if label.startswith(("config/", "accept/", "src/")):
            raise ValueError(f"v13 has no immutable copy for frozen input: {label}")
        return root / label
    path = descriptor["path"]
    if not path.startswith(PREFIX) or ".." in Path(path).parts:
        raise ValueError("v13 frozen input points at a live mutable path")
    content = (root / path).read_bytes()
    if len(content) != descriptor["size"] or hashlib.sha256(content).hexdigest() != descriptor["sha256"]:
        raise ValueError(f"v13 frozen input differs: {label}")
    return root / path
