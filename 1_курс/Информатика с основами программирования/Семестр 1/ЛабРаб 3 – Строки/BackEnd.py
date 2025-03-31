def palindrome(palind):
  '''Проверка на палиндром'''
  exSym = ['!', '?', '-', '+', '*', '=', ' ', ':', ';' '.', ',', '"', "'"]
  result = ''
  workWithStr = []
  for smb in palind:
    if smb != exSym and smb !=' ':
      workWithStr.append(smb)
  result = ''.join(workWithStr)
  result = result.lower()
  if result == result[::-1]:
    print('Это палиндром')
  else:
    print('Это не палиндром')


def multiTable(x, y):
  '''Таблица умножения'''
  index1 = 1
  index2 = 1
  for index1 in range(1, y+1):
    for index2 in range(1, x+1):
      print("%-5d" % (index1 * index2), end='')
    print()


def pyramid(height):
  '''Возведение пирамиды'''
  i = 0
  for row in range(height):
    print((height-row-1)*' ' + (i*2+1)*'X')
    i += 1


def parsingAString(lin):
  '''Согласная или гласная строка'''
  soglasnie = ['б', 'в', 'г', 'д', 'ж',
    'з', 'й', 'к', 'л', 'м',
    'н', 'п', 'р', 'с', 'т',
    'ф', 'х', 'ц', 'ч' ,'ш', 'щ']
  glasnie = ['а', 'о', 'э', 'е',
    'и', 'ы', 'у', 'ё', 'ю', 'я']
  countGlas = 0
  countSogl = 0
  lin = lin.lower()
  for smb in lin:
    if smb in soglasnie:
      countSogl += 1
    if smb in glasnie:
      countGlas +=1
  if countGlas > countSogl:
    print('Строка гласная')
  elif countGlas < countSogl:
    print('Строка согласная')
  else:
    print('Гласных и согласных букв одинаковое кол–во')


def rublesEnd(rub):
  '''Окончание рублей'''
  if rub % 10 == 1 and rub != 11:
    print('{0} рубль'.format(rub))
  elif rub % 10 == 2 and rub !=12:
    print('{0} рубля'.format(rub))
  elif rub % 10 == 3 and rub !=13:
    print('{0} рубля'.format(rub))
  elif rub % 10 == 4 and rub !=14:
    print('{0} рубля'.format(rub))
  else:
    print('{0} рублей'.format(rub))