from qiskit import QuantumCircuit, transpile, assemble
from qiskit_aer import AerSimulator
import numpy as np
import random

def qft_circuit(n):
    qc = QuantumCircuit(n)
    for j in range(n):
        for k in range(j):
            qc.cp(np.pi/float(2**(j-k)), k, j)
        qc.h(j)
    return qc

def shor_algorithm(N):
    # Выбираем количество кубитов для алгоритма
    n_qubits = 2 * N.bit_length()  

    # Создаем квантовую схему
    qc = QuantumCircuit(n_qubits, n_qubits)

    # Инициализация регистра
    qc.h(range(n_qubits))
    
    # Выбор случайного числа a, меньшего N
    a = random.randint(2, N - 1)
    
    # Применение операции f(x) = a^x mod N
    for i in range(n_qubits // 2):
        qc.x(i)
        qc.append(qft_circuit(n_qubits), range(n_qubits))
        qc.measure(range(n_qubits // 2), range(n_qubits // 2))
    
    # Выполнение схемы на симуляторе
    simulator = AerSimulator()
    t_qc = transpile(qc, simulator)
    qobj = assemble(t_qc)
    result = simulator.run(qobj).result()
    counts = result.get_counts()
    
    # Поиск периода
    period = 0
    for state in counts:
        if counts[state] > 0:
            period = int(state[::-1], 2)
            break
    
    # Извлечение порядка из результата измерения
    r = int(period / (2 ** (n_qubits // 2)))
    
    return r, a

# Пример использования:
N = 15
r, a = shor_algorithm(N)
print("Порядок r:", r)
print("Выбранное a:", a)
