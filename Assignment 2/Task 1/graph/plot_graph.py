import matplotlib.pyplot as plt

threads = [8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48]
times = [
    2.607578, 
    2.029830, 
    1.898412, 
    1.521958, 
    1.329382, 
    1.265058, 
    1.182115, 
    1.087490, 
    0.980724, 
    0.954309, 
    0.864297
    ]

plt.figure(figsize=(10, 6))
plt.plot(threads, times, marker='o', linestyle='-', color='b')
plt.title('Execution Time of Pi Computation vs Number of Threads')
plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (seconds)')
plt.grid(True)
plt.xticks(threads)
plt.tight_layout()

plt.savefig('Assignment 2/Task 1/graph/graph_task_1.png')
plt.show()