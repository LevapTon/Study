# **Лабораторная работа №3** 
---  
В данной работе необходимо исправить такие сортировки как: быстрая и слияния. Далее необходимо сравнить время выполнения исправленных и исходных сортировкок. Примерная сложность алгоритма: n * log n  
---
## **Таблица сравнения**  
---
|Кол-во эл-тов (ед.)|Слияние+ (с.)|Быстрая (с.)|Быстрая+ (с.)|
|----------|-----------|-----------|-----------|
|100000    |0,828      |1,076      |0,667      |
|250000    |3,385      |3,115      |1,803      |
|500000    |4,441      |6,48       |4,041      |
|750000    |6,871      |10,284     |6,188      |
|1000000   |9,56       |13,857     |8,481      |
|2000000   |21,07      |28,075     |17,343     |  
---  
Исходная сортировка слиянием не участвовала в сравнении, так как из-за некорректно написанного кода алгоритм переполнял стек рекурсии даже при малых числах. Исходная быстрая сортировка заметно замедляет скорость выполнения работы за счет трех отдельных фильтров для нахождения элементов. В данном тесте исправленная сортировка слиянием показала лучший результат.  
В результате проведенных замеров видно, что при увеличении количества элементов в два раза время выполнения программы возрастает примерно в два раза, следовательно сложность алгоритов действительно n * log n.
