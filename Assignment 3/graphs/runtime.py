import matplotlib.pyplot as plt

threads = [1, 2, 4, 8, 16, 32, 64, 128]
times = [
    472.162383,
    260.581648, 
    132.392202, 
    68.491152, 
    34.749140, 
    17.456178, 
    9.296636, 
    6.948887
    ]

ideal_times = [times[0]/threads[i] for i in range(len(threads))]

plt.figure(figsize=(10, 6))
plt.plot(threads, times, marker='o', linestyle='-', color='b', label="Experiment results")
plt.plot(threads, ideal_times, marker='o', linestyle='-', color='r', label="Ideal results")
plt.title('Execution time speedup vs thread number')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.grid(True)
plt.xticks(threads)
plt.tight_layout()

plt.legend()

plt.savefig('runtime.png')
plt.show()