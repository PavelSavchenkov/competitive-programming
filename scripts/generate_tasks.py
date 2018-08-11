import sys
import os
import shutil
from common import smart_strip

for i in range(1, len(sys.argv)):
    task_name = sys.argv[i]
    tasks_base = 'tasks'
    os.makedirs('tasks', exist_ok=True)
    task_path = os.path.join(tasks_base, task_name)

    os.makedirs(task_path, exist_ok=True)

    shutil.copyfile(os.path.join('prewritten-code', 'templates', 'main.cpp'), os.path.join(task_path, 'main.cpp'))
    open(os.path.join(task_path, 'a.in'), 'w')

    def gen_line(T):
        return "add_executable(%s %s/main.cpp)" % (T, T)

    cmake_filename = os.path.join(tasks_base, 'CMakeLists.txt')
    lines = list(map(smart_strip, open(cmake_filename, 'r').readlines())) if os.path.exists(cmake_filename) else []
    lines = [line for line in lines if line]
    cur_line = gen_line(task_name)
    if cur_line not in lines:
        lines.append(cur_line)
    lines = sorted(lines)
    lines = [line+"\n\n" for line in lines]

    open(cmake_filename, 'w').writelines(lines)
