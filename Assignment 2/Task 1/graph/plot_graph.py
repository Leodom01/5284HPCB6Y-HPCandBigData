import matplotlib.pyplot as plt

threads = [8,12,16,20,24,28,32,36,40,44,48]
times = [
    2.178053,
    1.450617,
    1.093899,
    0.911311,
    0.787798,
    0.684702,
    0.574708,
    0.778560,
    0.806757,
    0.685874,
    0.744235  
    ]

plt.figure(figsize=(10, 6))
plt.plot(threads, times, marker='o', linestyle='-', color='b')
plt.title('Execution Time of Pi Computation vs Number of Threads')
plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (seconds)')
plt.grid(True)
plt.xticks(threads)
plt.xticks(threads[::5]) 
plt.tight_layout()

plt.savefig('graph_task_1.png')
plt.show()