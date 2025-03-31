import numpy as np

def inverse_diagonal_matrix(matrix):
    """
    Function to invert a two-diagonal matrix using the identity matrix.
    """
    n = len(matrix)
    identity = np.eye(n)
    inversed_matrix = np.zeros((n, n))

    # First, create the inverse matrix
    for i in range(n):
        if i == 0:
            inversed_matrix[i][i] = 1 / matrix[i][i]
        else:
            inversed_matrix[i][i] = 1 / (matrix[i][i] - matrix[i-1][i] * inversed_matrix[i-1][i])

    # Then, perform backward substitution
    for i in range(n-1, -1, -1):
        identity[i] /= inversed_matrix[i][i]
        if i > 0:
            identity[i-1] -= matrix[i-1][i] * identity[i]

    return identity

# Example usage:
matrix = np.array([[1, 0, 0],
                   [4, 2, 0],
                   [0, 5, 3]])

inversed_matrix = inverse_diagonal_matrix(matrix)
print("Inversed Matrix:")
print(inversed_matrix)
