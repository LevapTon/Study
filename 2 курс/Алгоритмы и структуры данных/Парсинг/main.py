import requests


def get_html(url):
    ob = requests.get(url)
    ob.encoding = "utf-8"
    html = ob.text
    return html


def get_coll_of_hrefs(oKey, mKey, cKey, html):
    links = []
    names = []
    posSt = html.find(oKey)  # <a class="links" href="
    posM = html.find(mKey, posSt)  # " target=_blank>
    posEn = html.find(cKey, posSt)  # </a>
    while html.find(oKey, posSt) > 0:
        link = html[posSt + len(oKey):posM]
        name = html[posM + len(mKey):posEn]
        if ('.pdf' in link) and ('.pdf' in name):
            links.append(link)
            names.append(name)
        posSt = html.find(oKey, posEn)
        posM = html.find(mKey, posEn)
        posEn = html.find(cKey, posEn + len(cKey))
    create_csv_doc(links, names)


def create_csv_doc(links, names):
    with open ("Files.csv", "w", encoding="utf-8") as f:
        f.write('id,href,name\n')
        for i in range(len(links)):
            f.write("{0},{1},{2}\n".format(i + 1, links[i], names[i]))


def get_first_href(oKey, mKey, cKey, html, pos):
    posSt = html.find(oKey, pos)  # <a class="links" href="
    posM = html.find(mKey, posSt)  # " target=_blank>
    posEn = html.find(cKey, posSt)  # </a>
    print(posSt, posM, posEn)
    if posSt > 0:
        posR = html.find(cKey, posSt) + 1
        link = html[posSt + len(oKey):posM]
        name = html[posM + len(mKey):posEn]
        res = (posR, link, name)
    return res


ua = { "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36" }

url = "https://pcoding.ru/darkNet.php"
html = get_html(url)
openKey = '<a class="links" href="'
midKey = '" target=_blank>'
closeKey = '</a>'
pos = int(input())
get_coll_of_hrefs(openKey, midKey, closeKey, html)
# print(get_first_href(openKey, midKey, closeKey, html, pos))
# print(html)


"""
<a class="links" href="https://pcoding.ru/pdf/AgroRobot.pdf" 
    target=_blank>
        AgroRobot.pdf
</a>
"""