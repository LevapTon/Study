from fractions import Fraction
from math import gcd

m = 7
# Получить измеренное значение
measured_value = 0.625  # это примерное значение, полученное из измерения

# Преобразование в дробь
fraction = Fraction(measured_value).limit_denominator()  # Получение дроби
numerator, denominator = fraction.numerator, fraction.denominator

# Вычисление порядка с использованием gcd
order = gcd(denominator, 2**m)  # Получить порядок
print(order)
