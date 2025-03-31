# Загрузка набора данных swiss

data(swiss)

# Стандартизация данных

swiss_scaled <- scale(swiss)

# Вычисление главных компонент

pca_model <- prcomp(swiss_scaled, scale. = TRUE)

# Процент объяснённой дисперсии

explained_variance <- cumsum(pca_model$sdev^2) / sum(pca_model$sdev^2)

# Минимальное число компонент, объясняющих >90% изменчивости

min_components <- which(explained_variance > 0.9)[1]
cat("Минимальное число компонент:", min_components, "\n")

# Построение графика объяснённой дисперсии

fviz_eig(pca_model, addlabels = TRUE, xlab = "Признаки", 
         main = "Диаграма зависимости компонент",
         ylab = "Процент объяснения изменчивости")


# Добавление значений компонент в исходный набор данных

principal_components <- as.data.frame(pca_model$x[, 1:min_components])
colnames(principal_components) <- paste0("PC", 1:min_components)
swiss_with_pcs <- cbind(swiss, principal_components)

