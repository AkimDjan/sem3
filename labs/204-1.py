import numpy as np
import matplotlib.pyplot as plt
from math import pi, asin
from matplotlib.ticker import ScalarFormatter

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
Wf = np.array([calc(x) for x in cubs_1], dtype=float) * 10 ** (6)

A = np.array([10, 7, 4], dtype=float)
WA = np.array([calc(x) for x in cubs_2], dtype=float) * 10 ** (6)

print(WA)

# -----------------------
# 2. ФИТИРОВАНИЕ
# -----------------------
coeff_f = np.polyfit(np.log(f), np.log(Wf), 1)
n = coeff_f[0]
a = np.exp(coeff_f[1])

coeff_A = np.polyfit(np.log(A), np.log(WA), 1)
m = coeff_A[0]
b = np.exp(coeff_A[1])

print(f"Q(f): a={a:.4f}, n={n:.4f}")
print(f"Q(U): b={b:.4f}, m={m:.4f}")


# -----------------------
# 3. НАСТРОЙКА ГРАФИКОВ
# -----------------------

def make_grid_fixed_and_square(ax, x_ticks, y_ticks):
    """
      Принудительно ставит сетку только в указанных точках
      и убирает экспоненциальную запись.
      """
    # 1. Устанавливаем конкретные числа на осях
    ax.set_xticks(x_ticks)
    ax.set_yticks(y_ticks)

    # 2. Форматтер: обычные числа (не 10^x)
    formatter = ScalarFormatter()
    formatter.set_scientific(False)  # Никаких e+02

    ax.xaxis.set_major_formatter(formatter)
    ax.yaxis.set_major_formatter(formatter)

    # 3. Убираем мелкие "логарифмические" деления, которые портят вид
    ax.minorticks_off()

    # 4. Включаем сетку только по нашим основным делениям
    ax.grid(True, which='major', linestyle='-', linewidth=0.8, color='gray', alpha=0.5)


# --- График 1: W(f) ---
# Генерируем красивые "круглые" числа для осей
ticks_x_f = [0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200]
ticks_y_f = [1, 1.5, 2, 2.5, 3, 4, 5]

f_fit = np.linspace(0, 1200, 100)
W_fit_f = a * f_fit ** n

plt.figure(figsize=(6, 6))  # Делаем фигуру квадратной
plt.plot(f, Wf, 'o', markersize=8, label="Эксперимент")
plt.plot(f_fit, W_fit_f, '-', label=f"Аппроксимация Q(f) = a * f^n")

plt.xlabel("Частота f, Гц")
plt.ylabel("Потери, мкДж")
plt.title("Зависимость Q(f)")

# Применяем нашу жесткую сетку
ax = plt.gca()
make_grid_fixed_and_square(ax, ticks_x_f, ticks_y_f)

# Ограничиваем оси, чтобы сетка не 'уплывала'
plt.xlim(400, 1200)
plt.ylim(0.9, 5.5)

plt.legend()
plt.show()

# --- График 2: W(A) ---
# Генерируем красивые числа для напряжения
ticks_x_A = [3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28]
ticks_y_A = [0.5, 0.8, 1, 1.5, 2, 2.5, 3, 4, 5, 6]

A_fit = np.linspace(3, 30, 100)
W_fit_A = b * A_fit ** m

plt.figure(figsize=(6, 6))  # Квадратная фигура
plt.plot(A, WA, 'o', markersize=8, label="Эксперимент")
plt.plot(A_fit, W_fit_A, '-', label=f"Аппроксимация Q(U) = b * U^m")

plt.xlabel("Амплитуда U, В")
plt.ylabel("Потери, мкДж")
plt.title("Зависимость Q(U)")

# Применяем жесткую сетку
ax = plt.gca()
make_grid_fixed_and_square(ax, ticks_x_A, ticks_y_A)

plt.xlim(0, 11)
plt.ylim(0.2, 2.0)

plt.legend()
plt.show()
