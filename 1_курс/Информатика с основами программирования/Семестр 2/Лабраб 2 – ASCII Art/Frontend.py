import Backend


def interface():
    print('Преобразование картинки')
    Size = int(input('Введите итоговый размер картинки: '))
    print(*open('Symbols.ini', encoding='utf-8'))
    smb_num = int(input('Выбирете набор символов: ')) - 1
    color = input('Инвертировать цвет? (+/-): ')
    left_right = input('Инвертировать стороны лево-право? (+/-): ')
    up_down = input('Инвертировать стороны низ-верх? (+/-): ')
    res = input('Сохранить результаты в файл? (+/-): ')

    Backend.rework_to_ascii(Size, smb_num, res)
    if color == '+':
        Backend.invert_color(res)  
    if left_right =='+':
        Backend.invert_left_right(res)
    if up_down == '+':
        Backend.invert_top_bottom(res)


interface()