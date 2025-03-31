import numpy as np
from numba import jit

@jit(parallel=True)
def solve_two_diag(A, b):
    N = len(b)
    alpha = np.zeros(N)
    beta = np.zeros(N)
    x = np.zeros(N)

    alpha[0] = A[0, 0]
    beta[0] = b[0]

    for i in range(1, N):
        alpha[i] = A[i, i] - A[i, i-1] * A[i-1, i] / alpha[i-1]
        beta[i] = b[i] - A[i, i-1] * beta[i-1] / alpha[i-1]

    x[N-1] = beta[N-1] / alpha[N-1]

    for i in range(N-2, -1, -1):
        x[i] = (beta[i] - A[i, i+1] * x[i+1]) / alpha[i]

    return x

# Создание матрицы и вектора
A = np.array([[2, -1, 0],
              [0, 2, -1],
              [0, 0, 2]])

b = np.array([1, 2, 3])

# Решение системы уравнений
x = solve_two_diag(A, b)

print("-"*4,"ЗАДАНИЕ 1.3","-"*4)
# Вывод результата
print("Матрица A:")
print(A)
print("\nВектор b:")
print(b)
print("\nРешение x:")
print(x)
print("–"*21)
