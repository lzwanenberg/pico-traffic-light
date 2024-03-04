#!/usr/bin/env python

import subprocess
import inotify.adapters
from threading import Timer

debounce_time = 0.05
timer = None


build_command = ["./run/build.sh"]

def get_filename(filepath):
    return filepath.split('/')[-1]

def get_filename_stem(filepath):
    return get_filename(filepath).split(".")[0]

def get_tag(filepath):
    return f'[#{get_filename_stem(filepath)}.test]'

def get_test_command(filepath):
    return ["./run/test.sh", "-r", "compact", "-s", "-#", get_tag(filepath)]

def run_tests(filepath):
    subprocess.call(build_command)
    subprocess.call(get_test_command(filepath))

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
