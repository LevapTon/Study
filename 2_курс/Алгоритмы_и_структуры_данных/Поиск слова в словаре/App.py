def read_txt():
    with open('Words.txt', 'r', encoding='utf-8') as file:
        text = file.read().split('\n')
    return text


def find_word(text, word):
    left = 0
    right = len(text) - 1
    while True:
        mid = (left + right) // 2
        if text[mid] == word:
            return "Слово на позиции: {0}".format(mid + 1)
        if (mid < left or mid > right):
            return "Нет слова в словаре"
        if text[mid] < word:
            left = mid + 1
        else:
            right = mid - 1


word = "спирализация"
print (find_word(read_txt(), word))
