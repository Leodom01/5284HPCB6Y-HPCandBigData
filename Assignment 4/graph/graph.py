import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

df = pd.read_csv("output.csv")
df = df[df['size'].isin([512, 1024, 2048, 4096])]

# Categories for different test cases
categories = df['size']

# Breakdown of execution time into components (Kernel Creation, Memory Management, Computation)
kernel_creation = df['kernel_creation']
memory_management = df['mem_management']
computation = df['gpu_computation']

# Total execution times (measured separately)
iterative_solution = df['cpu_iterative']

pythonic_solution = df['cpu_pythonic']

gpu_full = df['kernel_creation']+df['mem_management']+df['gpu_computation']

# Compute stacked bar heights
x = np.arange(len(categories))
bar_width = 0.4

# Plot stacked components
bottoms = np.zeros(len(categories))
components = [kernel_creation, memory_management, computation]
labels = ['Kernel Creation', 'Memory Management', 'Computation']
colors = ['yellow', 'orange', 'red']

for i in range(len(components)):
    plt.bar(x - bar_width / 2, components[i], width=bar_width, bottom=bottoms, label=labels[i], color=colors[i])
    bottoms += np.array(components[i])  # Update bottom position for stacking

# Plot total execution time
plt.bar(x + bar_width / 2, iterative_solution, width=bar_width, color='blue', label='Iterative solution')
#plt.bar(x + bar_width / 2, pythonic_solution, width=bar_width, color='grey', label='Pythonic solution')

# Labels and formatting
plt.xticks(x, categories)
plt.ylabel("Time (seconds)")
plt.xlabel("Array size")
plt.title("CUDA Execution Time Breakdown")
plt.legend()

plt.savefig("graph.png")