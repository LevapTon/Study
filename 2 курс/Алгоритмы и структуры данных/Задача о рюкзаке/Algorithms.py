def greedy_price(lst, max_w=100):
    s = 0
    i = 0
    answ = []
    lst.sort(key=lambda x: int(x['price']), reverse=True)
    while True:
        if (s + int(lst[i]['weight']) < max_w): 
            s += int(lst[i]['weight'])
            answ.append(lst[i])
            i += 1
        else: break
    return answ


def greedy_relative_price(lst, max_w=100):
    s = 0
    i = 0
    answ = []
    lst.sort(key=lambda x: int(x['price'])/int(x['weight']), reverse=True)
    while True:
        if (s + int(lst[i]['weight']) < max_w): 
            s += int(lst[i]['weight'])
            answ.append(lst[i])
            i += 1
        else: break
    return answ


def bin_mask(lst, max_w=100):
    combs = []
    n = len(lst)
    for i in range(1 << n):
        s = 0
        combo = []
        for pos in range(n):
            mask = 1 << pos
            if bool(i & mask):
                combo += [lst[pos]]
                s += int(lst[pos]['weight'])
        if s < max_w: combs += [combo]
    return find_max(combs)


def rec_search(lst, max_w=100):  # переделать!!!
    def rec(combo, deep):
        if deep == len(lst):  # точка останова
            if (is_not_out_of_weight(combo, max_w)):
                combs.append(combo)
        else:  # шаги рекурсии
            rec(combo+[lst[deep]], deep+1)
            rec(combo, deep+1)
    combs = []
    rec([], 0)
    return find_max(combs)


def rec_search_deep(lst, max_w=100):  # переделать!!!
    def rec(combo, deep):
        if deep == len(lst):  # точка останова
            if (is_not_out_of_weight(combo, max_w)):
                combs.append(combo)
        else:  # шаги рекурсии
            rec(combo+[lst[deep]], deep+1)
            rec(combo, deep+1)
    combs = []
    rec([], 0)
    return combs[0]


def is_not_out_of_weight(lst, max_w=100):
    sum_w = 0
    for i in range(len(lst)):
        sum_w = sum_w + int(lst[i]['weight'])
    if sum_w <= max_w:
        return True
    return False


def find_max(lst):
    max_p = 0
    answ = []
    for item in lst:
        cur_p = 0
        for pos in range(len(item)):
            cur_p += int(item[pos]['price'])
        if cur_p > max_p:
            max_p = cur_p
            answ = item
    return answ
    