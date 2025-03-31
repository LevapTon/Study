# Градиентный спуск для квадратичной ошибки

gradient_descent_mse <- function(x, y, rate = 0.0001, tol = 1e-6, max_steps = 1000000) {
  
  # Начальные параметры
  # Смещение
  
  w0 <- 0  
  
  # Наклон
  
  w1 <- 0  
  
  # Количество наблюдений
  
  n <- length(y)  
  iter <- 0
  
  repeat {
    
    # Счетчик итераций
    iter <- iter + 1
    
    
    # Вычисление предсказанных значений
    
    y_pred <- w0 + w1 * x
    
    # Вычисление градиентов
    
    grad_w0 <- -2 / n * sum(y - y_pred)
    grad_w1 <- -2 / n * sum((y - y_pred) * x)
    
    # Обновление параметров
    
    w0_new <- w0 - rate * grad_w0
    w1_new <- w1 - rate * grad_w1
    
    # Проверка условия сходимости
    
    if (sqrt((w0_new - w0)^2 + (w1_new - w1)^2) < tol || iter >= max_steps) {
      break
    }
    
    # Присвоение новых значений
    
    w0 <- w0_new
    w1 <- w1_new
  }
  
  # Результаты
  
  list(
    intercept = w0,
    slope = w1,
    steps = iter
  )
}

# Данные

alligators <- read.table("./alligators.txt", header = TRUE)
x <- alligators$Length
y <- alligators$Weight

# Решение методом наименьших квадратов с помощью lm

model <- lm(Weight ~ Length, data = alligators)
lm_intercept <- coef(model)[1]
lm_slope <- coef(model)[2]

# Решение методом градиентного спуска

gd_result <- gradient_descent_mse(x, y)

# Построение графика

plot(x, y, pch = 1, col = "black", 
     xlab = "Длина", ylab = "Вес", 
     main = "Зависимость веса аллигатора от его длины")

# Добавление линии регрессии из модели lm

abline(lm_intercept, lm_slope, col = "sienna2", lwd = 2, lty = 1)

# Добавление линии регрессии из градиентного спуска

abline(gd_result$intercept, gd_result$slope, col = "green", lwd = 2, lty = 2)

# Легенда

legend("topleft", legend = c("lm", "Градиентный спуск"),
       col = c("sienna2", "green"), lty = c(1, 2), lwd = 2)

# Функция для вычисления среднеквадротичной ошибки

compute_mse <- function(x, y, intercept, slope) {
  y_pred <- intercept + slope * x
  mean((y - y_pred)^2)
}

# Вычисление среднеквадротичной ошибки для обеих моделей

lm_mse <- compute_mse(x, y, lm_intercept, lm_slope)
gd_mse <- compute_mse(x, y, gd_result$intercept, gd_result$slope)

# Вывод результатов 

cat("Результаты линейной регрессии:\n")
cat("Метод наименьших квадратов (lm):\n")
cat(sprintf("  Смещение (intercept): %.4f\n", lm_intercept))
cat(sprintf("  Наклон (slope): %.4f\n", lm_slope))
cat(sprintf("  Среднеквадратичная ошибка (MSE): %.4f\n", lm_mse))

cat("\nМетод градиентного спуска:\n")
cat(sprintf("  Смещение (intercept): %.4f\n", gd_result$intercept))
cat(sprintf("  Наклон (slope): %.4f\n", gd_result$slope))
cat(sprintf("  Среднеквадратичная ошибка (MSE): %.4f\n", gd_mse))
cat(sprintf("  Количество шагов до сходимости: %d\n", gd_result$steps))

