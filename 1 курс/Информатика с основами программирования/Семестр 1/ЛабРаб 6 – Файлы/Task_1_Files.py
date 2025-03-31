def sum_from_txt():
    'Сумма чисел из блокнота'
    with open('файлы\\Numbers_1.txt') as file:
        res = 0
        for line in file:
            res += sum(map(int, line.split()))
    print(res)

sum_from_txt()