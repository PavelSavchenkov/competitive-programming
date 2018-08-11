from common import smart_strip
import os
import sys
import time
from collections import defaultdict

def usage():
    print("Usage: \"python3 %s src-path dst-path\"" % os.path.basename(sys.argv[0]))

if len(sys.argv) < 3:
    usage()
    exit(1)

src_file_path = sys.argv[1]
dst_file_path = sys.argv[2]

src_file_path = os.path.abspath(src_file_path)
dst_file_path = os.path.abspath(dst_file_path)

if os.path.exists(dst_file_path):
    os.remove(dst_file_path)

def get_abspath_relative(start_path, relative_file_path):
    while True:
        candidate = os.path.join(start_path, relative_file_path)
        if os.path.isfile(candidate):
            return candidate
        assert not os.path.isfile(os.path.join(start_path, ".cmake_root"))
        start_path = os.path.dirname(start_path)

def parse_include(line):
    line = smart_strip(line)
    if not line.startswith("#include"):
        return None
    tokens = line.split()
    if len(tokens) != 2:
        return None
    assert tokens[0] == "#include"
    path = tokens[1]
    if len(path) < 2:
        return None
    sep = path[0]
    if sep == "\"":
        assert path[-1] == sep
    elif sep == "<":
        assert path[-1] == ">"
    return (path[1:-1], sep)

def which_local_include(source_file_path, line):
    parsed = parse_include(line)
    if parsed is None:
        return None
    path, sep = parsed
    if sep != "\"":
        return None
    dest = get_abspath_relative(os.path.dirname(source_file_path), path)
    return dest

have_includes = set()

def register_line(line):
    parsed = parse_include(line)
    if parsed is None:
        return
    path, sep = parsed
    if sep != "<":
        return
    have_includes.add(path)

def should_skip_line(line):
    line = smart_strip(line)
    if line == "#pragma once":
        return True
    parsed = parse_include(line)
    if parsed is None:
        return False
    path, sep = parsed
    if sep == "<" and path in have_includes:
        return True
    return False

result_lines = []
color = defaultdict(int)

def dfs(abspath):
    assert color[abspath] != 1
    if color[abspath] == 2:
        return
    color[abspath] = 1
    for line in open(abspath, "r").readlines():
        if should_skip_line(line):
            continue
        register_line(line)
        local_file_path = which_local_include(abspath, line)
        if local_file_path is None:
            result_lines.append(line)
        else:
            dfs(local_file_path)
    color[abspath] = 2

dfs(src_file_path)

open(dst_file_path, "w").writelines(result_lines)

