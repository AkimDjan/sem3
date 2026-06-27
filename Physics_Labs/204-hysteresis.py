import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter

# -----------------------
# 1. ДАННЫЕ
# -----------------------
f = np.array([400, 600, 800, 1000], dtype=float)
Wf = np.array([4.8, 2.6, 1.8, 1.4], dtype=float)

A = np.array([14.51, 13, 10.49, 9.5], dtype=float)
WA = np.array([2.6, 1.5, 0.8, 0.52], dtype=float)

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
    formatter.set_scientific(False) # Никаких e+02
    
    ax.xaxis.set_major_formatter(formatter)
    ax.yaxis.set_major_formatter(formatter)
    
    # 3. Убираем мелкие "логарифмические" деления, которые портят вид
    ax.minorticks_off()
    
    # 4. Включаем сетку только по нашим основным делениям
    ax.grid(True, which='major', linestyle='-', linewidth=0.8, color='gray', alpha=0.5)

# --- График 1: W(f) ---
# Генерируем красивые "круглые" числа для осей
ticks_x_f = [400, 500, 600, 700, 800, 900, 1000]
ticks_y_f = [1, 1.5, 2, 2.5, 3, 4, 5]

f_fit = np.linspace(400, 1000, 100)
W_fit_f = a * f_fit**n

plt.figure(figsize=(6, 6)) # Делаем фигуру квадратной
plt.loglog(f, Wf, 'o', markersize=8, label="Эксперимент")
plt.loglog(f_fit, W_fit_f, '-', label=f"Аппроксимация Q(f) = a * f^n")

plt.xlabel("Частота f, Гц")
plt.ylabel("Потери, мкДж")
plt.title("Зависимость Q(f)")

# Применяем нашу жесткую сетку
ax = plt.gca()
make_grid_fixed_and_square(ax, ticks_x_f, ticks_y_f)

# Ограничиваем оси, чтобы сетка не 'уплывала'
plt.xlim(380, 1050)
plt.ylim(0.9, 5.5)

plt.legend()
plt.show()

# --- График 2: W(A) ---
# Генерируем красивые числа для напряжения
ticks_x_A = [9, 10, 11, 12, 13, 14, 15]
ticks_y_A = [0.5, 0.8, 1, 1.5, 2, 2.5, 3]

A_fit = np.linspace(9, 15, 100)
W_fit_A = b * A_fit**m

plt.figure(figsize=(6, 6)) # Квадратная фигура
plt.loglog(A, WA, 'o', markersize=8, label="Эксперимент")
plt.loglog(A_fit, W_fit_A, '-', label=f"Аппроксимация Q(U) = b * U^m")

plt.xlabel("Амплитуда U, В")
plt.ylabel("Потери, мкДж")
plt.title("Зависимость Q(U)")

# Применяем жесткую сетку
ax = plt.gca()
make_grid_fixed_and_square(ax, ticks_x_A, ticks_y_A)

plt.xlim(8.8, 15.5)
plt.ylim(0.45, 3.2)

plt.legend()
plt.show()