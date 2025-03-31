import different_sorts as prog
import random
from time import monotonic


def get_list():
    '''генерация списка элементов'''
    lst = list(range(4001))
    random.shuffle(lst)
    print(lst[0:10], "\n")
    timer(lst)


def timer(lst):
    '''Замер результатов работы'''

    start = monotonic()
    lst = prog.insert_sort(lst)
    finish = monotonic()
    print(lst[:10])
    print("Сортировка вставкой:", round(finish - start, 3), "\n")

    random.shuffle(lst)
    start = monotonic()
    lst = prog.sort_choices(lst)
    finish = monotonic()
    print(lst[:10])
    print("Сортировкой выбором:", round(finish - start, 3), "\n") 

    random.shuffle(lst)
    start = monotonic()
    lst = prog.sort_bubble(lst)
    finish = monotonic()
    print(lst[:10])
    print("Сортировка пузырьком:", round(finish - start, 3), "\n") 

    random.shuffle(lst)
    start = monotonic()
    lst = prog.sort_bubble_plus(lst)
    finish = monotonic()
    print(lst[:10])
    print("Сортировка пузырьком+:", round(finish - start, 3), "\n")

    random.shuffle(lst)
    start = monotonic()
    lst = prog.sort_shaker(lst)
    finish = monotonic()
    print(lst[:10])
    print("Сортировка шейкером:", round(finish - start, 3), "\n") 


get_list()