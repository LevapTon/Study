class MaxCounter:
    def __init__(self):
        self.max_value = None
        self.max_count = 0

    def send(self, message):
        if message == "get_max_count":
            return self.max_count
        elif isinstance(message, list):
            self.process_list(message)

    def process_list(self, num_list):
        if not num_list:
            return

        max_val = max(num_list)

        count = num_list.count(max_val)

        if self.max_value is None or max_val > self.max_value:
            self.max_value = max_val
            self.max_count = count
        elif max_val == self.max_value:
            self.max_count += count


if __name__ == "__main__":
    actor = MaxCounter()

    num_list = [1, 3, 5, 3, 7, 5, 9, 5, 7, 9]

    actor.send(num_list)

    max_count = actor.send("get_max_count")
    print("Количество максимальных значений в списке:", max_count)
