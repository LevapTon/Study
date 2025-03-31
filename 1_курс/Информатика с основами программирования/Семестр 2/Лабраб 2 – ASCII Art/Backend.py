from PIL import Image
import PIL.ImageOps


def get_image_resize(img, height_new):
    width, height = img.size  # исходные размеры рисунка
    width_new = width // (height // height_new)
    return img.resize((width_new, height_new), Image.ANTIALIAS)


def rework_to_ascii(k, smb_num, Save):
    img = Image.open('images/emperor.jpg')
    img_new = get_image_resize(img, k)
    img.close()
    if Save == '+':
        to_acscii_art_save(img_new, smb_num)
    else:
        to_acscii_art(img_new, smb_num)
    


def to_acscii_art_save(img_new, smb_num):
    smb_list = []
    with open('Symbols.ini', encoding='utf-8') as smb:
        for items in smb: smb_list.append(items)
    symbols = smb_list[smb_num]

    count = len(symbols)
    full = 256 + 256 + 256  # максимальное значение
    segment = full // count  # длина сегмента

    result = ''
    width, height = img_new.size
    with open('emperor_ascii.txt', 'w', encoding='utf-8') as img_res:
        for y in range(height):
            for x in range(width):
                r, g, b = img_new.getpixel((x, y))
                color = r + g + b
                pos = 0
                if color >= segment * 1:
                    pos = 1
                if color >= segment * 2:
                    pos = 2
                if color >= segment * 3:
                    pos = 3
                result += symbols[pos] * 3
            result += '\n'
            img_res.write(result)
        print(result)

    
def to_acscii_art(img_new, smb_num):
    smb_list = []
    with open('Symbols.ini', encoding='utf-8') as smb:
        for items in smb: smb_list.append(items)
    symbols = smb_list[smb_num]

    count = len(symbols)
    full = 256 + 256 + 256  # максимальное значение
    segment = full // count  # длина сегмента

    result = ''
    width, height = img_new.size
    for y in range(height):
        for x in range(width):
            r, g, b = img_new.getpixel((x, y))
            color = r + g + b
            pos = 0
            if color >= segment * 1:
                pos = 1
            if color >= segment * 2:
                pos = 2
            if color >= segment * 3:
                pos = 3
            result += symbols[pos] * 3
        result += '\n'
    print(result)


def invert_color(Save):
    with Image.open('images/emperor.jpg') as img:
        img_inv = PIL.ImageOps.invert(img)
        img_inv.show()
        if Save == '+':
            img_inv.save('results/emperor_inv_color.jpg')


def invert_left_right(Save):
    with Image.open('images/emperor.jpg') as img:
        img_inv = img.transpose(Image.FLIP_LEFT_RIGHT)
        img_inv.show()
        if Save == '+':
            img_inv.save('results/emperor_inv_left-right.jpg')


def invert_top_bottom(Save):
    with Image.open('images/emperor.jpg') as img:
        img_inv = img.transpose(Image.FLIP_TOP_BOTTOM)
        img_inv.show()
        if Save == '+':
            img_inv.save('results/emperor_inv_top-bottom.jpg')