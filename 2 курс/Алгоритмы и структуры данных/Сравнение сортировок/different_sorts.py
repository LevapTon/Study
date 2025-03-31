def sort_bubble(lst):
    '''Сортировка пузырьком'''
    n = len(lst)
    for j in range(1, n):
        for i in range(n-j):
            if lst[i] > lst[i+1]:
                lst[i], lst[i+1] = lst[i+1], lst[i]
    return lst


def sort_bubble_plus(lst):
    ''' усовершенствованная сортировка пузырьком '''
    n = len(lst)
    for j in range(1, n):
        swap = False  # была ли замена элементов
        for i in range(n-j):
            if lst[i] > lst[i+1]:
                lst[i], lst[i+1] = lst[i+1], lst[i]
                swap = True
        if not swap:  # всё уже отсортировано
            break
    return lst


def insert_sort(lst):
    '''Сортировка вставкой'''
    for i in range(1, (len(lst))):
        tmp = lst[i]
        while (i > 0) and (tmp <= lst[i - 1]):
            lst[i] = lst[i - 1] 
            i -= 1
        else:
            lst[i] = tmp
    return lst


def sort_choices(lst):
    '''Сортировка выбором по возрастанию''' 
    for i in range(len(lst), 1, -1):
        max = -10000
        for j in range(i):
            if lst[j] >= max: 
                max, index = lst[j], j
        lst[i - 1], lst[index] = lst[index], lst[i - 1]
    return lst
        

def sort_shaker(lst):
    '''Сортировка шейкером'''
    index = 0  # Нижняя граница списка
    for i in range(len(lst), 1, -1):  # Цикл с уменьшением верхней границы
        for j in range(index, i):  # "Всплытие" наибольшего
            if lst[j] > lst[i-1]:
                lst[i-1], lst[j] = lst[j], lst[i-1]
        for k in range(i - 1, index, -1):  # "Утопление" наименьшего
            if lst[k] < lst[index]:
                lst[index], lst[k] = lst[k], lst[index]
        index += 1  # Увеличение нижней границы
    return lst
