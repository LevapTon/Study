# Функция для генрации выборки
get_random_vektor <- function() {
  # Получение случайного вектора
  vektor = c(sample(1:9, 1), sample(1:9, 1), sample(1:9, 1))
}

# Получем выборку по каждому дню
day_1 = get_random_vektor()
day_2 = get_random_vektor()
day_3 == get_random_vektor()
# Выводим полученные векторы
day_1
day_2
day_3

# Создаем фрейм из полученных векторов
days_data <- data.frame(day_1, day_2, day_3)
