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


    def gen_lines(T):
        lines = []
        lines.append("set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/%s)" % T)
        lines.append("add_executable(%s %s/main.cpp)" % (T, T))
        return lines


    cmake_filename = os.path.join(tasks_base, 'CMakeLists.txt')
    lines = list(map(smart_strip, open(cmake_filename, 'r').readlines())) if os.path.exists(cmake_filename) else []
    lines = [line for line in lines if line]
    cur_lines = gen_lines(task_name)
    cnt_already = 0
    for cur_line in cur_lines:
        if cur_line in lines:
            cnt_already += 1
    if cnt_already == len(cur_lines):
        continue
    assert cnt_already == 0, "bad cmake for task %s" % task_name
    lines += cur_lines

    open(cmake_filename, 'w').writelines([line + "\n\n" for line in lines])
