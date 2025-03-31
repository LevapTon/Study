def read_file(file_name, sep=','):
    with open(file_name) as f:
        lines = f.readlines()
    return [tuple(map(int, line.split(sep))) for line in lines]


def find_max_way(x, y, p, w):
    if x == rx-1 and y == ry-1:
        if p > resOne[0]:
            resOne[0] = p
            resOne[1] = w
    else:
        if x+1 < rx: 
            find_max_way(x+1, y, p+ways[y][x+1], w + 'x')
        if y+1 < ry:
            find_max_way(x, y+1, p+ways[y+1][x], w + 'y')


def find_min_jumps(x, y, c, w):
    if x == rx-1 and y == ry-1:
        if c < resTwo[0]:
            resTwo[0] = c
            resTwo[1] = w
    else:
        if x+steps[y][x] < rx: 
            find_min_jumps(x+steps[y][x], y, c + 1, w + f'x{steps[y][x]}')
        if y+steps[y][x] < ry:
            find_min_jumps(x, y+steps[y][x], c + 1, w + 'y' + str(steps[y][x]))


ways = read_file('Ways.csv')
steps = read_file('Steps.csv')
resOne = [0, ""]
resTwo = [len(steps[0])**2, ""]
rx, ry = len(ways[0]), len(ways)
find_max_way(0,0,ways[0][0], "")
print ("Задание №1\nМаксимальная длина: {0}, Путь: {1}".format(resOne[0], resOne[1]))
rx, ry = len(steps[0]), len(steps)
find_min_jumps(0,0,0, "")
print ("Задание №2\nМинимальное кол-во шагов: {0}, Путь: {1}".format(resTwo[0], resTwo[1]))
