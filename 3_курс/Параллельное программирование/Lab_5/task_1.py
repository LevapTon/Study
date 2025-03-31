def fact(num):
    if num == 1:
        return num
    return num * fact(num - 1)


def enter():
    print("Введите число для нахождения факториала:")
    goal = int(input())
    res = fact(goal)
    print(f"Факториал {goal} = {res}")


enter()
