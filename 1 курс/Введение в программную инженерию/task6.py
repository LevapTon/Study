person_one = [
  "Алексеев",
  "Алексей",
  "Алексеевич",
  "18",]


person_two = {
  "Фамилия": "Иванов",
  "Имя": "Иван",
  "Отчество": "Иванович",
  "Возраст": "20",}


def search_by_request(query):
  'Поиск по словарю'
  if query == 'Фамилия':
    print('Поиск по словарю: ', person_two[query])
  elif query == 'Имя':
    print('Поиск по словарю: ', person_two[query])
  elif query == 'Отчество':
    print('Поиск по словарю: ', person_two[query])
  elif query == 'Возраст':
    print('Поиск по словарю: ', person_two[query])
  'Поиск по списку'
  if query == 'Фамилия':
    index = 0
    print('Поиск по списку: ', person_one[index])
  elif query == 'Имя':
    index = 1
    print('Поиск по списку: ', person_one[index])
  elif query == 'Отчество':
    index = 2
    print('Поиск по списку: ', person_one[index])
  elif query == 'Возраст':
    index = 3
    print('Поиск по списку: ', person_one[index])
  print(compare(person_one[index], person_two[query]))


def compare(word_one, word_two):
  ngrams = [word_one[i:i+3] for i in range(len(word_one))]
  count = 0
  for ngram in ngrams:
    count += word_two.count(ngram)
  return(count/max(len(word_one), len(word_two)))


def query_entry():
  query = input('Введите запрос:\n')
  search_by_request(query)

query_entry()