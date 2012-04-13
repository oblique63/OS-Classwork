import matplotlib.pyplot as plt
from matplotlib.mlab import csv2rec

virtual_memory = csv2rec('output/virtual_memory.csv', delimiter=',')

# Page Faults vs Number of Frames
#plt.yscale('log', basey=2)
plt.plot(virtual_memory.frames, virtual_memory.fifo, label='FIFO', color='r', linestyle='-', marker='o')
plt.plot(virtual_memory.frames, virtual_memory.lru, label='LRU', color='g', linestyle='-', marker='o')
plt.plot(virtual_memory.frames, virtual_memory.opt, label='OPT', color='b', linestyle='-', marker='o')

plt.xlabel("Number Of Frames")
plt.ylabel("Page Faults")
plt.title("Page Faults vs. Number Of Frames")
plt.legend(loc='upper right')

plt.savefig('output/graph')
plt.close()
