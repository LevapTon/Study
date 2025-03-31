import threading

class ParallelSum:
    def __init__(self, num_threads):
        self.num_threads = num_threads
        self.values = [0] * num_threads
        self.lock = threading.Lock()

    def add(self, index, value):
        with self.lock:
            self.values[index] += value

    def parallel_sum(self, numbers):
        chunk_size = len(numbers) // self.num_threads
        threads = []

        for i in range(self.num_threads):
            start = i * chunk_size
            end = start + chunk_size if i < self.num_threads - 1 else len(numbers)
            thread = threading.Thread(target=self.sum_worker, args=(numbers[start:end], i))
            threads.append(thread)
            thread.start()

        for thread in threads:
            thread.join()

        return sum(self.values)

    def sum_worker(self, numbers, index):
        local_sum = sum(numbers)
        self.add(index, local_sum)

if __name__ == "__main__":
    numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    num_threads = 4
    parallel_sum = ParallelSum(num_threads)
    result = parallel_sum.parallel_sum(numbers)
    print("Parallel Sum:", result)
