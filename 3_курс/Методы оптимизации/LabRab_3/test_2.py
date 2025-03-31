import copy


def simplex(c, b, a, o, f, basis):
    steps = 0
    count = 0 # Для подсчета не отрицательных оценок замещения
    notComplete = True # Для остановки цикла
    while notComplete:
        print("–"*20)
        for i in range(len(o)): # Пересчитываем оценки замещения
            o[i] = recaclO([c[basis[0]-1], c[basis[1]-1], c[basis[2]-1]], c[i], [a[0][i], a[1][i], a[2][i]])

        print("Значения оценок замещения:")
        for elem in o:
            print("%5.2f" % elem, end=" ")
        print()

        f = recaclF(b, [c[basis[0]-1], c[basis[1]-1], c[basis[2]-1]]) # Считаем значение функции
        print("Значение функции %5.2f" % f)

        for i in range(len(o)): # Просто проверка окончания вычислений по оценкам замещения
            if o[i] >= 0: count += 1
            else:
                count = 0
                break
            if count == 6: notComplete = not notComplete

        if not notComplete: break # Выход из цикла

        minO = abs(o[i])*100 # Просто чтобы был хоть какой-то минимум для сравнения

        for i in range(len(o)): # Поиск минимальной положительной оценки
            if o[i] < 0 and o[i] < minO:
                minO = o[i]

        # Берем индекс минимальной оценки, является j для ключевого элемента
        key = o.index(minO) 

        # Считаем минимальный q и берем его индекс как i для ключевого предмета
        q = getMinPlus(b, [a[0][key], a[1][key], a[2][key]])

        # Меняем базисные переменные
        basis[q] = key + 1
        print("Новый базис:\n", basis)

        # Пересчитываем значения b
        b = recalcB(b, [a[0][key], a[1][key], a[2][key]], q)
        print("Новые значения b:")
        for elem in b:
            print("%5.2f" % elem, end=" ")
        print()

        # Пересчитываем значения a
        a = recalcA(a, q, key)
        print("Новые значения a:")
        for i in range(len(a)):
            for j in range(len(a[i])):
                print("%5.2f" % a[i][j], end=" ")
            print()
        steps += 1
    print("–"*20)
    print("Количество итераций\n", steps)
    return [b, basis, f]


def recalcA(a, keyI, keyJ):
    '''
    Пересчет значений a.

    a – list – список значений a;

    keyI – int – нидекс ключевого элемента по i;

    keyJ – int – индекс ключевого элемента по j.
    '''
    x = a[keyI][keyJ]
    origA = copy.deepcopy(a)
    for i in range(len(a)):
        for j in range(len(a[i])):
            if i == keyI:
                a[i][j] = origA[i][j] / x
            else:
                a[i][j] = origA[i][j] - (origA[keyI][j] * origA[i][keyJ] / x)
    return a


def recalcB(b, col, key):
    '''
    Пересчет значений b.

    b – list – список значений b;

    col – list – столбец значений под новым базисом;

    key – int – индекс ключевого элемента.
    '''
    x = col[key]
    origB = copy.deepcopy(b)
    for i in range(len(b)):
        if i == key:
            b[i] = origB[i] / x
        else:
            b[i] = origB[i] - (col[i] * origB[key] / x)
    return b


def recaclF(b, c):
    '''
    Расчет значений функции.

    b – list – список значений b;

    c – list – список коэффициентов базисных x в функции.
    '''
    return c[0]*b[0] + c[1]*b[1] + c[2]*b[2]


def recaclO(o, c, col):
    '''
    Пересчет оценок замещения.

    o – list – коэффициенты при базисных переменных в функции;

    c – float – коэффициент при x;

    col – list – список значений a[i][j] под столбцом x.
    '''
    return (o[0]*col[0] + o[1]*col[1] + o[2]*col[2]) - c


def getMinPlus(b, col):
    '''
    Расчет q и взятие его индекса.

    b – list – список значений b;

    col – list – столбец значений под новым базисом.
    '''
    res = [b[0]/col[0], b[1]/col[1], b[2]/col[2]]

    print("Новые значения q:")
    for elem in res:
        print("%5.2f" % elem, end=" ")
    print()
    
    min = abs(res[0]*100)
    for i in range(len(res)):
        if res[i] < 0: res[i] = abs(res[i]*100)
        if res[i] < min: min = res[i]
    return res.index(min)


c = [1, -2, 0, 0, 0, -100] # Коэффициенты при переменных в функции
b = [9, 27, 14] # Правая часть ограничений
a = [[-3, 2, 1, 0, 0, 0], # Коэффициенты при переменных в ограничениях
     [3, 4, 0, -1, 0, 1],
     [2, 1, 0, 0, 1, 0]]
o = [0, 0, 0, 0, 0, 0] # Оценки замещения
f = 0 # Значение функции
basis = [3, 6, 5] # Начальный базис
answ = simplex(c, b, a, o, f, basis)
print("Номера базисных переменных\n", answ[1])
print("Значения базисных переменных при соответствующем индексе")
for elem in answ[0]:
    print("%5.2f" % elem, end=" ")
print()
print("Значение функции\n", answ[2])
