def gauss_jordan(matrix, vector):
    n = len(vector)

    # Прямой ход метода Гаусса
    for i in range(n):
        pivot = matrix[i][i]
        for j in range(i + 1, n):
            ratio = matrix[j][i] / pivot
            for k in range(n):
                matrix[j][k] -= ratio * matrix[i][k]
            vector[j] -= ratio * vector[i]

    # Обратный ход метода Гаусса
    for i in range(n - 1, -1, -1):
        pivot = matrix[i][i]
        for j in range(i - 1, -1, -1):
            ratio = matrix[j][i] / pivot
            for k in range(n):
                matrix[j][k] -= ratio * matrix[i][k]
            vector[j] -= ratio * vector[i]

    # Нормализация
    for i in range(n):
        vector[i] /= matrix[i][i]
        matrix[i][i] /= matrix[i][i]

    return vector

# Пример использования
matrix = [[2, -1, 1],
          [1, 1, -1],
          [3, -2, 1]]
vector = [5, -2, 9]

solution = gauss_jordan(matrix, vector)
print("-"*4,"ЗАДАНИЕ 8","-"*4)
print("Решение СЛАУ:", solution)
