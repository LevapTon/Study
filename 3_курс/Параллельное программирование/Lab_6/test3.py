from qiskit import *
from qiskit_aer import AerSimulator
from qiskit.visualization import *
from math import *
import fractions
import numpy as np

# Определяем функцию квантового преобразования Фурье (QFT)
def qft(circuit, n):
    """Применение QFT к квантовому регистру"""
    for i in range(n):
        for j in range(i):
            circuit.cp(np.pi / (2 ** (i - j)), j, i)
        circuit.h(i)

# Создаем квантовую схему для алгоритма Шора
n = 4  # Число кубитов для QFT (достаточно для факторизации числа 15)
qc = QuantumCircuit(n, n)  # Второй аргумент - регистр для измерений

# Применяем операцию Адамара для создания суперпозиции
for i in range(n):
    qc.h(i)

# Применяем QFT
qft(qc, n)

# Измеряем все кубиты после применения QFT
qc.measure(range(n), range(n))  # Измеряем все кубиты

# Запускаем квантовый симулятор
simulator = AerSimulator(method='statevector')
transpiled_qc = transpile(qc, simulator)

# Выполнение квантовой схемы и получение результатов
job = simulator.run(transpiled_qc, shots=4096)
# Выполняем несколько раз, чтобы подтвердить результат
periods = []
for _ in range(10):  # Повторяем 10 раз для устойчивости
    results = simulator.run(transpiled_qc, shots=1024).result()
    counts = results.get_counts()
    
    measured_value = max(counts, key=counts.get)  # Наиболее вероятное значение
    fractional_value = int(measured_value, 2) / (2 ** n)
    approximate_fraction = fractions.Fraction(fractional_value).limit_denominator()
    period = approximate_fraction.denominator
    
    periods.append(period)

print("Полученные периоды:", periods)  # Показ всех периодов

# Находим делители числа 15 на основе полученного периода
N = 15
a = 7

if period % 2 == 1:
    raise ValueError("Полученный порядок нечетный, попробуйте еще раз.")

candidate1 = gcd(a ** (period // 2) - 1, N)
candidate2 = gcd(a ** (period // 2) + 1, N)

print(f"Кандидаты для делителей числа {N}: {candidate1}, {candidate2}")
