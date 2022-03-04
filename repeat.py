import os
import subprocess

for i in range(0,100):
    proc = subprocess.Popen(["./main_base", str(i+1)])
    proc.wait()