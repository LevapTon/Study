import csv


def get_lst_dct(file_name, sep=','):
    with open(file_name, 'r', encoding='utf8') as f:
        reader = csv.DictReader(f, delimiter=sep)
        return [line for line in reader]


def get_answer(file_name, max_w, step):
    lst = get_lst_dct(file_name)
    lst.sort(key=lambda x: x['w'])
    matrix = [[[0, 0] for i in range(len(lst))] for j in range(max_w//step + 1)]
    w = 0
    while w <= max_w:
        for j in range(len(lst)):
            item_w = int(lst[j]['w'])
            '''
            проверка для первого предмета
            '''
            if (j == 0) and (matrix[w//step][j][0] + item_w <= w):
                matrix[w//step][j][0] += item_w
                matrix[w//step][j][1] += int(lst[j]['p'])
            else:
                '''
                добавление результата в правую ячейку
                '''
                matrix[w // step][j][0] = matrix[w // step][j - 1][0]
                matrix[w // step][j][1] = matrix[w // step][j - 1][1]
            if (j != 0) and (matrix[item_w//step][j - 1][0] + item_w <= w)\
                    and (matrix[item_w//step][j - 1][1] + int(lst[j]['p']) > matrix[w//step][j - 1][1]):
                '''
                если сумма с предыдущим лучше - заменить
                '''
                matrix[w // step][j][0] = matrix[item_w//step][j - 1][0] + item_w
                matrix[w // step][j][1] = matrix[item_w//step][j - 1][1] + int(lst[j]['p'])
        w += step
    for i in range(step + 1):
        for j in range(len(lst)):
            print(matrix[i][j], end='')
        print()

file_name = 'input5.csv'
max_w = 100
step = 10
get_answer(file_name, max_w, step)
