from graph import func


def ternary_search(x1, x2, count=1):
    left = x1 + (x2 - x1) / 3
    right = x2 - (x2 - x1) / 3
    if abs(func(left) - func(right)) < 0.0001:
        print("Тернарный поиск. Кол-во выз-в:", count)
        return (left+right)/2
    if func(left) < func(right):
        count = count + 1
        res = ternary_search(left, x2, count)
    elif func(left) > func(right):
        count = count + 1
        res = ternary_search(x1, right, count)
    elif func(left) == func(right):
        count = count + 1
        res = ternary_search(left, right, count)
    return res
