# from posixpath import split


class Person(object):
    def __init__(self, stud):
        self.id = int(stud.split('|')[0])
        self.name = str(stud.split('|')[1])
        self.group = str(stud.split('|')[2])
        self.age = int(stud.split('|')[3])
        self.gender = str(stud.split('|')[4])
        self.inf = int(stud.split('|')[5])
        self.eng = int(stud.split('|')[6])
        self.math = int(stud.split('|')[7])
    def __str__(self):
        return f"{self.name}\t{self.age}\t{self.gender}\t{self.inf}\t{self.eng}\t{self.math}"


def make_list_of_objects():
    """Формирование списка объектов"""
    stud_obj = []
    with open('Students.csv', encoding='utf-8') as students:
        student = list(map(lambda x: x.strip('\n'), students))
        for i in range(1, len(student)):
            stud_obj.append(Person(student[i]))
        find_man_gender(stud_obj)
        find_mans_army_age(stud_obj)
        best_in_inf(stud_obj)
        best_middle_score(stud_obj)


def find_man_gender(stud_obj):
    """Поиск студентов мужского пола"""
    for items in filter(lambda x: x.gender=='М', stud_obj):
        print(items)


def find_mans_army_age(stud_obj):
    """Поиск студентов муж. пола и призывного возраста"""
    func = lambda x: (x.gender=='М' and x.age > 17)
    for items in filter(func , stud_obj):
        print(items)


def best_in_inf(stud_obj, cnt=3):
    """Поиск троих лучших по информатике"""
    for item in sorted(stud_obj, key=lambda x: x.inf, reverse=True)[:cnt]:
        print(item)


def best_middle_score(stud_obj):
    """Поиск лучших по среднему баллу"""
    func = lambda x: (x.inf + x.eng + x.math)/3
    for item in sorted(stud_obj, key=func, reverse=True):
        print(item, (item.inf + item.eng + item.math)/3)


make_list_of_objects()