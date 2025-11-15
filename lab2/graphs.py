import matplotlib.pyplot as plt
import numpy as np

threads = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16])
times = np.array([
    3.104505, 2.056225, 1.802762, 1.622883, 1.496913,
    1.414316, 1.485076, 1.417514, 2.687007, 2.564150,
    2.700711, 2.870689, 2.954632, 2.702372, 2.631406,
    2.366569
])

speedup = times[0] / times
efficiency = speedup / threads

# Общие настройки
plt.rcParams.update({'font.size': 10})

# 1. Время выполнения
plt.figure(figsize=(12, 5))
plt.plot(threads, times, marker='o', linewidth=2, markersize=6, color='blue')
plt.xlabel("Количество потоков")
plt.ylabel("Время (сек)")
plt.title("Время выполнения vs Количество потоков")
plt.xticks(threads)
plt.xlim(0.5, 18.5)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.show()

# 2. Ускорение
plt.figure(figsize=(12, 5))
plt.plot(threads, speedup, marker='o', linewidth=2, markersize=6, color='green')
plt.xlabel("Количество потоков")
plt.ylabel("Ускорение")
plt.title("Ускорение vs Количество потоков")
plt.xticks(threads)
plt.yticks(np.arange(0, 2.4, 0.2))
plt.xlim(0.5, 18.5)
plt.ylim(0, 2.2)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.show()

# 3. Эффективность
plt.figure(figsize=(12, 6))
plt.plot(threads, efficiency, marker='o', linewidth=2, markersize=7, color='red', label='Эффективность')

# for i, txt in enumerate(efficiency):
#     plt.annotate(f"{txt:.2f}", (threads[i], efficiency[i]),
#                  textcoords="offset points", xytext=(0, 8),
#                  ha='center', fontsize=8, color='darkred')

plt.xlabel("Количество потоков", fontsize=12)
plt.ylabel("Эффективность", fontsize=12)
plt.title("Эффективность vs Количество потоков", fontsize=14)


plt.xticks(threads)

max_eff = np.max(efficiency)
plt.ylim(0, max_eff * 1.1)

# Явные деления по Y с шагом 0.05
y_ticks = np.arange(0, max_eff * 1.1 + 0.01, 0.05)
plt.yticks(y_ticks)

plt.xlim(0.5, 18.5)
plt.grid(True, alpha=0.4)
plt.legend()
plt.tight_layout()
plt.show()
