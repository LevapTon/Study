class Person(object):

  def __init__(self, s_name, name, patron, height):
    self.s_name = s_name
    self.name = name
    self.patron = patron
    self.height = int(height)

  def __str__(self):
        return f"{(self.s_name).ljust(12)}{(self.name).ljust(10)}{(self.patron).ljust(13)}{str(self.height).ljust(5)}"


def self_description():
  'Описание себя'
  me_line = data.strip().split('\n')
  for line in me_line[1:2]:
    me_tpl = line.split('|')
    s_name, name, patron, height = me_tpl 
    person_item = Person(s_name, name, patron, height)
  print(person_item)


def to_the_list():
  'Преобразование исходных данных в список'
  data_lst = []
  lines = data.strip().split('\n')
  for line in lines[1:]:
    data_tpl = line.split('|')
    s_name, name, patron, height = data_tpl
    data_lst += [Person(s_name, name, patron, height)]
  query_entry(data_lst)


def query_entry(data_lst):
  'Ввод запроса'
  query = input('''Введите запрос.
Например:
"рост больше 120"
"имя Наталья"\n''')
  lines = query.lower().strip().split()
  if 'имя' in lines:
    search_by_name(lines[1], data_lst)
  elif 'фамилия' in lines:
    search_by_s_name(lines[1], data_lst)
  elif 'отчество' in lines:
    search_by_patron(lines[1], data_lst)
  elif 'рост' in lines and ('больше' in lines or 'выше' in lines):
    search_highest(int(lines[2]), data_lst)
  elif 'рост' in lines and ('меньше' in lines or 'ниже' in lines):
    search_lowest(int(lines[2]), data_lst)


def search_by_name(name, data_lst):
  'Поиск по имени'
  for item in data_lst:
    if compare(name, item.name) >= 0.5:
      print(item)


def search_by_s_name(s_name, data_lst):
  'Поиск по фамилии'
  for item in data_lst:
    if compare(s_name, item.s_name) >= 0.5:
      print(item)


def search_by_patron(patron, data_lst):
  'Поиск по отчеству'
  for item in data_lst:
    if compare(patron, item.patron) >= 0.5:
      print(item)


def search_highest(num, data_lst):
  'Поиск людей выше num'
  for item in data_lst:
    if num + 3 >= item.height >= num - 3 or item.height > num:
      print(item)


def search_lowest(num, data_lst):
  'Поиск людей ниже num'
  for item in data_lst:
    if num + 3 >= item.height >= num - 3 or item.height < num:
      print(item)


def compare(word_one, word_two):
  'Нечеткое сравнение запроса и исходных данных'
  ngrams = [word_one[i:i+3] for i in range(len(word_one))]
  count = 0
  for ngram in ngrams:
    count += word_two.count(ngram)
  return(count/max(len(word_one), len(word_two)))


data = '''surname|name|patronymic|height
Плотников|Павел|Сергеевич|174
Абрамова|Елена|Анатольевна|153
Распутин|Николай|Николаевич|197
Никонова|Наталья|Львовна|160
Габидулин|Ранис|Расулович|181
'''


self_description()
to_the_list()