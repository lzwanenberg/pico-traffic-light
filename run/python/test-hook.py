#!/usr/bin/env python

import subprocess

build_command = ["./run/build.sh"]
test_command = ["./run/test.sh", "-r", "compact", "-s"]

def run_hook(filepath):
    subprocess.call(build_command)
    subprocess.call(test_command)

run_hook("./todo")
