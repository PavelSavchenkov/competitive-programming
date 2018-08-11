import sys
import os
import shutil

for i in range(1, len(sys.argv)):
    task_name = sys.argv[i]
    tasks_base = 'tasks'
    os.makedirs('tasks', exist_ok=True)
    task_path = os.path.join(tasks_base, task_name)

    os.makedirs(task_path, exist_ok=True)

    shutil.copyfile(os.path.join('prewritten-code', 'templates', 'main.cpp'), os.path.join(task_path, 'main.cpp'))
    open(os.path.join(task_path, 'a.in'), 'w')

    with open(os.path.join(tasks_base, 'CMakeLists.txt'), 'a+') as CMakeLists:
        CMakeLists.write("\nadd_executable(%s %s/main.cpp)\n" % (task_name, task_name))
