def multiply_matrices(matrix1, matrix2):
    # Проверяем, что матрицы одинакового размера и квадратные
    if len(matrix1) != len(matrix2) or len(matrix1[0]) != len(matrix2[0]) or len(matrix1) != len(matrix1[0]):
        raise ValueError("Матрицы должны быть одинакового размера и квадратные")

    # Создаем новую матрицу для хранения результата
    result = [[0 for _ in range(len(matrix1))] for _ in range(len(matrix1))]

    # Перемножаем матрицы
    for i in range(len(matrix1)):
        for j in range(len(matrix2[0])):
            for k in range(len(matrix2)):
                result[i][j] += matrix1[i][k] * matrix2[k][j]

    return result

# Пример использования
matrix1 = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
]

matrix2 = [
    [9, 8, 7],
    [6, 5, 4],
    [3, 2, 1]
]

result_matrix = multiply_matrices(matrix1, matrix2)

print("-"*4,"ЗАДАНИЕ 3.1","-"*4)
for row in result_matrix:
    print(row)
print("–"*21)
