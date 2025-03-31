from cProfile import label
from turtle import color, title
import matplotlib.pyplot as plt


merge = [0.828, 3.385, 4.441, 6.871, 9.56, 21.07]
quick = [0.667, 1.803, 4.041, 6.188, 8.481, 17.343]
heap = [1.036, 2.683, 5.802, 9.112, 12.853, 28.023]
scale_y = [100000, 250000, 500000, 750000, 1000000, 2000000]

fig, axes = plt.subplots()
axes.grid(True, linestyle='-.')
axes.tick_params(labelcolor='b', labelsize='medium', width=3)
axes.set_title("Сравнение функций")
clr_labels = ['red', 'green', 'orange']

plt.plot(merge, scale_y, color=clr_labels[0], label="Слияние")
plt.plot(quick, scale_y, color=clr_labels[1], label="Быстрая")
plt.plot(heap, scale_y, color=clr_labels[2], label="Куча")
plt.legend(title="Сортировки")

plt.show()