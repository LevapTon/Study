def workWithStrings():
  strOne = 'xxxabcdxxx'
  strTwo = ''
  print(strOne)
  for i in strOne:
    if i == 'x':
      strTwo += 'y'
    else:
      strTwo += i
  print(strTwo)


def workWithNumbers():
  num = [1, 5, 15, 30, 7, 8, 29]
  result = 1
  for i in num:
    if i % 3 == 0 and i % 5 == 0:
      result *= i
  print(result)


def wordChanger():
  strOne = 'xxxabcdxxx'
  print(strOne)
  for i in strOne:
    if i == 'x':
      strOne = strOne.replace(i, 'y')
  print(strOne)


workWithStrings()
workWithNumbers()
wordChanger()