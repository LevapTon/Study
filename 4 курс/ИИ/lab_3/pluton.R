# Подключение библиотеки визуализации

library(factoextra)

# Чтение данных

pluton = read.table("./pluton.txt", header = TRUE)

# Кол-во кластеров

k = 3

# Кол-во итераций
# При iters < 6 – нечеткое разделение
# При iters >= 6 – ровное разделение на 3 кластера

iters = 6

# Кластеризация методом k-средних

clusters = kmeans(pluton, k, nstart = iters)

# Отображение результата кластеризации на исходном датасете

fviz_cluster(clusters, pluton, main = "Кластеризация плутона",
             xlab = FALSE, ylab = FALSE)