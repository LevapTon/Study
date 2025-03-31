import requests
import json


def get_html(url):
    ob = requests.get(url)
    ob.encoding = "utf-8"
    html = ob.text
    return html


def get_info(html, start, href_b, href_e, data_b, data_e, text_b, text_e):
    res = []
    names = ['data', 'href', 'title', 'text']
    point_one = html.find(start)  # Заголовок таблицы с новостями
    for _ in range(10):
        values = []
        point_one = html.find(href_b, point_one)
        point_two = html.find(href_e, point_one)
        ref_title = html[point_one + len(href_b):point_two].split('">')  # Ссылка с заголовком статьи
        href, title = 'https://pgsha.ru' + ref_title[0], ref_title[1]

        point_one = html.find(data_b, point_two)
        point_two = html.find(data_e, point_one)
        data = html[point_one + len(data_b):point_two]  # Дата статьи

        point_one = html.find(text_b, point_two)
        point_two = html.find(text_e, point_one)
        text = html[point_one + len(text_b):point_two].strip()  # Текст статьи

        values.extend([data, href, title, text])
        res.append(dict(zip(names, values)))
    write_into_json(res)


def write_into_json(lst):
    with open('news.json', 'w', encoding='utf8') as f:
       json.dump(lst, f, ensure_ascii=False, indent=4)


url = "https://pgsha.ru/today/"
html = get_html(url)
start = '<h3  class="panel-title">Новости</h3>'
href_b = '<h5><a href="'
href_e = '</a></h5>'
data_b = '<p><small>'
data_e = '</small></p>'
text_b = '<div style="text-align: justify;">'
text_e = '</div>'
get_info(html, start, href_b, href_e, data_b, data_e, text_b, text_e)
# print(html[html.find(start):html.find(start)+50])


'''
начать искать ин-фу с
<h3  class="panel-title">Новости</h3>
ссылка + заголовок:
<h5><a href="/news_view/dan-start-agrontri-2023/">ДАН СТАРТ АгроНТРИ-2023!</a></h5>
дата:
<p><small>17.04.2023</small></p>
текст:
<div style="text-align: justify;"> + </div>
'''