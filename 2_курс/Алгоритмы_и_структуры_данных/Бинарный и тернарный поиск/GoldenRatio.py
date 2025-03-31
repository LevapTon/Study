from graph import func


def golden_ratio(x1, x2, count=1):
    left = x2 - (x2 - x1) / 1.618
    right = x1 + (x2 - x1) / 1.618
    if abs(func(left) - func(right)) < 0.0001:
        print("Золотое сечение. Кол-во выз-в:", count)
        return (left+right)/2
    if func(left) <= func(right):
        count = count + 1
        res = golden_ratio(left, x2, count)
    elif func(left) > func(right):
        count = count + 1
        res = golden_ratio(x1, right, count)
    return res
