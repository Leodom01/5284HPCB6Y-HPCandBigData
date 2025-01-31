# Task 1

import pycuda.autoinit
import pycuda.driver as cuda
import numpy as np
from pycuda.compiler import SourceModule
import time 
import csv
import pandas as pd

# Initialize input array with size=1024 since it's the highest and allows us to see the more differences
# We do not time this since it is the imput creation
gpu_kernel = []
gpu_mem = []
gpu_comp = []
cpu_iter = []
cpu_pythonic = []
size = []
for input_size in [1000, 5000, 25000, 125000, 625000, 3125000, 6250000, 12500000]:
    for _ in range(4):
        size.append(input_size)
        host_array = np.random.randint(low=0, high=10, size=input_size, dtype=np.int32)

        start_full_gpu = cuda.Event()
        end_full_gpu = cuda.Event()
        end_kernel_gpu = cuda.Event()
        start_computation_gpu = cuda.Event()
        end_computation_gpu = cuda.Event()

        #################### Satrt GPU timing
        start_full_gpu.record()

        # CUDA kernel for left rotation
        kernel_code = """
        __global__ void left_rotation(int *arr, int *output, int n) {
            int idx = blockIdx.x * blockDim.x + threadIdx.x;
            if (idx < n) {
                int new_idx = (idx + n - 1) % n;
                output[new_idx] = arr[idx];
            }
        }
        """

        # Compile the CUDA kernel
        mod = SourceModule(kernel_code)
        left_rotation = mod.get_function("left_rotation")

        #################### End kernel GPU timing
        end_kernel_gpu.record()

        # Allocate memory on GPU
        device_array = cuda.mem_alloc(host_array.nbytes)
        device_output = cuda.mem_alloc(host_array.nbytes)

        # Copy data to GPU
        cuda.memcpy_htod(device_array, host_array)

        # Define block and grid size
        # More than one block to get best performance possible
        block_size = 128 
        grid_size = (input_size + block_size - 1) // block_size  

        #################### Start computation GPU timing
        start_computation_gpu.record()
        # Launch kernel
        left_rotation(device_array, device_output, np.int32(input_size), block=(block_size, 1, 1), grid=(grid_size, 1))
        #################### End computation GPU timing
        end_computation_gpu.record()

        # Copy result back to CPU
        host_output = np.empty_like(host_array)
        cuda.memcpy_dtoh(host_output, device_output)

        #################### End GPU timing
        end_full_gpu.record()
        cuda.Context.synchronize()
        gpu_full_time = start_full_gpu.time_till(end_full_gpu)
        gpu_kernel_time = start_full_gpu.time_till(end_kernel_gpu)
        computation_full_time = start_computation_gpu.time_till(end_computation_gpu)

        # Store output
        gpu_kernel.append(gpu_kernel_time)
        gpu_comp.append(computation_full_time)
        gpu_mem.append(gpu_full_time-gpu_kernel_time-computation_full_time)
        # CPU Sequential implementation (naive)

        #################### Start CPU timing
        start_cpu = time.time()

        naive_out = np.empty(input_size)
        for idx, item in enumerate(host_array):
            naive_out[(idx-1)%input_size] = item
            
        #################### End CPU timing
        end_cpu = time.time()
        cpu_time = (end_cpu - start_cpu)*1000
        cpu_iter.append(cpu_time)

        # CPU sequential implementation (pythonic)

        #################### Start CPU timing
        start_cpu = time.time()

        pythonic_out = np.concatenate((host_array[1:], host_array[:1])) 

        #################### End CPU timing
        end_cpu = time.time()
        cpu_time = (end_cpu - start_cpu)*1000
        cpu_pythonic.append(cpu_time)

        # Task 2
        # Compare the different implementations
        if (host_output == pythonic_out).all() and (pythonic_out == np.array(naive_out, dtype=np.int32)).all():
            print("Output OK")
        else:
            print("Output do not match, please investigate!")

    # Save results to output file
    rows = zip(size, gpu_kernel, gpu_mem, gpu_comp, cpu_iter, cpu_pythonic)

data = { 
    'size': size,
    'kernel_creation': gpu_kernel,
    'mem_management': gpu_mem,
    'gpu_computation': gpu_comp,
    'cpu_iterative': cpu_iter,
    'cpu_pythonic': cpu_pythonic
}

df = pd.DataFrame(data)
to_save = df[df['kernel_creation'] <= 5].groupby('size').mean()
to_save.to_csv("output.csv")

#with open("output.csv", "w") as file: 
#    writer = csv.writer(file)
#    writer.writerow(["size", "kernel_creation", "memory_mgmt", "gpu_computation", "cpu_iter", "cpu_pythonic"])
#    for size, gpu_kernel, gpu_mem, gpu_comp, cpu_iter, cpu_pythonic in zip(size, gpu_kernel, gpu_mem, gpu_comp, cpu_iter, cpu_pythonic):
#        writer.writerow([size, gpu_kernel, gpu_mem, gpu_comp, cpu_iter, cpu_pythonic])
