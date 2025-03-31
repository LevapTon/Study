import requests
import json
from bs4 import BeautifulSoup


def get_html(url):
    ob = requests.get(url)
    ob.encoding = "utf-8"
    html = ob.text
    return html


def write_into_json(lst):
    with open('news.json', 'w', encoding='utf8') as f:
       json.dump(lst, f, ensure_ascii=False, indent=4)


def get_news(soup):
    res = []
    name = ['data', 'title', 'href', 'text']
    for item in soup.find_all(class_='list-entry'):
        title = item.find_next('h5').text
        href = item.find_next('a').get('href')
        data = item.find_next('small').text
        txt = item.find("div", {"style": "text-align: justify;"}).text.strip()
        val = [data, title, href, txt]
        res.append(dict(zip(name, val)))
    write_into_json(res)


url = "https://pgatu.ru/today/"
soup = BeautifulSoup(get_html(url), 'html.parser')
get_news(soup)


'''
ы
'''
