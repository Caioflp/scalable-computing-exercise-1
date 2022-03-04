import os
import wget

if not os.path.exists("./shakespeare.txt"):
    url = "https://www.gutenberg.org/files/100/100-0.txt"
    wget.download(url, "./shakespeare.txt")

## with open("./shakespeare.txt", "r+b") as f:
##     f.seek(0)
##     content = f.read()
##     for i in range(300):
##         f.write(content)
