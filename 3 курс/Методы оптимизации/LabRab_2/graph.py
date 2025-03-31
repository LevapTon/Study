import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Создание данных
x1 = np.linspace(0, 0.3, 100)
x2 = np.linspace(-0.35, -0.2, 100)
x1, x2 = np.meshgrid(x1, x2)
z = x1**2 + 2*x2**2 + np.exp(x1**2 + x2**2) - x1 + 2*x2

# Построение графика
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(x1, x2, z, cmap='viridis')

# Настройка осей
ax.set_xlabel('X1')
ax.set_ylabel('X2')
ax.set_zlabel('Z')
ax.set_title('График функции f(x1, x2)')

# Вывод графика
plt.show()
