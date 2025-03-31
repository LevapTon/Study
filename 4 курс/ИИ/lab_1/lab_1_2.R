# Загрузка данных

cygage <- read.table("./cygage.txt", header = TRUE)

# Извлечение переменных

x <- cygage$Depth    
y <- cygage$calAge       
w <- cygage$Weight     

# Взвешенная линейная регрессия с использованием lm

lm_model <- lm(y ~ x, weights = w)
lm_intercept <- coef(lm_model)[1]
lm_slope <- coef(lm_model)[2]

# Вычисление MSE для lm

compute_mse <- function(x, y, w, intercept, slope) {
  y_pred <- intercept + slope * x
  mean(w * (y - y_pred)^2)
}
lm_mse <- compute_mse(x, y, w, lm_intercept, lm_slope)

# Градиентный спуск для взвешенной регрессии

weighted_gradient_descent <- function(x, y, w, start, rate = 0.000001, tol = 1e-4, max_steps = 10000000) {
  intercept <- start[1]
  slope <- start[2]
  iter <- 0
  
  repeat {
    iter <- iter + 1
    
    # Градиенты по смещению и наклону
    
    y_pred <- intercept + slope * x
    grad_intercept <- -2 * sum(w * (y - y_pred))
    grad_slope <- -2 * sum(w * (y - y_pred) * x)
    
    # Обновление параметров
    
    intercept_new <- intercept - rate * grad_intercept
    slope_new <- slope - rate * grad_slope
    
    # Проверка условия сходимости
    
    if (sqrt((intercept_new - intercept)^2 + (slope_new - slope)^2) < tol || iter >= max_steps) {
      break
    }
    
    intercept <- intercept_new
    slope <- slope_new
  }
  
  # Возврат результата
  
  list(
    intercept = intercept,
    slope = slope,
    steps = iter
  )
}

# Начальная точка и выполнение градиентного спуска

start_point <- c(0, 0)
gd_result <- weighted_gradient_descent(x, y, w, start_point)

# Вычисление MSE для градиентного спуска

gd_mse <- compute_mse(x, y, w, gd_result$intercept, gd_result$slope)

# Построение графика

plot(x, y, pch = 1, col = "black", main = "Регрессия возраста отложений", xlab = "Глубина", ylab = "Возраст")
abline(lm_intercept, lm_slope, col = "blue", lwd = 2, lty = 2)
abline(gd_result$intercept, gd_result$slope, col = "red", lwd = 2, lty = 3)
legend("topleft", legend = c("Метод lm", "Градиентный спуск"), col = c("blue", "red"), lty = c(1, 2), lwd = 2)

# Вывод результатов

cat("Результаты взвешенной регрессии:\n")
cat("Метод наименьших квадратов (lm):\n")
cat(sprintf("  Смещение (intercept): %.4f\n", lm_intercept))
cat(sprintf("  Наклон (slope): %.4f\n", lm_slope))
cat(sprintf("  Среднеквадратичная ошибка (MSE): %.4f\n", lm_mse))

cat("\nМетод градиентного спуска:\n")
cat(sprintf("  Смещение (intercept): %.4f\n", gd_result$intercept))
cat(sprintf("  Наклон (slope): %.4f\n", gd_result$slope))
cat(sprintf("  Среднеквадратичная ошибка (MSE): %.4f\n", gd_mse))
cat(sprintf("  Количество шагов до сходимости: %d\n", gd_result$steps))
