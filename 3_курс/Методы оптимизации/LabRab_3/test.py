import numpy as np
from scipy.optimize import linprog

# Целевая функция и ограничения
c = [-1, 2]
A = [[-3, 2], [-3, -4], [2, 1]]
b = [9, -27, 14]

# Преобразуем задачу к стандартной форме
# Добавляем искусственный базис
A = np.array(A)
A = np.hstack((A, np.eye(3)))
c = np.hstack((c, np.zeros(3)))

# Решаем задачу с искусственным базисом
res = linprog(c, A_eq=A, b_eq=b, method='simplex')

# Выводим результаты
print('Optimal value:', -res.fun)
print('Optimal solution:', res.x[:2])
