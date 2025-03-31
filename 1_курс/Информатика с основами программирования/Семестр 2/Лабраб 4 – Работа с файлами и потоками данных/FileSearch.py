import os
from os.path import isfile, join, isdir


def workWithTasks():
    """Выполнение поставленных задач"""
    lst_files = list(makeListOfCurrentFiles())
    print('Список файлов в текущей папке:')
    for item in lst_files: 
        if os.path.isfile(item) == True: print(item)
    print('Отсортированный по размеру список:')
    for item in sorted(lst_files, key=lambda x: os.stat(x).st_size, reverse=True):
        if os.path.isfile(item) == True: print(item)
    changeExtAndAddInfo()


def makeListOfCurrentFiles():
    """Формирование списка файлов в папке с программой"""
    workdir = os.getcwd()
    lst_files = map(lambda x: x, os.listdir(workdir))
    return(lst_files)


def changeExtAndAddInfo():
    """Изменение расширения файла с .ini на .cfg
    и изменение их содержимого"""
    results = []
    dirs = ["."]
    while dirs:
        last = dirs.pop()
        lst = os.listdir(last)
        for file in filter(lambda item: isfile(join(last, item)), lst):
            results.append(join(last, file))
        cur_dirs = [join(last, item) for item in lst if isdir(join(last, item))]
        dirs = cur_dirs + dirs
    for item in results:
        if item.endswith('.ini') == True:
            name = os.path.splitext(item)[0]
            os.rename(item, name + '.cfg')
    for elem in results:
         if elem.endswith('.cfg') == True:
             with open(elem, 'a') as open_file:
                print(open_file)
                file_name = os.path.splitext(os.path.basename(elem))[0]
                open_file.write('name_file: ' + file_name + '\n')


workWithTasks()