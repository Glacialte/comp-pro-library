#!/usr/bin/env python3
# scripts/expand.py
#
# 方針:
# - #include <...> は「そのまま残す」（標準ライブラリは展開しない）
# - #include "..." だけを再帰的に展開する（自作ヘッダを単一ファイル化）
# - ルート直下の algorithm/ と data-structure/ を探索対象にする
# - include guard / pragma once の代わりに「同一ファイルは1回だけ展開」(seen) を採用

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import List, Set, Optional

INCLUDE_RE = re.compile(r'^\s*#\s*include\s*([<"])([^>"]+)[>"]\s*$')


def find_project_root(script_path: Path) -> Path:
    """
    scripts/expand.py からプロジェクトルートを推定する。
    期待する構造:
      <root>/scripts/expand.py
      <root>/algorithm/...
      <root>/data-structure/...
    """
    root = script_path.resolve().parent.parent
    if not (root / "algorithm").is_dir() or not (root / "data-structure").is_dir():
        # 多少ゆるく：scripts がどこかにあっても root を遡って探す
        cur = script_path.resolve().parent
        for _ in range(6):  # 深追いしすぎない
            cand = cur.parent
            if (cand / "algorithm").is_dir() and (cand / "data-structure").is_dir():
                return cand
            cur = cand
        raise RuntimeError("Project root not found (expected algorithm/ and data-structure/ near scripts/).")
    return root


def resolve_user_include(header: str, current_file: Path, include_dirs: List[Path]) -> Optional[Path]:
    """
    "hoge.hpp" を次の順で探索して実パスへ解決:
    1) current_file のディレクトリ
    2) include_dirs の順
    """
    # current dir first
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
    emit_markers: bool = True,
) -> List[str]:
    """
    指定ファイルを読み取り、#include "..." を再帰展開したテキストを返す。
    """
    path = path.resolve()

    # 同一ファイルの二重展開を回避（#pragma once / include guard 的な扱い）
    if path in seen:
        return [f"// [skipped duplicated include] {path.as_posix()}\n"]

    seen.add(path)

    try:
        lines = path.read_text(encoding="utf-8").splitlines(keepends=True)
    except UnicodeDecodeError:
        # 競プロ向けヘッダでUTF-8以外はほぼ無い想定だが、一応フォールバック
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines(keepends=True)

    out: List[str] = []
    if emit_markers:
        out.append(f"// ===== BEGIN {path.as_posix()} =====\n")

    for line in lines:
        m = INCLUDE_RE.match(line)
        if not m:
            out.append(line)
            continue

        bracket, header = m.group(1), m.group(2)

        # <...> は展開しない（そのまま残す）
        if bracket == "<":
            out.append(line)
            continue

        # "..." は展開対象
        resolved = resolve_user_include(header, path, include_dirs)
        if resolved is None:
            raise FileNotFoundError(
                f'Cannot resolve include "{header}"\n'
                f"  from: {path.as_posix()}\n"
                f"  searched: {path.parent.as_posix()} and include_dirs={ [d.as_posix() for d in include_dirs] }\n"
            )

        out.extend(expand_file(resolved, include_dirs, seen, emit_markers=emit_markers))

    if emit_markers:
        out.append(f"// ===== END {path.as_posix()} =====\n")

    return out


def main(argv: List[str]) -> int:
    ap = argparse.ArgumentParser(description='Expand only #include "..." into a single C++ file for AtCoder.')
    ap.add_argument("input", type=Path, help="Entry file (e.g., main.cpp)")
    ap.add_argument("-o", "--output", type=Path, default=Path("expanded.cpp"), help="Output file")
    ap.add_argument(
        "--no-markers",
        action="store_true",
        help="Do not emit BEGIN/END markers (pure concatenation)",
    )

    args = ap.parse_args(argv)

    script_path = Path(__file__)
    root = find_project_root(script_path)

    # ルート直下の algorithm/ と data-structure/ を include 探索対象に含める
    include_dirs = [
        root,  # ルート直下にある "all.hpp" を拾えるように
        root / "algorithm",
        root / "data-structure",
    ]

    # input の場所解決（scripts/からの相対でも、rootからの相対でも動くように）
    inp = args.input
    if not inp.is_absolute():
        # まず root 基準を優先
        cand = (root / inp).resolve()
        inp = cand if cand.exists() else (Path.cwd() / args.input).resolve()

    if not inp.exists():
        print(f"error: input not found: {inp}", file=sys.stderr)
        return 2

    seen: Set[Path] = set()
    expanded_lines = expand_file(inp, include_dirs, seen, emit_markers=not args.no_markers)
    args.output.write_text("".join(expanded_lines), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
