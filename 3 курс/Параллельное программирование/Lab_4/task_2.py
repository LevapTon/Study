import threading
import queue


data_queue = queue.Queue()
result_queue = queue.Queue()


def calculate_factorial():
    while True:
        number = data_queue.get()
        if number is None:
            break
        factorial = 1
        for i in range(1, number + 1):
            factorial *= i
        result_queue.put(factorial)


factorial_thread = threading.Thread(target=calculate_factorial)
factorial_thread.start()


numbers = [5, 3, 7, 2, 4]


for number in numbers:
    data_queue.put(number)


factorials = []
for _ in numbers:
    result = result_queue.get()
    factorials.append(result)


for num, factorial in zip(numbers, factorials):
    print("Factorial of", num, "is:", factorial)


data_queue.put(None)


factorial_thread.join()
print("Factorial calculation thread has finished.")
