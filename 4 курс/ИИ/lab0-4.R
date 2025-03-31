# Функция для подсчета мужчин и женщин
get_count_of_pass <- function(peoples) {
  male_data = subset(peoples, Sex =="male")
  female_data = subset(peoples, Sex =="female")
  male_count = nrow(male_data)
  female_count = nrow(female_data)
  result = c(male_count, female_count)
  result
}

# Функция для подсчета доли выживших
get_percent_of_surv <- function(survivors) {
  survival_data = subset(survivors, Survived == 1)
  all_count = nrow(survivors)
  alive_count = nrow(survival_data)
  result = round(alive_count / all_count, 2)
  result
}

# Функция для подсчета доли пассажиров первого класса
get_percent_of_f_class <- function(classes) {
  f_class_data = subset(classes, Pclass == 1)
  all_count = nrow(classes)
  first_count = nrow(f_class_data)
  result = round(first_count / all_count, 2)
  result
}

# Функция для подсчета среднего возраста и медианы
get_mid_age_of_pass <- function(ages) {
  ages_data = na.omit(ages)
  mid_ages = mean(ages_data[1])
  mid_ages
}

# Получение данных csv-файла
titanic_data <- data.frame(read.csv("./titanic.csv"))

get_count_of_pass(titanic_data[5])
get_percent_of_surv(titanic_data[2])
get_percent_of_f_class(titanic_data[3])
get_mid_age_of_pass(titanic_data[6])
