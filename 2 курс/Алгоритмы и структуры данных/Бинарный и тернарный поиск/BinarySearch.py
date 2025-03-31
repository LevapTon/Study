from graph import func


def binary_search(x1, x2, count=1):
    mid = (x1 + x2) / 2
    val1 = func(mid - accuarcy)
    val2 = func(mid + accuarcy)
    mid_val = func(mid)
    if (val1 < mid_val and val2 < mid_val):
        print("Бинарный поиск. Кол-во выз-в:", count)
        return mid
    if (val1 > mid_val):
        count = count + 1
        res = binary_search(x1, mid - accuarcy, count)
    elif (val2 > mid_val):
        count = count + 1
        res = binary_search(mid + accuarcy, x2, count)
    return res


accuarcy = .0001
