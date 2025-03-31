def output_numbers(width, height):
    import random
    with open('файлы\\Numbers_2.txt', 'w') as file:
        for i in range(height):
            for j in range(width):
                file.write(str(random.randint(-100,100)) + ' ')
            file.write('\n')

def front_end():
    'Ввод данных'
    width = int(input('Введите ширину таблицы: '))
    height = int(input('Введите высоту таблицы: '))
    output_numbers(width, height)

front_end()