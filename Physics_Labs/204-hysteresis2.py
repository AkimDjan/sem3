import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

xi = 5.658 # Дж / (м^3 * В^2)
V = 7.069 * 10**(-7)

# -----------------------
# 1. ДАННЫЕ
# -----------------------
f = np.array([400, 600, 800, 1000], dtype=float)
Wf = np.array([4.8, 2.6, 1.8, 1.4], dtype=float) * 10**(-6)

U = np.array([14.51, 13, 10.49, 9.5], dtype=float)
Wu = np.array([2, 1.5, 0.8, 0.52], dtype=float) * 10**(-6)

@np.vectorize
def phi(W, Ux, Uy):
    thing = W / (np.pi * Ux * Uy * xi * V)
    if thing > 1:
        return 90
    return np.degrees(np.arcsin(W / (np.pi * Ux * Uy * xi * V)))

Uxmax_f = np.array([17.5, 17.5, 17.5, 17.5], dtype=float) * 0.001
Uymax_f = np.array([23, 16, 12.5, 11], dtype=float)

Uxmax_u = np.array([10, 8, 7, 5.32], dtype=float) * 0.001
Uymax_u = np.array([20, 18, 11.5, 11], dtype=float)

phi_f = phi(Wf, Uxmax_f, Uymax_f)
phi_U = phi(Wu, Uxmax_u, Uymax_u)

print(f"phi(f): {phi_f}")
print(f"phi(U): {phi_U}")

# -----------------------
# 5. РАСЧЕТ ЗАДЕРЖКИ ПО ВРЕМЕНИ (для зависимости от частоты)
# -----------------------

# Перевод угла из градусов в радианы
phi_f_rad = np.radians(phi_f)

# Расчет задержки по времени (секунды)
# Используем f из первой серии данных
delta_t_f = phi_f_rad / (2 * np.pi * f) 

print("---")
print(f"Задержка по времени Delta_t(f) (с): {delta_t_f}")

# -----------------------
# 6. ПОСТРОЕНИЕ ГРАФИКОВ
# -----------------------

# --- График 1: Сдвиг фаз (phi) от частоты (f) ---
plt.figure(figsize=(8, 6))
plt.plot(f, phi_f, 'o-', label="Сдвиг фаз $\phi(f)$")

plt.xlabel("Частота $f$, Гц")
plt.ylabel("Сдвиг фаз $\phi$, градусы")
plt.title("Зависимость сдвига фаз от частоты (U=const)")
plt.grid(True)
plt.legend()
plt.show()

# --- График 2: Задержка по времени (Delta t) от частоты (f) ---
plt.figure(figsize=(8, 6))
plt.plot(f, delta_t_f, 's-', label="Задержка $\Delta t(f)$")

plt.xlabel("Частота $f$, Гц")
plt.ylabel("Задержка по времени $\Delta t$, с")
plt.title("Зависимость задержки по времени от частоты (U=const)")
plt.grid(True)
plt.ticklabel_format(axis='y', style='sci', scilimits=(0,0)) # Формат для маленьких чисел
plt.legend()
plt.show()