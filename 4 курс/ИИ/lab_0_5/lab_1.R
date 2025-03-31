# Определение функции

f <- function(x1, x2) {
  85 * x1^2 +168 * x1 * x2 + 85 * x2^2 + 29 * x1 - 51 * x2 + 83
}

gradient <- function(x1, x2) {
  
  # Производная по x1
  
  df_x1 <- 170 * x1 + 168 * x2 + 29
  
  # Производная по x2
  
  df_x2 <- 168 * x1 + 170 * x2 - 51
  c(df_x1, df_x2)
}

# Поиск точки экстремума 

gradient_descent <- function(start, rate = 0.001, tol = 1e-6, max_steps = 6000) {
  
  # Начальные значения
  
  x1 <- start[1]
  x2 <- start[2]
  iter <- 0
  
  repeat {
    
    # Счетчик проделанных шагов
    
    iter <- iter + 1
    
    # Получение градиента
    
    grad <- gradient(x1, x2)
    
    # Получение следующих точек
    
    x1_new <- x1 - rate * grad[1]
    x2_new <- x2 - rate * grad[2]
    
    # Проверка условия сходимости
    if (sqrt((x1_new - x1)^2 + (x2_new - x2)^2) < tol || iter >= max_steps) {
      break
    }
    
    # Присваивание новых значений
    
    x1 <- x1_new
    x2 <- x2_new
  }
  
  # Список с точкой экстремума, его значением и кол-вом шагов
  
  list(
    minimum_x1 = round(x1, 2),
    minimum_x2 = round(x2, 2),
    extremum = round(f(x1, x2), 2),
    steps = iter
  )
}

# Стартовая точка

start_point <- c(0, 0)

result <- gradient_descent(start_point)

result
