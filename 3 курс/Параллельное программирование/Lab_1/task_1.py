import multiprocessing

# Решение методом "Разделяй и властвуй"

# Функция для вычисления значения многочлена в точке
def evaluate_polynomial(coeffs, x, queue):
    n = len(coeffs)
    if n == 1:
        queue.put(coeffs[0])
    else:
        # Разделяем многочлен на две части
        mid = n // 2
        left_coeffs = coeffs[:mid]
        right_coeffs = coeffs[mid:]

        # Создаем очереди для получения результатов из процессов
        left_queue = multiprocessing.Queue()
        right_queue = multiprocessing.Queue()

        # Создаем процессы для параллельного вычисления значений каждой части
        left_process = multiprocessing.Process(target=evaluate_polynomial, args=(left_coeffs, x, left_queue))
        right_process = multiprocessing.Process(target=evaluate_polynomial, args=(right_coeffs, x, right_queue))

        # Запускаем процессы
        left_process.start()
        right_process.start()

        # Ждем завершения процессов
        left_process.join()
        right_process.join()

        # Получаем результаты из процессов
        left_result = left_queue.get()
        right_result = right_queue.get()

        # Объединяем результаты
        queue.put(left_result + right_result * x ** mid)

if __name__ == "__main__":
    # Коэффициенты многочлена
    coeffs = [2, -3, 1]  # Пример: 2 - 3x + x^2 + 5x^4
    x = 5  # Точка, в которой нужно вычислить значение многочлена

    queue = multiprocessing.Queue()
    evaluate_polynomial(coeffs, x, queue)
    result = queue.get()
    print("-"*4,"ЗАДАНИЕ 1.1","-"*4)
    print("Значение многочлена в точке", x, ":", result)
    print("–"*21)
