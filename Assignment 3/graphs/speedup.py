import matplotlib.pyplot as plt

threads = [1, 2, 4, 8, 16, 32, 64, 128]
speedups = [
    472.162383/472.162383,
    472.162383/260.581648, 
    472.162383/132.392202, 
    472.162383/68.491152, 
    472.162383/34.749140, 
    472.162383/17.456178, 
    472.162383/9.296636, 
    472.162383/6
    ]

ideal_speedup = [speedups[0]*threads[i] for i in range(len(threads))]

plt.figure(figsize=(10, 6))
plt.plot(threads, speedups, marker='o', linestyle='-', color='b', label="Experiment results")
plt.plot(threads, ideal_speedup, marker='o', linestyle='-', color='r', label="Ideal results")
plt.title('Execution time speedup vs thread number')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.grid(True)
plt.xticks(threads)
plt.tight_layout()

plt.legend()

plt.savefig('speedup.png')
plt.show()