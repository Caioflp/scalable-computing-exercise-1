import numpy as np
from matplotlib import pyplot as plt
a = np.genfromtxt("time.txt", delimiter=" ")

plt.figure(figsize=(8,4), tight_layout=True)
plt.plot(a[:,0],a[:,1],label="Execution Time")
plt.plot(a[:,0],a[:,2],label="Total Time")
plt.xlabel('Number of Threads')
plt.ylabel('Time (ms)')
plt.title('Comparison of Threads X Time')
plt.legend()
plt.savefig("graphic.png")