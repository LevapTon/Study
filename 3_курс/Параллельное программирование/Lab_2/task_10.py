import numpy as np

def split_matrix(matrix):
    """
    Разбивает матрицу на четыре подматрицы.
    """
    row, col = matrix.shape
    row2, col2 = row // 2, col // 2
    return matrix[:row2, :col2], matrix[:row2, col2:], matrix[row2:, :col2], matrix[row2:, col2:]

def strassen_multiply(matrix1, matrix2):
    """
    Умножение матриц методом Штрассена.
    """
    if min(matrix1.shape) <= 64:  # В случае небольших матриц, используем обычное умножение
        return matrix1.dot(matrix2)

    a, b, c, d = split_matrix(matrix1)
    e, f, g, h = split_matrix(matrix2)

    p1 = strassen_multiply(a, f - h)
    p2 = strassen_multiply(a + b, h)
    p3 = strassen_multiply(c + d, e)
    p4 = strassen_multiply(d, g - e)
    p5 = strassen_multiply(a + d, e + h)
    p6 = strassen_multiply(b - d, g + h)
    p7 = strassen_multiply(a - c, e + f)

    result = np.zeros_like(matrix1)

    result[:matrix1.shape[0] // 2, :matrix1.shape[1] // 2] = p5 + p4 - p2 + p6
    result[:matrix1.shape[0] // 2, matrix1.shape[1] // 2:] = p1 + p2
    result[matrix1.shape[0] // 2:, :matrix1.shape[1] // 2] = p3 + p4
    result[matrix1.shape[0] // 2:, matrix1.shape[1] // 2:] = p1 + p5 - p3 - p7

    return result


matrix1 = np.array([[1, 2, 3, 4],
                    [5, 6, 7, 8],
                    [9, 10, 11, 12],
                    [13, 14, 15, 16]])

matrix2 = np.array([[1, 0, 0, 1],
                    [0, 1, 1, 0],
                    [1, 0, 0, 1],
                    [0, 1, 1, 0]])

# Выводим результат перемножения
print("Первая матрица:")
print(matrix1)
print("\nВторая матрица:")
print(matrix2)
print("\nРезультат перемножения:")
print(strassen_multiply(matrix1, matrix2))
