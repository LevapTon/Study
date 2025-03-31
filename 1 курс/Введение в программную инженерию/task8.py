class Person(object):

  def __init__(self, s_name, name, patron, height, street, number):
    self.s_name = s_name
    self.name = name
    self.patron = patron
    self.height = int(height)
    self.street = street
    self.number = int(number)

  def __str__(self):
        return f"{(self.s_name).ljust(12)}{(self.name).ljust(12)}{(self.patron).ljust(13)}{str(self.height).ljust(5)}{(self.street).ljust(15)}{str(self.number).ljust(5)}"


def to_the_list():
  'Преобразование исходных данных в список'
  data_lst = []
  lines = data.strip().split('\n')
  for line in lines[1:]:
    data_tpl = line.split('|')
    s_name, name, patron, height, street, number = data_tpl
    data_lst += [Person(s_name, name, patron, height, street, number)]
  query_entry(data_lst)


def query_entry(data_lst):
  'Ввод запроса'
  query = input('''Введите запрос.
Например:
"рост больше 120"
"имя Наталья"\n''')
  lines = query.lower().strip().split()
  if ('имя' or 'зовут') in lines:
    search_by_name(lines[1], data_lst)
  elif 'фамилия' in lines:
    search_by_s_name(lines[1], data_lst)
  elif 'отчество' in lines:
    search_by_patron(lines[1], data_lst)
  elif ('живет' and 'на') in lines:
    search_by_street(lines[2], data_lst)
  elif 'дом' in lines:
    search_by_number(int(lines[1]), data_lst)
  elif 'рост' in lines and ('больше' in lines or 'выше' in lines):
    search_highest(int(lines[2]), data_lst)
  elif 'рост' in lines and ('меньше' in lines or 'ниже' in lines):
    search_lowest(int(lines[2]), data_lst)


def search_by_name(name, data_lst):
  'Поиск по имени'
  for item in data_lst:
    percent = compare(name, item.name)
    print('Процент совпадения: %.2f' % percent)
    print(item)


def search_by_s_name(s_name, data_lst):
  'Поиск по фамилии'
  for item in data_lst:
    percent = compare(s_name, item.s_name)
    print('Процент совпадения: %.2f' % percent)
    print(item)


def search_by_patron(patron, data_lst):
  'Поиск по отчеству'
  for item in data_lst:
    percent = compare(patron, item.patron)
    print('Процент совпадения: %.2f' % percent)
    print(item)


def search_by_street(street, data_lst):
  'Поиск по улице'
  for item in data_lst:
    percent = compare(street, item.street)
    print('Процент совпадения: %.2f' % percent)
    print(item)


def search_by_number(num, data_lst):
  'Поиск по номеру дома'
  print('Точность: ±3 ед. от', num)
  for item in data_lst:
      print(item)


def search_highest(num, data_lst):
  'Поиск людей выше num'
  print('Точность: ±3 ед. от %.i или выше %.i' %(num, num))
  for item in data_lst:
      print(item)


def search_lowest(num, data_lst):
  'Поиск людей ниже num'
  print('Точность поиска: ±3 ед. от %.i или ниже %.i' %(num, num))
  for item in data_lst:
      print(item)


def compare(word_one, word_two):
  'Нечеткое сравнение запроса и исходных данных'
  ngrams = [word_one[i:i+3] for i in range(len(word_one))]
  count = 0
  for ngram in ngrams:
    count += word_two.count(ngram)
  return(count/max(len(word_one), len(word_two)))


data = '''surname|name|patronymic|height|street|number
Плотников|Павел|Сергеевич|174|Пушкина|89
Абрамова|Елена|Анатольевна|153|Ленина|47
Распутин|Николай|Николаевич|197|Октябрьская|56
Никонова|Елена|Львовна|160|Революции|3
Габидулин|Ранис|Расулович|181|Комсомольская|28
Садчиков|Александр|Алексеевич|164|Пушкина|29
Шабанова|Александра|Павловна|157|Революции|59
'''


to_the_list()