def transportationChallenge(C, X, a, b):
    a, b, C = checkSums(a, b, C)
    print("Производство:", a)
    print("Потребление:", b)
    print("Матрица стоимости:")
    for i in range(len(C)):
        for j in range(len(C[i])):
            print("%2d" % C[i][j], end=" ")
        print()
    X = getStartPlan(a, b)
    print("План:")
    for i in range(len(X)):
        for j in range(len(X[i])):
            print("%2d" % X[i][j], end=" ")
        print()
    f = calcFunc(C, X)
    print("Значение функции:", f)
    while True:
        u, v = getPotentials(C, X)
        print("Потенциалы:")
        print("u:", u)
        print("v:", v)
        evMatrix = getEvaluationMatrix(C, u, v)
        print("Оценочная матрица:")
        for i in range(len(evMatrix)):
            for j in range(len(evMatrix[i])):
                print("%2d" % evMatrix[i][j], end=" ")
            print()
        minEl = evMatrix[0][0]
        for i in range(len(evMatrix)):
            for j in range(len(evMatrix[i])):
                if evMatrix[i][j] < minEl:
                    minEl = evMatrix[i][j]
        if minEl >= 0:
            break
        X = getNewPlan(X, evMatrix)

        # break

    f = calcFunc(C, X)
    print("Оптимальный план:")
    for i in range(len(X)):
        for j in range(len(X[i])):
            print("%2d" % X[i][j], end=" ")
        print()
    print("Значение функции:", f)


def checkSums(a, b, C):
    '''
    Проверка сумм ограничений.

    a – Объем производства

    b – Объем потребления

    C – Матрица стоимости перевозки
    '''
    sumA = sum(a)
    sumB = sum(b)
    if abs(sumA - sumB) != 0:
        if sumA < sumB:
            a.append(abs(sumA - sumB))
            C.append([0 for _ in range(len(b))])
        else:
            b.append(abs(sumA - sumB))
            for i in range(len(a)):
                C[i].append(0)
    return (a, b, C)


def getStartPlan(a, b):
    '''
    Получение начального базиса
    методом северо-западного угла.

    a – Объем производства

    b – Объем потребления
    '''
    X = []
    for i in range(len(a)):
        raw = []
        for j in range(len(b)):
            if b[j] == 0:
                raw.append(0)
                continue
            if a[i] == 0:
                for _ in range(j, len(b)):
                    raw.append(0)
                break
            cur = min(b[j], a[i])
            a[i] -= cur
            b[j] -= cur
            raw.append(cur)
        X.append(raw)
    return X


def getPotentials(C, X):
    '''
    Поиск потенциалов.

    C – Матрица стоимости перевозки

    X – Текущий план
    '''
    u = [None] * len(X)
    v = [None] * len(X)
    u[0] = 0
    count = 0
    rest = len(X)*2 - 1
    for i in range(len(X)):
        for j in range(len(X[i])):
            if X[i][j] > 0:
                count += 1
    if count == rest:
        while (u.count(None) + v.count(None)) > 0:
            for i in range(len(X)):
                for j in range(len(X[i])):
                    if X[i][j] == 0:
                        continue
                    if u[i] == None and v[j] != None:
                        u[i] = v[j] - C[i][j]
                        continue
                    if v[j] == None and u[i] != None:
                        v[j] = C[i][j] + u[i]
                        continue
    else:
        for i in range(len(X)):
            for j in range(len(X[i])):
                sumJ = 0
                for k in range(len(X)):
                    sumJ += X[i][k]
                if sum(X[i][:]) == X[i][j] and sumJ == X[i][j]:
                    if i - 1 > 0:
                        v[j] = C[i-1][j] + u[i-1]
                        u[i] = v[j] - C[i][j]
                    if j - 1 > 0:
                        u[i] = v[j-1] - C[i][j-1]
                        v[j] = C[i][j] + u[i]
                    continue
                if X[i][j] == 0:
                    continue
                if u[i] == None and v[j] != None:
                    u[i] = v[j] - C[i][j]
                    continue
                if v[j] == None and u[i] != None:
                    v[j] = C[i][j] + u[i]
                    continue
    return (u, v)



def getEvaluationMatrix(C, u, v):
    '''
    Получение оценочной матрицы для плана.

    C – Матрица стоимости перевозки

    u – Потенциал по производству (i)

    v – Потенциал по потреблению (j)
    '''
    evMatrix = []
    for i in range(len(C)):
        raw = []
        for j in range(len(C[i])):
            raw.append(C[i][j] - (v[j] - u[i]))
        evMatrix.append(raw)
    return evMatrix


def getNewPlan(X, evMatrix):
    leadI, leadJ = 0, 0
    minEl = evMatrix[leadI][leadJ]
    for i in range(len(evMatrix)):
        for j in range(len(evMatrix[i])):
            if evMatrix[i][j] < minEl:
                leadI = i
                leadJ = j
                minEl = evMatrix[leadI][leadJ]
    # j1 = 0
    # i2 = 0
    # for j in range(len(X)):
    #     if j == leadJ:
    #         continue
    #     if X[leadI][j] != 0:
    #         j1 = j
    #         break

    # for i in range(len(X)):
    #     if i == leadI:
    #         continue
    #     if X[i][leadJ] != 0:
    #         i2 = i
    #         break

    # minEl = min(X[leadI][j1], X[i2][leadJ], X[leadJ][leadI])
    # X[leadI][leadJ] += minEl
    # X[leadI][j1] -= minEl
    # X[i2][leadJ] -= minEl
    # X[i2][j1] += minEl
    chainSign = True
    chainEnd = False
    chain = [(leadI, leadJ)]
    print("Главный элемент:\n", X[leadI][leadJ], leadI, leadJ)
    curCoord = 0
    for i in range(len(X)):
                # if X[i][leadJ] == X[leadI][leadJ]:
                #     continue
                if X[i][leadJ] <= 0:
                    continue
                print(X[i][leadJ], i, leadJ)
                chain.append((i, leadJ))
                curCoord = i
                chainSign = not chainSign
                break
    while not chainEnd:
        if chainSign:
            print("Ищем -")
            for i in range(len(X)-1, -1, -1):
                if i == chain[-1][0]:
                    continue
                if i == leadI and curCoord == leadJ:
                    chainEnd = not chainEnd
                    break
                # if X[i][curCoord] == X[leadI][leadJ]:
                #     chainEnd = not chainEnd
                #     break
                if X[i][curCoord] <= 0:
                    continue
                print(X[i][curCoord], i, curCoord)
                chain.append((i, curCoord))
                curCoord = i
                chainSign = not chainSign
                break
        else:
            print("Ищем +")
            # chainEnd = not chainEnd
            for j in range(len(X)-1, -1, -1):
                if j == chain[-1][1]:
                    continue
                if curCoord == leadI and j == leadJ:
                    chainEnd = not chainEnd
                    break
                if X[curCoord][j] == X[leadI][leadJ]:
                    continue
                print(X[curCoord][j], curCoord, j)
                chain.append((curCoord, j))
                curCoord = j
                chainSign = not chainSign
                break
            
    print(chain)
    arr = []
    for k in range(len(chain)//2):
        arr.append(X[chain[2*k+1][0]][chain[2*k+1][1]]) 
    minEl = min(arr)
    for k in range(len(chain)//2):
        X[chain[2*k][0]][chain[2*k][1]] += minEl
        X[chain[2*k+1][0]][chain[2*k+1][1]] -= minEl

    print("Новый план:")
    for i in range(len(X)):
        for j in range(len(X[i])):
            print("%2d" % X[i][j], end=" ")
        print()
    return X


def calcFunc(C, X):
    '''
    Получение значения функции.

    C – Матрица стоимости перевозки
    
    X – Текущий план
    '''
    f = 0
    for i in range(len(C)):
        for j in range(len(C[i])):
            f += C[i][j] * X[i][j]
    return f


# Матрица стоимости перевозки
C = [[2, 4, 1, 3],
     [5, 6, 5, 4],
     [3, 7, 9, 5],
     [1, 2, 2, 7]]

# План производства
X = []

# Объем производства
a = [30, 20, 40, 50]

# Объем потребления
b = [35, 20, 55, 30]

transportationChallenge(C, X, a, b)
