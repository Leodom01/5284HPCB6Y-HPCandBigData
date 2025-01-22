import matplotlib.pyplot as plt

iterations = [31250000,62500000,125000000,250000000,500000000,1000000000,2000000000]
times = [0.02228,
        0.042992,
        0.085207,
        0.179741,
        0.337964,
        0.671338,
        1.403166
]

plt.figure(figsize=(10, 6))
plt.plot(iterations, times, marker='o', linestyle='-', color='g')
plt.title('Execution Time of Pi Computation vs Number of Iterations')
plt.xlabel('Number of Iterations')
plt.ylabel('Execution Time (seconds)')
plt.grid(True)
#plt.xscale('log')
plt.tight_layout()

plt.savefig('graph_task_2.png')
plt.show()
