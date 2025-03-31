import requests
import json
import bs4


def get_html(url):
    ob = requests.get(url)
    ob.encoding = "utf-8"
    html = ob.text
    return html


def write_into_json(lst):
    with open('feedbacks.json', 'w', encoding='utf8') as f:
       json.dump(lst, f, ensure_ascii=False, indent=4)


def get_feedbacks(soup):
    res = []
    name = ['id', 'title', 'text']
    index = 1
    for item in soup.find_all(class_='post__title'):
        title = item.find('h3').text
        txt = item.find_next(class_='post__text').text.strip()
        val = [index, title, txt]
        res.append(dict(zip(name, val)))
        index += 1
    write_into_json(res)


url = "https://pcoding.ru/parsing/01/page.html"
soup = bs4.BeautifulSoup(get_html(url), 'html.parser')
get_feedbacks(soup)


'''
ы
'''