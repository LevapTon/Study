import numpy as np
from numba import jit

@jit(parallel=True)
def backward_substitution(matrix_A, vector_b):
    n = len(vector_b)
    x = np.zeros(n)

    for i in range(n - 1, -1, -1):
        x[i] = vector_b[i]
        for j in range(i + 1, n):
            x[i] -= matrix_A[i, j] * x[j]
        x[i] /= matrix_A[i, i]

    return x

# Пример использования
matrix_A = np.array([[2, 3, 0],
                     [0, 5, 6],
                     [0, 0, 7]], dtype=np.float64)
vector_b = np.array([10, 15, 14], dtype=np.float64)

solution = backward_substitution(matrix_A, vector_b)
print("-"*4,"ЗАДАНИЕ 4","-"*4)
print(solution)
