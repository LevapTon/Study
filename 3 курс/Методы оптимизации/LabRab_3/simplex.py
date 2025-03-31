import numpy as np

def simplex_method(A, b, c):
    m, n = A.shape
    # Добавление искусственного базиса
    A_artificial = np.hstack((A, np.eye(m)))
    c_artificial = np.hstack((np.zeros(n), np.ones(m)))
    
    # Исходная симплекс-таблица
    tableau = np.vstack((np.hstack((c, np.zeros((1, m)))), np.concatenate((A_artificial, b.reshape(-1, 1)), axis=1)))
    
    while True:
        # Шаг 4: Проверка условия: все σk ≥ 0
        if all(tableau[0, :-1] >= 0):
            break
        
        # Шаг 5: Выбор разрешающего столбца
        col_idx = np.where(tableau[0, :-1] < 0)[0][0]
        
        # Шаг 6: Проверка условия: все ain ≤ 0
        if all(tableau[1:, col_idx] <= 0):
            return None, None  # Задача неограничена
        
        # Шаг 7: Выбор разрешающей строки
        ratios = tableau[1:, -1] / tableau[1:, col_idx]
        row_idx = np.where(ratios == np.min(ratios[ratios > 0]))[0][0] + 1
        
        # Шаг 8: Пересчет элементов симплекс-таблицы
        pivot = tableau[row_idx, col_idx]
        tableau[row_idx, :] /= pivot
        for i in range(m + 1):
            if i != row_idx:
                multiplier = tableau[i, col_idx]
                tableau[i, :] -= multiplier * tableau[row_idx, :]
    
    basic_variables = np.where(np.sum(tableau[:, :-1], axis=1) == 1)[0]
    solution = np.zeros(n)
    for i, var in enumerate(basic_variables):
        solution[var] = tableau[i + 1, -1]
    
    return solution[:n], tableau

# Заданные данные
c = np.array([1, -2])
A = np.array([[-1, 2], [3, 4], [2, 1]])
b = np.array([9, 27, 14])

# Решение симплекс-методом
solution, tableau = simplex_method(A, b, c)

if solution is not None:
    print("Оптимальное решение:", solution)
    print("Значение целевой функции в оптимальной точке:", np.dot(c, solution))
    print("Симплекс-таблица:")
    print(tableau)
else:
    print("Задача неограничена")
