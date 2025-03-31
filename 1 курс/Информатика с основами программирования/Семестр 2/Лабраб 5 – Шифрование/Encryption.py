import random


def workWithtasks():
    '''Работа с задачей'''
    rus = 'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'
    nums = '1234567890'
    smbs = ':; !&-+=()*/.,'
    alph = rus + rus.upper() + nums + smbs  # алфавит символов
    alph = ''.join(random.sample(alph, k=len(alph))) # перемешанный алфавит
    text = input('Введите Ваш текст:\n')
    direct = int(input('Необходимо зашифроватть (1) или дешифровать (2) текст?: '))
    enc_key = int(input('Введите значение сдвига: '))
    way = int(input('Сдвиг вправо (1) или влево (2)?: '))
    if way == 2: way = -1
    result = code(text, direct, alph, enc_key, way)
    print('Результат:\n' + result)
    if direct == 1:
        print('Исходный текст:\n' + decriptText(result, alph, enc_key, way))
    else:
        print('Исходный текст:\n ' + encripText(result, alph, enc_key, way))


def encripText(text, alph, enc_key, way):
    '''Возврат зашифрованного текста'''
    result = ''
    for char in text:
        index = alph.find(char)
        if index != -1:
            if index + enc_key * way < 0: # значение нового элемента < 0
                result += alph[len(alph) + (index + enc_key * way)]                   
            else: 
                if index + enc_key * way >= len(alph): # значение нового элемента >= длины алфавита
                    result += alph[(index + enc_key * way) - len(alph)]                    
                else: 
                    result += alph[index + enc_key * way]
        else:
            print('В тексте присутвует неизвестный символ, он не будет изменен')
            result += char
    return(result)


def decriptText(text, alph, enc_key, way):
    '''Возврат дешифрованного текста'''
    result = ''
    for char in text:
        index = alph.find(char)
        if index != -1:
            if index - enc_key * way < 0: # значение нового элемента < 0
                result += alph[len(alph) + (index - enc_key * way)]
            else:
                if index - enc_key * way >= len(alph): # значение нового элемента >= длины алфавита
                    result += alph[(index - enc_key * way) - len(alph)]
                else:
                    result += alph[index - enc_key * way]
        else:
            print('В тексте присутвует неизвестный символ, он не будет изменен')
            result += char
    return(result)


def code(text, direct,  alph, enc_key, way):
    '''Шифрование и дешифрование текста'''
    result = ''
    if direct == 1: # шифрование текста
        for char in text:
            index = alph.find(char)
            if index != -1:
                if index + enc_key * way < 0: # значение нового элемента < 0
                    result += alph[len(alph) + (index + enc_key * way)]                   
                else: 
                    if index + enc_key * way >= len(alph): # значение нового элемента >= длины алфавита
                        result += alph[(index + enc_key * way) - len(alph)]                    
                    else: 
                        result += alph[index + enc_key * way]
            else:
                print('В тексте присутвует неизвестный символ, он не будет изменен')
                result += char
    else: # дешифрование текста
        for char in text:
            index = alph.find(char)
            if index != -1:
                if index - enc_key * way < 0: # значение нового элемента < 0
                    result += alph[len(alph) + (index - enc_key * way)]
                else:
                    if index - enc_key * way >= len(alph): # значение нового элемента >= длины алфавита
                        result += alph[(index - enc_key * way) - len(alph)]
                    else:
                        result += alph[index - enc_key * way]
            else:
                print('В тексте присутвует неизвестный символ, он не будет изменен')
                result += char
    return(result)


workWithtasks()
# Слово - не воробей, вылетит - не поймаешь!