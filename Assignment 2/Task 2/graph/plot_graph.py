import matplotlib.pyplot as plt

iterations = [31250000, 62500000, 125000000, 250000000, 500000000, 1000000000, 2000000000]
times = [0.052694, 0.096979, 0.203025, 0.400047, 0.744852, 1.060696, 1.393582]

plt.figure(figsize=(10, 6))
plt.plot(iterations, times, marker='o', linestyle='-', color='g')
plt.title('Execution Time of Pi Computation vs Number of Iterations')
plt.xlabel('Number of Iterations')
plt.ylabel('Execution Time (seconds)')
plt.grid(True)
plt.xscale('log')
plt.tight_layout()

plt.savefig('Assignment 2/Task 2/graph/graph_task_2.png')
plt.show()
