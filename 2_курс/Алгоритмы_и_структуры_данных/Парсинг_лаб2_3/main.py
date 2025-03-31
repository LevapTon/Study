import requests
import json
from bs4 import BeautifulSoup


def get_html(url):
    ob = requests.get(url)
    ob.encoding = "utf-8"
    html = ob.text
    return html


def write_into_json(lst):
    with open('pads.json', 'w', encoding='utf8') as f:
       json.dump(lst, f, ensure_ascii=False, indent=4)


def get_info(soup, url):
    res = []
    names = ['id', 'name', 'href', 'cost']
    index = 1
    for i in range(1, 11):
        for item in soup.find("form", {"id": "list_form1"}).find_all(class_='model-short-block'):
            href = item.find('a').get('href')
            href = 'https://n-katalog.ru' + href
            name = item.find("span", {"class": "u"}).text
            cost = item.find("div", {"class": "model-price-range"}).find('a').text
            cost = 'от ' + cost.replace(' ', " ")
            val = [index, name, href, cost]
            index += 1
            res.append(dict(zip(names, val)))
        url = url.replace('page={0}', 'page={1}').format(i, i + 1)
        soup = BeautifulSoup(get_html(url), 'html.parser')
    write_into_json(res)


url = "https://n-katalog.ru/category/planshety/list?page=1"
soup = BeautifulSoup(get_html(url), 'html.parser')
get_info(soup, url)


'''
ы
'''