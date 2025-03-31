def fib(num):
    if num == 1:
        return num
    if num < 1:
        return 0
    return fib(num - 1) + fib(num - 2)


def enter():
    print("Введите номер числа Фибоначчи:")
    goal = int(input())
    res = fib(goal)
    print(f"Фибоначчи {goal} = {res}")


enter()
