from PIL import Image
from os import path

with Image.open('./white.jpg') as img:
    widht, height = img.size
    k_nmb = 256 / (min(widht // 2, height // 2) * 2**.5)
    for y in range(height):
        for x in range(widht):
            nmb = int((x - widht // 2)**2 + (y - height // 2)**2)**.5
            r = 255 - int(k_nmb * nmb)
            g = 255 - int(k_nmb * nmb)
            b = 255 - int(k_nmb * nmb)
            img.putpixel((x, y), (r, g, b))
    img.save('circle_grad.jpg')
    img.show()