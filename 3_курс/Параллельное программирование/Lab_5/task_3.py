import threading

class FileReaderWriter:
    def __init__(self, file_path):
        self.file_path = file_path
        self.lock = threading.Lock()

    def read_file(self):
        with self.lock:
            try:
                with open(self.file_path, 'r', encoding='UTF-8') as file:
                    return file.read()
            except FileNotFoundError:
                return None

    def write_to_file(self, data):
        with self.lock:
            try:
                with open(self.file_path, 'a', encoding='UTF-8') as file:
                    file.write(data+'\n')
                return True
            except Exception as e:
                return False


file_path = 'example.txt'
file_reader_writer = FileReaderWriter(file_path)

result = file_reader_writer.read_file()
if result is not None:
    print("Прочитано из файла:", result)
else:
    print("Файл не найден или пустой.")

print("Введите данные для записи:")
data_to_write = input()
write_result = file_reader_writer.write_to_file(data_to_write)
if write_result:
    print("Данные успешно записаны в файл.")
else:
    print("Ошибка при записи данных в файл.")
