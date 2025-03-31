# Подключение библиотеки

library(ggplot2)

# Загрузка данных для обучения

svmdata <- read.table("./svmdata1.txt", header = TRUE)

# Загрузка тестовых данных

svmdata_test <- read.table("./svmdata1test.txt", header = TRUE)

# Замена класса "red" на -1, а "green" на 1 в данных

svmdata$Color <- ifelse(svmdata$Color == "red", -1, ifelse(svmdata$Color == "green", 1, NA))
svmdata_test$Color <- ifelse(svmdata_test$Color == "red", -1, ifelse(svmdata_test$Color == "green", 1, NA))

# Функция вычисления экспоненты

exponent_func <- function(w, y, x1, x2) {
  exp(-y * (w[1] + w[2] * x1 + w[3] * x2))
}

# Функция ошибки

error_func <- function(w, y, x1, x2, l) {
  (1/l)*sum(log(1 + exponent_func(w, y, x1, x2)))
}

# Градиент

gradient <- function(w, y, x1, x2, l) {
  exp_vals <- exponent_func(w, y, x1, x2)
  df_w0 <- (1/l)*sum((-y * exp_vals) / (1 + exp_vals))
  df_w1 <- (1/l)*sum((-y * x1 * exp_vals) / (1 + exp_vals))
  df_w2 <- (1/l)*sum((-y * x2 * exp_vals) / (1 + exp_vals))
  c(df_w0, df_w1, df_w2)
}

# Градиентный спуск

gradient_descent <- function(start, dataset, rate = 0.001, tol = 1e-6, max_steps = 4000000) {
  w <- start
  l <- nrow(dataset)
  iter <- 0
  
  repeat {
    iter <- iter + 1
    grad <- gradient(w, dataset$Color, dataset$X1, dataset$X2, l)
    w_new <- w - rate * grad
    if (sqrt(sum((w_new - w)^2)) < tol || iter >= max_steps) {
      w <- w_new
      break
    }
    w <- w_new
  }
  
  list(
    w = w,
    steps = iter,
    error = round(error_func(w, dataset$Color, dataset$X1, dataset$X2, l), 4)
  )
}

# Запуск градиентного спуска для обучения на svmdata

start_point <- c(0, 0, 0)
result <- gradient_descent(start_point, svmdata)

# Подсчет маржи для каждой точки в обучающих данных

svmdata$margin <- svmdata$Color * (result$w[1] + result$w[2] * svmdata$X1 + result$w[3] * svmdata$X2)

# Определение опорных векторов

support_vectors <- svmdata[abs(svmdata$margin - 1) < 3.745, ]

# Создаение сетки значений для X1 и X2 для визуализации

x1_range <- seq(min(svmdata$X1) - 1, max(svmdata$X1) + 1, length.out = 200)
x2_range <- seq(min(svmdata$X2) - 1, max(svmdata$X2) + 1, length.out = 200)
grid <- expand.grid(X1 = x1_range, X2 = x2_range)

# Вычисление предсказанных классов для сетки

grid$Prediction <- ifelse(result$w[1] + result$w[2] * grid$X1 + result$w[3] * grid$X2 > 0, 1, -1)

# Построение графика

ggplot() +
  geom_tile(data = grid, aes(x = X1, y = X2, fill = factor(Prediction)), alpha = 0.3) +
  scale_fill_manual(values = c("-1" = "red", "1" = "green"), name = "Класс") +
  geom_point(data = svmdata_test, aes(x = X1, y = X2, color = factor(Color)), size = 3) +
  scale_color_manual(values = c("-1" = "red", "1" = "green"), name = "Истинный класс (тест)") +
  geom_abline(intercept = -result$w[1] / result$w[3], slope = -result$w[2] / result$w[3], color = "blue", size = 1, linetype = "dashed") +
  labs(title = "SVM с линейной функцией ошибки", x = "X1", y = "X2") +
  theme_minimal()

cat("w: ", result$w, "\nКол-во шагов: ", result$steps, "\nФункция ошибки", result$error, "\n")
cat("Количество опорных векторов:", nrow(support_vectors), "\n")
