from ReadCsv import get_lst_dct
from Algorithms import greedy_price, greedy_relative_price, bin_mask, rec_search, rec_search_deep


def elem_sum(lst):
    sum_p = 0
    for i in range(len(lst)):
        sum_p = sum_p + int(lst[i]['price'])
    return sum_p


def find_answ(lst, max_w):
    answ = greedy_price(lst, max_w)
    print("Жадный:\n",answ,'\n',"Сумма:", elem_sum(answ),'\n')
    answ = greedy_relative_price(lst, max_w)
    print("Жадный относ.:\n",answ,'\n',"Сумма:", elem_sum(answ),'\n')
    answ = bin_mask(lst, max_w)
    print("Бин. маски:\n",answ,'\n',"Сумма:", elem_sum(answ),'\n')
    answ = rec_search(lst, max_w)
    print("Рекурсия:\n",answ,'\n',"Сумма:", elem_sum(answ),'\n')
    answ = rec_search_deep(lst, max_w)
    print("Поиск в глубину:\n",answ,'\n',"Сумма:", elem_sum(answ),'\n')


lst = get_lst_dct('./Conteiner.csv')
max_w = 100
answ = []
find_answ(lst, max_w)
