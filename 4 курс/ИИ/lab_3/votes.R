# Чтение данных

library("dplyr")

votes = read.table("./votes.repub.txt", header = TRUE)

# Избавляемся от NA

votes[is.na(votes)] = 0

# Заменим номера строк названиями шататов

rownames(votes) = votes$States

# Делаем срез, т.к. для дендрограммы важны голоса, а не штаты

votes = votes[,2:32]

# Получаем расстояния между точками

votes_d = dist(scale(votes))^2

# Получаем кластеризацию

votes_h = hclust(votes_d, method = "ward.D")

plot(votes_h, main = "Дендрограма votes.repub", xlab = "Штаты", ylab = "Расстояние")

# Выделим на графике 3 кластера

rect.hclust(votes_h, k = 3, border="red") 
