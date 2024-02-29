#!/usr/bin/env python

import subprocess
import inotify.adapters
import os
import time
from threading import Timer

debounce_time = 0.05  # 50 milliseconds
timer = None

build_command = ["./run/build.sh"]
test_command = ["./run/test.sh", "-r", "compact", "-s"]

def run_tests(filepath):
    subprocess.call(build_command)
    subprocess.call(test_command)

def debounce_run_script(filepath):
    global timer
    if timer is not None:
        timer.cancel()
    timer = Timer(debounce_time, run_tests, args=[filepath])
    timer.start()

def watch_directory(directory):
    i = inotify.adapters.InotifyTree(directory)

    for event in i.event_gen():
        if event is not None:
            (_, type_names, path, filename) = event

            if "IN_MODIFY" in type_names:
                filepath = path + '/' + filename
                debounce_run_script(filepath)

if __name__ == "__main__":
    print("Watching directory ./src for file changes...")
    watch_directory("./src")
