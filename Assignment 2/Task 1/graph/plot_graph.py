import matplotlib.pyplot as plt

threads = [4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,236,240,244,248,252,256]
times = [
    4.401381,
    2.195105,
    1.486943,
    1.094913,
    0.896487,
    0.726554,
    0.638693,
    0.544433,
    0.813295,
    0.820949,
    1.005010,
    1.125101,
    0.821422,
    1.722580,
    0.878321,
    0.910182,
    1.192308,
    1.768895,
    1.157382,
    0.888179,
    1.055259,
    0.868649,
    1.571599,
    1.278799,
    1.511296,
    0.880189,
    1.132121,
    1.402623,
    0.966250,
    1.148749,
    3.476143,
    1.366683,
    1.033702,
    1.076740,
    1.653242,
    1.379026,
    2.487939,
    2.315856,
    1.148305,
    1.209875,
    3.916071,
    1.447530,
    1.317107,
    1.768699,
    1.769624,
    2.067777,
    1.463982,
    1.888508,
    1.530433,
    1.900974,
    2.599638,
    2.210243,
    1.846338,
    3.481162,
    1.310077,
    1.425773,
    4.481463,
    3.670873,
    1.593790,
    1.772977,
    1.629327,
    6.849148,
    2.681047,
    1.928373
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

plt.savefig('Assignment 2/Task 1/graph/graph_task_1.png')
plt.show()