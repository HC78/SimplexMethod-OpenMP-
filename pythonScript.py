import matplotlib.pyplot as plt

# Read performance gains from the file
with open('performance_gains.txt', 'r') as file:
    lines = file.readlines()

# Parse the performance gains
performance_gains = {}
for line in lines:
    parts = line.split(': ')
    if len(parts) == 2:
        key = parts[0].strip()
        value = float(parts[1].strip())
        performance_gains[key] = value

# Extract the performance gains
gains = [performance_gains[f'{i} threads'] for i in [2, 4, 8, 16, 32]]

# Plot the graph
threads = [2, 4, 8, 16, 32]
plt.plot(threads, gains, marker='o', label='Performance Gain')
plt.xlabel('Number of Threads')
plt.ylabel('Performance Gain')
plt.title('Performance Gain vs Number of Threads')
plt.legend()
plt.grid(True)
plt.show()


# # For last graph
# import matplotlib.pyplot as plt

# # Read the data from the file
# problem_sizes = []
# thread_counts = []
# performance_gains = []

# with open('performance_gains.txt', 'r') as file:
#     for line in file:
#         size, threads, gain = map(float, line.split())
#         problem_sizes.append(int(size))
#         thread_counts.append(int(threads))
#         performance_gains.append(gain)

# # Create a dictionary to store the gains for each thread count
# gains_dict = {}
# for size, threads, gain in zip(problem_sizes, thread_counts, performance_gains):
#     if threads not in gains_dict:
#         gains_dict[threads] = []
#     gains_dict[threads].append((size, gain))

# # Plot the results
# plt.figure(figsize=(10, 6))

# for threads, gains in gains_dict.items():
#     sizes, gains = zip(*sorted(gains))
#     plt.plot(sizes, gains, label=f'{threads} threads')

# plt.xlabel('Problem Size')
# plt.ylabel('Performance Gain')
# plt.title('Performance Gain vs Problem Size for Different Number of Thread')
# plt.legend()
# plt.grid(True)
# plt.show()