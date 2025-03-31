def backward_substitution(matrix_A, vector_b):
    n = len(vector_b)
    x = [0] * n

    for i in range(n - 1, -1, -1):
        x[i] = vector_b[i]
        for j in range(i + 1, n):
            x[i] -= matrix_A[i][j] * x[j]
        x[i] /= matrix_A[i][i]

    return x

# Пример использования
matrix_A = [[2, 3, 0],
            [0, 5, 6],
            [0, 0, 7]]
vector_b = [10, 15, 14]

solution = backward_substitution(matrix_A, vector_b)
print("-"*4,"ЗАДАНИЕ 3","-"*4)
print(solution)
