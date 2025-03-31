from cProfile import label
from turtle import color, title
import numpy as np
import matplotlib.pyplot as plt


def func(list_x=0.):
    return np.sin(list_x)*np.log2(list_x)


def find_ext():
    x = np.linspace(2.*np.pi, 3.*np.pi, 100)
    max_y = func(x[0])
    mx = x[0]
    for i in range(1, len(x)):
        if func(x[i]) > max_y:
            max_y = func(x[i])
            mx = x[i]
    return mx


np.seterr(all='ignore')
fig, axes = plt.subplots()
axes.grid(True, linestyle='-.')
axes.tick_params(labelcolor='b', labelsize='medium', width=3)
axes.set_title("Функция: y = sin(x) * log(x;2)")
lst_x = np.linspace(0.*np.pi, 3.*np.pi, 100)
plt.plot(lst_x, func(lst_x), color="red", label="sin(x) * log(x;2)")
ext_x = find_ext()
plt.plot(ext_x, func(ext_x), marker="x", color="green")
plt.show()
