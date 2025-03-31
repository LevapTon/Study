from operator import indexOf


def get_pos_naive(text, phrase):
    lw = len(phrase)
    for pos in range(len(text) - lw):
        flag = True
        for index in range(lw):
            if (phrase[index] != text[pos + index]):
                flag = False
                break
        if (flag == True):
            return pos
    return -1

def get_pos_equel(text, phrase):
    lw = len(phrase)
    for pos in range(len(text)):
        if phrase == text[pos:pos+lw]:
            return pos
    return -1

def get_pos_bm(text, phrase):
    dct = {x: len(phrase) - 1 - indexOf(phrase, x) for x in phrase}


phrase = 'Ты хорошо сражался, приятель'  # lw = 6
with open('Text.txt', 'r', encoding='utf-8') as file:
    text = file.read()
    print (get_pos_naive(text, phrase))
    #print (text[:10])
 
#phrase = 'Вы служили долго и весьма хорошо'
#phrase = 'Ты хорошо сражался, приятель'

'''
Во славу повелителя бурь Имотеха
Вы служили долго и весьма хорошо
Ты хорошо сражался, приятель
'''
#print (get_pos_naive(text, phrase))
#dct = {x: len(phrase) - 1 - indexOf(phrase, x) for x in phrase}
#print (dct)