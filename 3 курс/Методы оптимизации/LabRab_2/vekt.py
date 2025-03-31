import numpy as np

# Функция и ее градиент
def func(x):
    '''
    Значение функции в точке x1 x2
    '''
    return x[0]**2 + 2*x[1]**2 + np.exp(x[0]**2 + x[1]**2) - x[0] + 2*x[1]

def gradient(x):
    df_dx1 = 2*x[0]*np.exp(x[0]**2 + x[1]**2)+2*x[0]-1 # Производная по x1 
    df_dx2 = 2*x[1]*np.exp(x[0]**2 + x[1]**2)+4*x[1]+2 # Производная по x2
    return np.array([df_dx1, df_dx2])


# Градиентный метод с изменяющимся шагом
def gradient_descent(func, gradient, start_point, learning_rate=.5, tolerance=1e-6, max_iter=100000):
    x = start_point
    prev_value = func(x)
    for i in range(max_iter):
        grad = gradient(x)
        new_x = x - learning_rate * grad
        new_value = func(new_x)
        if np.linalg.norm(new_x - x) < tolerance:
            break
        # print(new_value, prev_value, i, new_value > prev_value)
        if new_value > prev_value:
            learning_rate /= 2  # Уменьшаем learning_rate вдвое
            new_x = x - learning_rate * grad
            x = new_x
            prev_value = func(new_x)
            # print(func(new_x), prev_value, i)
        else:
            x = new_x
            prev_value = new_value
    return x, func(x), i + 1  # Возвращаем также количество итераций

# Начальная точка
start_point = np.array([2, 2])

# Запуск градиентного спуска
minimum_point, minimum_value, iterations = gradient_descent(func, gradient, start_point)

print("Минимум функции:", minimum_value)
print("Точка минимума:", minimum_point)
print("Количество итераций:", iterations)
