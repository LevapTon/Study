from timeit import default_timer as t
from random import randint as r


def create_heap(lst):
    heap = []
    for item in lst:
        heap.append(item)
        pos = len(heap) - 1
        while pos > 0:
            pos_par = (pos - 1) // 2
            if heap[pos_par] < heap[pos]:
                heap[pos_par], heap[pos] = heap[pos], heap[pos_par]
            else: break
            pos = pos_par
    return heap


def sorting(heap):
    for i in range(len(heap) - 1, 0, -1):  # от последнего до нулевого с шагом -1
        heap[0], heap[i] = heap[i], heap[0]
        par = 0
        while par < i:
            child_1 = par * 2 + 1
            child_2 = par * 2 + 2
            if child_2 < i and heap[child_2] > heap[par] and heap[child_2] > heap[child_1]:
                heap[par], heap[child_2] = heap[child_2], heap[par]
                par = child_2
            elif child_1 < i and heap[child_1] > heap[par]:
                heap[par], heap[child_1] = heap[child_1], heap[par]
                par = child_1
            else: break  # если замен не было – выйти из цикла
    return heap


def sort_heap(lst):
    heap = create_heap(lst)
    print(heap[:10])
    return sorting(heap)


count = 2000000
lst = [r(0, 1000000) for _ in range(count)]
print(lst[:10])

start = t()

res = sort_heap(lst)

print(round(t() - start, 3))
print(res[:10])
