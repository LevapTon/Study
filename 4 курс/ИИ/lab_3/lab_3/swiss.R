
library(factoextra)

swiss = read.table("./swiss.txt", header = TRUE, sep = ",", row.names = 1)

swiss_pca = prcomp(swiss)

fviz_eig(swiss_pca, addlabels = TRUE, xlab = "Признаки", 
         main = "Диаграма зависимости компонент", ylab = "Процент объяснения изменчивости")
