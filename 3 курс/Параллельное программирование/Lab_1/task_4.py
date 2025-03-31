import numpy as np

nmax = 10

A = np.zeros((nmax, nmax), dtype=float)
P = np.zeros(nmax, dtype=int)

n = 3  # Размерность матрицы А
A_input = [
    [3, 8, 0],
    [0, 2, 9],
    [0, 0, 6]
]

for i in range(n):
    for j in range(n):
        A[i, j] = A_input[i][j]

for i in range(n):
    P[i] = i

for k in range(n):
    e = 0
    b = 0
    for i in range(k, n):
        if e < abs(A[i, k]):
            e = abs(A[i, k])
            b = i
    if e == 0:
        print('Матрица вырождена')
        break
    if k < b:
        for j in range(n):
            s = A[k, j]
            A[k, j] = A[b, j]
            A[b, j] = s
        t = P[k]
        P[k] = P[b]
        P[b] = t
    A[k, k] = 1 / A[k, k]
    for j in range(n):
        if j == k:
            continue
        s = A[k, j] * A[k, k]
        for i in range(n):
            if i == k:
                continue
            A[i, j] = A[i, j] - A[i, k] * s
        A[k, j] = s
    for i in range(n):
        if i == k:
            continue
        A[i, k] = -A[i, k] * A[k, k]

for k in range(n):
    if k < P[k]:
        i = P[k]
        for j in range(n):
            s = A[j, k]
            A[j, k] = A[j, i]
            A[j, i] = s

print("-"*4,"ЗАДАНИЕ 1.4","-"*4)
for i in range(n):
    print()
    for j in range(n):
        print(f'{A[i, j]:10.5f}', end='')
print("\n","–"*21)
