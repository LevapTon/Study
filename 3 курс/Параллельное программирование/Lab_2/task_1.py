def horner_scheme(coefficients, x):
    result = coefficients[0]
    for coef in coefficients[1:]:
        result = result * x + coef
    return result

# Пример использования
coefficients = [1, -3, 2]  # коэффициенты многочлена: x^2 - 3x + 2
x = 5  # значение x
result = horner_scheme(coefficients, x)
print("-"*4,"ЗАДАНИЕ 1","-"*4)
print("Значение многочлена в точке", x, ":", result)
