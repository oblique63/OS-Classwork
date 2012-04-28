import matplotlib.pyplot as plt
from matplotlib.mlab import csv2rec

unix_single = csv2rec('output/unix_search_singlecore.csv', delimiter=',')
unix_multi = csv2rec('output/unix_search_multicore.csv', delimiter=',')

win_single = csv2rec('output/win_search_singlecore.csv', delimiter=',')
win_multi = csv2rec('output/win_search_multicore.csv', delimiter=',')

# Page Faults vs Number of Frames
#plt.yscale('log', basey=2)

plt.xlabel("Threads")
plt.ylabel("Time (micro-seconds)")

plt.title("Unix Single-core")
plt.plot(unix_single.threads, unix_single.time, color='g', linestyle='-', marker='o')
plt.savefig('output/unix_singlecore_graph')
plt.close()

#plt.yscale('log', basey=2)
plt.xlabel("Threads")
plt.ylabel("Time (micro-seconds)")

plt.title("Unix Dual-core")
plt.plot(unix_multi.threads, unix_multi.time, color='r', linestyle='-', marker='o')
plt.savefig('output/unix_multicore_graph')
plt.close()

#plt.yscale('log', basey=2)
plt.xlabel("Threads")
plt.ylabel("Time (micro-seconds)")

plt.title("Windows Single-core")
plt.plot(win_single.threads, win_single.time, color='b', linestyle='-', marker='o')
plt.savefig('output/win_singlecore_graph')
plt.close()

#plt.yscale('log', basey=2)
plt.xlabel("Threads")
plt.ylabel("Time (micro-seconds)")

plt.title("Windows Dual-core")
plt.plot(win_multi.threads, win_multi.time, color='y', linestyle='-', marker='o')
plt.savefig('output/win_multicore_graph')
plt.close()
