import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

xi = 5.658  # Дж / (м^3 * В^2)

R_1 = 11_000
R_2 = 22_000
C = 4 * 10 ** (-6)

d = 25 * 10 ** (-3)

# площадь одного квадратика
S = (3 * 10 ** (-3)) ** 2

# кол-во витков
W_1 = 150
W_2 = 300

U = 27.0
V = 7.069 * 10 ** -7  # объем тороида
kappa = (W_1 * R_2 * C) / (W_2 * R_1 * np.pi * d * S)
u_x = 5.00 * 10 ** (-3)
u_y = 10.00


def calc(N):
    Q = kappa * V * 5 * 10 ** (-2) * N

    return Q


cubs_1 = np.array([19.5, 16, 13.5, 12])
cubs_2 = np.array([9, 6, 2.5])
# -----------------------
# 1. ДАННЫЕ
# -----------------------
f = np.array([500, 700, 900, 1100], dtype=float)
Wf = np.array([calc(x) for x in cubs_1], dtype=float)

A = np.array([10, 7, 4], dtype=float)
Wu = np.array([calc(x) for x in cubs_2], dtype=float)


@np.vectorize
def phi(W, Ux, Uy):
    thing = W / (np.pi * Ux * Uy * xi * V)
    if thing > 1:
        return 90
    return np.degrees(np.arcsin(W / (np.pi * Ux * Uy * xi * V)))


Uymax_f = np.array([21, 17.5, 15.5, 14.5], dtype=float)
Uxmax_f = np.array([16.25, 17.5, 17.5, 17.5], dtype=float) * 0.001

# Uxmax_u = np.array([13.75, 9, 7.75], dtype=float) * 0.001
# Uymax_u = np.array([12.0, 13.75, 9.5], dtype=float)

phi_f = phi(Wf, Uxmax_f, Uymax_f)
# phi_U = phi(Wu, Uxmax_u, Uymax_u)

print(f"phi(f): {phi_f}")
# print(f"phi(U): {phi_U}")

# --- Продолжение существующего кода ---

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
plt.ticklabel_format(axis='y', style='sci', scilimits=(0, 0))  # Формат для маленьких чисел
plt.legend()
plt.show()


# Время релаксации, Тл А/м переделать графики 
# Чем отличаются пара, ферро и диамагнетики
