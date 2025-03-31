CharactersList = [
  ['', '', ''], # 1й персонаж, "Игрок"
  [200, 300, 50], # 2й персонаж, "Боец"
  [100, 100, 100], # 3й перснонаж, "Лекарь"
  [150, 200, 150]] # 4й персонаж, "Стрелок"
# У каждого персонажа есть 3 критерия,
# которые показывают значения защиты,
# здоровья и скорости, соответственно.
# Значения критериев 1-го персонажа
# необходимо заполнить самому
CharactersDict = {
  'Игрок': {
    "Защита": '',
    "Здоровье": '',
    "Скорость": ''},
  'Боец': {
    "Защита": 200,
    "Здоровье": 300,
    "Скорость": 50},
  'Лекарь': {
    "Защита": 100,
    "Здоровье": 100,
    "Скорость": 100},
  'Стрелок': {
    "Защита": 150,
    "Здоровье": 200,
    "Скорость": 150},
}
# Аналогичная ситуация с персонажами в списке

def FindBestChList():
  print('Обработка списка')
  for critNum in range(3):
    max = -1
    if critNum == 0:
      critName = 'Защита'
    elif critNum == 1:
      critName = 'Здоровье'
    else:
      critName = 'Скорость'
    for charNum in range(4):
      if CharactersList[charNum][critNum] > max:
        max = CharactersList[charNum][critNum]
        if charNum == 0:
          bestChar = 'Игрок'
        elif charNum == 1:
          bestChar = 'Боец'
        elif charNum == 2:
          bestChar = 'Лекарь'
        else:
          bestChar = 'Стрелок'
    print('Лучший персонаж по критерию %(0)s: %(1)s' % {"0": critName, "1":  bestChar})


def FindBestChDict():
  print('Обработка словаря')
  for critNum in range(3):
    max = -1
    if critNum == 0:
      critName = 'Защита'
    elif critNum == 1:
      critName = 'Здоровье'
    else:
      critName = 'Скорость'
    for charNum in range(4):
      if charNum == 0:
        currentChar = 'Игрок'
      elif charNum == 1:
        currentChar = 'Боец'
      elif charNum == 2:
        currentChar = 'Лекарь'
      else:
        currentChar = 'Стрелок'
      if CharactersDict[currentChar][critName] > max:
        max = CharactersDict[currentChar][critName]
        if charNum == 0:
          bestChar = 'Игрок'
        elif charNum == 1:
          bestChar = 'Боец'
        elif charNum == 2:
          bestChar = 'Лекарь'
        else:
          bestChar = 'Стрелок'
    print('Лучший персонаж по критерию %(0)s: %(1)s' % {"0": critName, "1":  bestChar})


def inputCherCritOne():
  print('''Введите по очереди критерии персонажа
"Игрок", который описан списком.''')
  for i in range(3):
    if i == 0:
      critName = 'Защита'
    elif i == 1:
      critName = 'Здоровье'
    else:
      critName = 'Скорость'
    CharactersList[0][i] = int(input('Критерий %s: ' % (critName)))
  FindBestChList()


def inputCherCritTwo():
  print('''Введите по очереди критерии персонажа
"Игрок", который описан словарем.''')
  for i in range(3):
    if i == 0:
      critName = 'Защита'
    elif i == 1:
      critName = 'Здоровье'
    else:
      critName = 'Скорость'
    CharactersDict["Игрок"][critName] = int(input('Критерий %s: ' % (critName)))
  FindBestChDict()


inputCherCritOne()
inputCherCritTwo()