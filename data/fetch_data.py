import os
import wget

if not os.path.exists("./shakespeare.txt"):
    url = "https://www.gutenberg.org/files/100/100-0.txt"
    wget.download(url, "./shakespeare.txt")
    
n = 300
if not os.path.exists(f"./shakespeare{n}.txt"):
    # Create empty file
    open(f"./shakespeare{n}.txt", 'a').close()
    # Get lower content
    with open("./shakespeare.txt", "r+b") as fo:
        fo.seek(0)
        content = fo.read().lower()
    # Repeat 300 times
    with open(f"./shakespeare{n}.txt", "r+b") as f:
        for i in range(n):
            f.write(content)
