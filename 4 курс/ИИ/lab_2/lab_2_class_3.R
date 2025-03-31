# Загрузка необходимых пакетов

library(e1071)
library(ggplot2)

# Загрузка данных из файлов

train_data <- read.table("./svmdata1.txt", header = TRUE)
test_data <- read.table("./svmdata1test.txt", header = TRUE)

# Преобразуем метки классов в фактор

train_data$Color <- as.factor(train_data$Color)
test_data$Color <- as.factor(test_data$Color)

# Построение модели SVM с ядром "linear" и C=1

svm_model <- svm(Color ~ X1 + X2, data = train_data, kernel = "linear", cost = 1)

# Создание сетки для предсказания

grid <- expand.grid(X1 = seq(min(train_data$X1), max(train_data$X1), length = 200),
                    X2 = seq(min(train_data$X2), max(train_data$X2), length = 200))

# Прогнозирование на сетке

predictions <- predict(svm_model, newdata = grid)

# Создание графика с использованием ggplot2

plot <- ggplot(train_data, aes(x = X1, y = X2)) +
  geom_point(aes(color = Color), size = 3) + 
  geom_raster(data = grid, aes(x = X1, y = X2, fill = predictions), interpolate = TRUE, alpha = 0.3) +
  scale_fill_manual(values = c("red" = "red", "green" = "green"), name = "Класс по решению SVM") +
  scale_color_manual(values = c("red" = "grey", "green" = "black"), name = "Цвет точек") +  # Цвета точек отличны от классов
  theme_minimal() +
  labs(title = "SVM с линейным ядром",
       x = "X1", y = "X2")

print(plot)

cat("Количество опорных векторов:", nrow(svm_model$SV), "\n")
