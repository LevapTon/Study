w = int(input())
density = 1000
length = 1

while True:
    width = length / 3
    height = width / 2
    volume = height * length * width
    plotnost = volume * 50
    if plotnost + w == volume / 1000 * density:
        if height >= 30:
            print(length, width, height)
            break
    length += 1