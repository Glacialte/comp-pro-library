from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import List, Set, Optional

INCLUDE_RE = re.compile(r'^\s*#\s*include\s*([<"])([^>"]+)[>"]\s*$')


def find_project_root(script_path: Path) -> Path:
    root = script_path.resolve().parent.parent
    if not (root / "algorithm").is_dir() or not (root / "data-structure").is_dir():
        cur = script_path.resolve().parent
        for _ in range(6):
            cand = cur.parent
            if (cand / "algorithm").is_dir() and (cand / "data-structure").is_dir():
                return cand
            cur = cand
        raise RuntimeError(
            "Project root not found (expected algorithm/ and data-structure/ near scripts/)."
        )
    return root


def display_path(path: Path, root: Path, *, fallback: str = "name") -> str:
    """
    表示用のパスを返す。絶対パスを出さない。
    - root配下なら root からの相対パス
    - それ以外は fallback に従い:
        - "name": ファイル名のみ
        - "relative": CWD からの相対 (失敗したら name)
    """
    p = path.resolve()
    r = root.resolve()
    try:
        return p.relative_to(r).as_posix()
    except ValueError:
        if fallback == "relative":
            try:
                return p.relative_to(Path.cwd().resolve()).as_posix()
            except ValueError:
                return p.name
        return p.name


def resolve_user_include(
    header: str, current_file: Path, include_dirs: List[Path]
) -> Optional[Path]:
    cand = (current_file.parent / header).resolve()
    if cand.exists() and cand.is_file():
        return cand

    for d in include_dirs:
        cand = (d / header).resolve()
        if cand.exists() and cand.is_file():
            return cand

    return None


def expand_file(
    path: Path,
    include_dirs: List[Path],
    seen: Set[Path],
    *,
    root: Path,
    emit_markers: bool = True,
) -> List[str]:
    path = path.resolve()

    if path in seen:
        # 絶対パスを出さない
        return [f"// [skipped duplicated include] {display_path(path, root)}\n"]

    seen.add(path)

    try:
        lines = path.read_text(encoding="utf-8").splitlines(keepends=True)
    except UnicodeDecodeError:
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines(
            keepends=True
        )

    out: List[str] = []
    if emit_markers:
        out.append(f"// ===== BEGIN {display_path(path, root)} =====\n")

    for line in lines:
        m = INCLUDE_RE.match(line)
        if not m:
            out.append(line)
            continue

        bracket, header = m.group(1), m.group(2)

        if bracket == "<":
            out.append(line)
            continue

        resolved = resolve_user_include(header, path, include_dirs)
        if resolved is None:
            # エラーメッセージから絶対パスを排除
            searched = [path.parent] + include_dirs
            searched_disp = ", ".join(display_path(d, root) for d in searched)
            raise FileNotFoundError(
                f'Cannot resolve include "{header}"\n'
                f"  from: {display_path(path, root)}\n"
                f"  searched: {searched_disp}\n"
            )

        out.extend(
            expand_file(
                resolved, include_dirs, seen, root=root, emit_markers=emit_markers
            )
        )

    if emit_markers:
        out.append(f"// ===== END {display_path(path, root)} =====\n")

    return out


def main(argv: List[str]) -> int:
    ap = argparse.ArgumentParser(
        description='Expand only #include "..." into a single C++ file for AtCoder.'
    )
    ap.add_argument("input", type=Path, help="Entry file (e.g., main.cpp)")
    ap.add_argument(
        "-o", "--output", type=Path, default=Path("expanded.cpp"), help="Output file"
    )
    ap.add_argument(
        "--no-markers", action="store_true", help="Do not emit BEGIN/END markers"
    )
    args = ap.parse_args(argv)

    script_path = Path(__file__)
    root = find_project_root(script_path)

    note = lambda p: display_path(p, root)  # 表示用（絶対パス禁止）

    include_dirs = [
        root,
        root / "algorithm",
        root / "data-structure",
    ]

    inp = args.input
    if not inp.is_absolute():
        cand = (root / inp).resolve()
        inp = cand if cand.exists() else (Path.cwd() / args.input).resolve()

    if not inp.exists():
        print(f"error: input not found: {note(inp)}", file=sys.stderr)
        return 2

    seen: Set[Path] = set()
    expanded_lines = expand_file(
        inp, include_dirs, seen, root=root, emit_markers=not args.no_markers
    )
    args.output.write_text("".join(expanded_lines), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
