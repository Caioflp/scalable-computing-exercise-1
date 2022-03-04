import os
import wget

if not os.path.exists("./shakespeare.txt"):
    url = "https://www.gutenberg.org/files/100/100-0.txt"
    wget.download(url, "./shakespeare.txt")
    
if not os.path.exists("./shakespeare300.txt"):
    # Create empty file
    open("./shakespeare300.txt", 'a').close()
    # Get lower content
    with open("./shakespeare.txt", "r+b") as fo:
        fo.seek(0)
        content = fo.read().lower()
    # Repeat 300 times
    with open("./shakespeare300.txt", "r+b") as f:
        for i in range(300):
            f.write(content)
