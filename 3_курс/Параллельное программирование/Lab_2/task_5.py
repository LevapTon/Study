import numpy as np

def solve_matrix_equation(A, B):
    # Транспонируем матрицу B
    B_transpose = np.transpose(B)
    
    # Вычисляем значение выражения B + B^T
    B_sum = B + B_transpose
    
    # Вычисляем значение выражения A^T * (B + B^T)
    result_intermediate = np.matmul(np.transpose(A), B_sum)
    
    # Вычисляем значение конечного выражения A^T * (B + B^T) * A
    result_final = np.matmul(result_intermediate, A)
    
    return result_final

# Пример матриц A и B
A = np.array([[5, 6],
              [8, 7]])

B = np.array([[1, 3],
              [4, 2]])
    
# Решение уравнения
solution = solve_matrix_equation(A, B)
print("-"*4,"ЗАДАНИЕ 5","-"*4)
print("Решение матричного уравнения A^T * (B + B^T) * A:")
print(solution)
