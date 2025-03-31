import numpy as np
import matplotlib.pyplot as plt

# Функция для построения графика
def plot_constraints():
    # Определение ограничений
    x = np.linspace(0, 10, 400)
    y1 = (3*x + 9) / 2
    y2 = (27 - 3*x) / 4
    y3 = 14 - 2*x

    # Отображение ограничений
    plt.plot(x, y1, label='-3x + 2y <= 9')
    plt.plot(x, y2, label='3x + 4y >= 27')
    plt.plot(x, y3, label='2x + y <= 14')

    plt.plot([-6, 0, 2, 14], [-3, 0, 1, 7], linestyle='--')
    plt.plot([-6.2, 3.8, 5.8, 15.8], [-3.6, 1.4, 2.4, 7.4], linestyle='--')

    # Ограничение по x >= 0
    plt.axvline(x=0, color='black', linestyle='--')

    # Ограничение по y >= 0
    plt.axhline(y=0, color='black', linestyle='--')

    # Градиент исходной функции
    gradient = [1, -2]
    plt.arrow(0, 0, gradient[0], gradient[1], head_width=0.3, head_length=0.3, fc='blue', ec='blue', label='Градиент функции', zorder=5)

    plt.fill_between(x, y3, y2, where=(y3 >= y2), color='gray', alpha=0.5, label='ОДУ')
    plt.fill_between(x, y3, y1, where=(y1 <= y3), color='white')

    # Задаем область для рисования графика
    plt.xlim(-3, 12)
    plt.ylim(-3, 12)

    # Название графика и подписи осей
    plt.title('Симплекс метод. Графически')
    plt.xlabel('x')
    plt.ylabel('y')

    # Включение легенды
    plt.legend()

    # Вывод графика
    plt.grid(True)
    plt.show()

# Вызываем функцию для построения графика
plot_constraints()
