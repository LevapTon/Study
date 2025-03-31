def palindrome():
  '''Проверка на палиндром'''
  with open('файлы\\Input.txt', encoding='utf-8') as file:
    with open('файлы\\Output.txt', 'w', encoding='utf-8') as res:
        for line in file:
            palind = line
            exSym = ['!', '?', '-', '+', '*', '=', ' ', ':',
            ';', '.', ',', '"', "'", '\n']
            result = ''
            workWithStr = []
            for smb in palind:
                if smb not in exSym:
                    workWithStr.append(smb)
            result = ''.join(workWithStr)
            result = result.lower()
            if result == result[::-1]:
                res.write(line)

palindrome()