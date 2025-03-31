import networkx as nx
import matplotlib.pyplot as plt

def strassen_algorithm_flowchart():
    # Создаем граф
    G = nx.DiGraph()

    # Добавляем узлы (шаги алгоритма)
    steps = [
        "Шаг 1: Дискретное преобразование Фурье",
        "Шаг 2: Рекурсивное разделение массива с последующим ДПФ",
        "Шаг 3: Оратная рекурсия с объединением результатов"
    ]
    for step in steps:
        G.add_node(step)

    # Добавляем ребра (соединения между шагами)
    edges = [
        ("Шаг 1: Дискретное преобразование Фурье", "Шаг 2: Рекурсивное разделение массива с последующим ДПФ"),
        ("Шаг 2: Рекурсивное разделение массива с последующим ДПФ", "Шаг 3: Оратная рекурсия с объединением результатов")
    ]
    G.add_edges_from(edges)

    # Располагаем узлы на диаграмме
    pos = nx.spring_layout(G)

    # Рисуем граф
    nx.draw_networkx_nodes(G, pos, node_size=2000, node_color='skyblue')
    nx.draw_networkx_labels(G, pos, font_size=10, font_family='sans-serif')
    nx.draw_networkx_edges(G, pos, width=2, arrowsize=20)

    # Отображаем граф
    plt.title("Алгоритм быстрого преобразования Фурье")
    plt.axis('off')
    plt.show()

# Отобразим диаграмму потока выполнения алгоритма Штрассена
strassen_algorithm_flowchart()
