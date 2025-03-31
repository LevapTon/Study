'''
сортировки слиянием, быстрая, пирамидальная
'''


def merge(a, b):
    '''
    функция слияния двух отсортированных списков
    возвращает общий отсортированный список
    переписать функцию без рекурсии
    циклом, сложность равна n
    '''
    if a == [] or b == []: return a + b
    if a[0] < b[0]:
        return [a[0]] + merge(a[1:], b)
    else:
        return [b[0]] + merge(a, b[1:])


def merge_fix(a, b):
    '''
    исправленная функция
    '''
    # в данной функции необходимо слить два отсортированных списка
    # т.к. списки отсортированны по возрастанию
    # то каждый последующий элемент списка гарантированно больше предыдущего
    index_a = 0  # индекс элементов списка a
    index_b = 0  # индеекс элементов списка b
    res = []  # пустой список для, в котором будет конечный результат
    while True:
        # сперва необходимо делать проверку на
        # выход индекса за пределы индексации списка
        if index_a + 1 > len(a):  # индексация элементов идет с нуля, но длина списка с единицы
            res += b[index_b:]  # т.к. список a закончился, то нужно добавить остаток списка b
            break
        elif index_b + 1 > len(b):  # тоже самое, только с b
            res += a[index_a:]
            break
        # далее простое сравнение элементов списков
        if a[index_a] < b[index_b]:
            res += a[index_a]  # если больше b[i], то добавить к результату a[i]
            index_a += 1  # после увеличиваем индекс а, т.к. элемент оттуда уже взят
        else:
            res += b[index_b]  # обратно, если a[i] больше
            index_b += 1
    return res


def sort_merge(lst):
    if len(lst) < 2: return lst
    middle = len(lst) // 2
    left = sort_merge(lst[:middle])
    right = sort_merge(lst[middle:])
    return merge_fix(left, right)


def sort_quick(lst):
    '''
    переписать эту функцию так
    чтобы не было три цикла
    за один цикл выбрать меньшие, большие и равные pivot
    '''
    if len(lst) < 2: return lst
    pivot = lst[0]
    left = list(filter(lambda x: x < pivot, lst))
    middle = list(filter(lambda x: x == pivot, lst))
    right = list(filter(lambda x: x > pivot, lst))
    return sort_quick(left) + middle + sort_quick(right)


def sort_quick_fix(lst):
    '''
    исправленная функция
    '''
    if len(lst) < 2: return lst  # если список из 1 или 0 элементов, то он уже отсортирован
    pivot = lst[len(lst) // 2]  # берем элемент, относительно которого будем разбивать список
    left = []  # список элементов меньше pivot
    right = []  # список элементов больше pivot
    middle = []  # список элементов, которые равны pivot
    for i in range(len(lst)):  # цикл по длине списка
        if lst[i] < pivot:  # если элемент меньше pivot
            left += [lst[i]]  # то добавляем его в список меньших элементов
        elif lst[i] > pivot:  # если элемент больше pivot
            right += [lst[i]]  # то добавляем его в список меньших элементов
        else: 
            middle += [lst[i]]  # если элемент равен pivot, то добавляем в список равных элементов
    return sort_quick_fix(left) + middle + sort_quick_fix(right)  # рекурсивный вызов для
    # меньших элементов + равных(без рекурсии, т.к. сортировать нечего) + больших элементов()
