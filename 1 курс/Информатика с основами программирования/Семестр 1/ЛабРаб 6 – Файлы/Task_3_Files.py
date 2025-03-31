from operator import itemgetter

def sort_file(num):
    'Сортировка по столбцу'
    with open ('файлы\\Numbers_3.txt') as file:
        number = file.readlines()
        res = []
        x = ''
        for line in number:
            tpl = line.strip().split()
            res.append(tpl)
        for i in range(4):
            for j in range(6):
                res[i][j] = int(res[i][j])
        print(res, type(res[0][0]))
        with open('файлы\\Numbers_Sorted.txt', 'w') as result:
            for obj in sorted(res, key=itemgetter(num - 1), reverse=True):
                for i in range(len(obj)):
                    x += ''.join(str(obj[i]))
                print(x)
                result.write(str(obj) + '\n')
                x = ''


def front_end():
    'Ввод номера столбца'
    num = int(input('Введите номер столбца: '))
    sort_file(num)

front_end()