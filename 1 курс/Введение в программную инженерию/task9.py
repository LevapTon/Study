def is_even(number):
    if number % 2 == 0:
        return('четное')
    else:
        return('Нечетное')


def number_input():
    number = int(input('Введите целое число: '))
    print(is_even(number))


number_input()