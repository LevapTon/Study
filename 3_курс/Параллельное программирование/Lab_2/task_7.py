def gaussian_elimination(A, b):
    n = len(A)

    # Прямой ход
    for i in range(n):
        # Поиск максимального элемента в столбце под текущим элементом
        max_row = i
        for k in range(i + 1, n):
            if abs(A[k][i]) > abs(A[max_row][i]):
                max_row = k

        # Обмен строк
        A[i], A[max_row] = A[max_row], A[i]
        b[i], b[max_row] = b[max_row], b[i]

        # Приведение матрицы к треугольному виду
        for k in range(i + 1, n):
            factor = -A[k][i] / A[i][i]
            for j in range(i, n):
                A[k][j] += factor * A[i][j]
            b[k] += factor * b[i]

    # Обратный ход
    x = [0] * n
    for i in range(n - 1, -1, -1):
        x[i] = b[i] / A[i][i]
        for k in range(i - 1, -1, -1):
            b[k] -= A[k][i] * x[i]

    return x


A = [[2, 1, -1],
    [-3, -1, 2],
    [-2, 1, 2]]
b = [1, 2, 3]

solution = gaussian_elimination(A, b)
print("-"*4,"ЗАДАНИЕ 7","-"*4)
print("Решение СЛАУ:" , solution)
