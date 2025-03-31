import numpy as np
import multiprocessing

# метод прогонки (метод Томаса)

def forward_pass(A, b, n, queue):
    for i in range(1, n):
        ratio = A[i][i-1] / A[i-1][i-1]
        A[i] -= ratio * A[i-1]
        b[i] -= ratio * b[i-1]
    queue.put((A, b))

def backward_pass(A, b, n, queue):
    x = np.zeros(n)
    x[-1] = b[-1] / A[-1][-1]
    for i in range(n-2, -1, -1):
        x[i] = (b[i] - np.dot(A[i][i+1:], x[i+1:])) / A[i][i]
    queue.put(x)

def solve_tridiagonal_system(A, b):
    n = len(b)

    forward_queue = multiprocessing.Queue()
    forward_process = multiprocessing.Process(target=forward_pass, args=(A.astype(np.float64).copy(), b.astype(np.float64).copy(), n, forward_queue))
    forward_process.start()
    forward_process.join()
    A, b = forward_queue.get()

    backward_queue = multiprocessing.Queue()
    backward_process = multiprocessing.Process(target=backward_pass, args=(A, b, n, backward_queue))
    backward_process.start()
    backward_process.join()
    x = backward_queue.get()

    return x

if __name__ == "__main__":
    A = np.array([[4, 1, 0],
                  [1, 4, 1],
                  [0, 1, 4]])

    b = np.array([1, 2, 3])

    solution = solve_tridiagonal_system(A, b)
    print("-"*4,"ЗАДАНИЕ 1.2","-"*4)
    print("Solution:", solution)
    print("–"*21)
