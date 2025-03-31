/* ------------------------------------------------------------------------- */
/*  Данный файл является частью комплекта исходных текстов,                  */
/*  иллюстрирующих материал следующего издания:                              */
/*  Федотов И.Е.                                                             */
/*  Модели параллельного программирования. - М.: СОЛОН-ПРЕСС, 2012. - 384 с. */
/*  ISBN 978-5-91359-102-9                                                   */
/*                                                                           */
/*  Как и само издание, прилагаемые к нему исходные тексты предназначены     */
/*  лишь для ознакомления с принципами высокоуровневого построения           */
/*  параллельных программ, а также для проведения дальнейших экспериментов.  */
/*  Использование этого кода в качестве основы для реальных программ         */
/*  допустимо, однако требует доработок и может осуществляться лишь          */
/*  на личный страх и риск разработчика.                                     */
/*  Исходные тексты предоставляются "как есть", без каких бы то ни было      */
/*  явных или неявных гарантий пригодности к практическому применению.       */
/*                                                                           */
/*  Copyright © 2008-2011 Федотов И.Е.                                       */
/* ------------------------------------------------------------------------- */


#include "zquantum.hxx"
#include "../../common/synprintf.hxx"


using namespace std;
using namespace z;
using namespace z::quantum;

// схема классической функции со значением в 1 бит
// |x> |0>  -->  |x> |f(x)>
// в данном примере функция инвертирует бит значения,
// если аргумент равен заданному числу num
class func_type: public circuit_type
{
public:
    func_type(int n, int num):
        circuit_type(n + 1)
    {
        assert(n > 0 && num >= 0 && num < (1 << n));
        // временная инверсия битов, соответствующих нулям константы
        for (int i = 0; i < n; ++i)
        {
            if ((num & (1 << i)) == 0)
                add_unary(gate_not_type(), i);
        };

        // инверсия значения функции в случае, если все входы = 1
        pinlist_type argpin = range(0, n), pin = range(n, qubit_number());
        pin.insert(pin.end(), argpin.begin(), argpin.end());
        add_circuit(circuit_ccccnot_type(qubit_number()), pin);

        // возврат битов аргумента в первоначальное значение
        for (int i = 0; i < n; ++i)
        {
            if ((num & (1 << i)) == 0)
                add_unary(gate_not_type(), i);
        };
    }
};

// схема многократно управляемого вентиля Паули Z
class controlled_zpauli_type: public circuit_type
{
public:
    controlled_zpauli_type(int n):
        circuit_type(n)
    {
        // если схема занимает более одного бита
        if (n > 1)
        {
            // подлежащая управлению схема - также управляемая
            controlled_zpauli_type lower(n - 1);
            add_circuit(
                circuit_controlled_type(lower),
                range(0, qubit_number()));
        }
        else
            // в противном случае управляемая схема - Z-вентиль
            add_unary(gate_shift_type(0.5), 0);
    }
};

// схема инверсии амплитуд относительно их среднего значения
class average_inversion_type: public circuit_type
{
public:
    average_inversion_type(int n):
        circuit_type(n)
    {
        pinlist_type pin = range(0, qubit_number());
        // предварительное преобразование Уолша-Адамара
        add_circuit(circuit_hadamard_type(qubit_number()), pin);

        // временная инверсия всех кубитов
        for (int i = 0; i < qubit_number(); ++i)
            add_unary(gate_not_type(), i);

        // многократно управляемый Z-вентиль
        // можно повесить на любой кубит, мы вешаем на нулевой
        add_circuit(controlled_zpauli_type(qubit_number()), pin);

        // обратная инверсия кубитов
        for (int i = 0; i < qubit_number(); ++i)
            add_unary(gate_not_type(), i);

        // это дань "честной" инверсии относительно среднего,
        // хотя в данном случае она не нужна, поскольку минус
        // оказывается общим фазовым сдвигом квантового состояния
        /*
        // обращение знака у всех амплитуд
        add_unary(gate_shift_type(0.5), 0);
        add_unary(gate_not_type(), 0);
        add_unary(gate_shift_type(0.5), 0);
        add_unary(gate_not_type(), 0);
        //*/

        // заключительное преобразование Уолша-Адамара
        add_circuit(circuit_hadamard_type(qubit_number()), pin);
    }
};

int main(int argc, char *argv[])
{
    // длина аргумента функции
    const int n = 12;
    // количество итераций Гровера
    const int iternum = int(0.25 * M_PI * sqrt(1.0 * (1 << n)) + 0.5);

    // исследуемая двоичная функция от n кубитов
    func_type func(n, 219);

    // квантовый компьютер
    quantum_machine_type qm(n + 1);

    // схема Уолша-Адамара на n кубитов аргумента
    qm.add_circuit(circuit_hadamard_type(n), qm.range(0, n));

    /*
    // итерации Гровера (вариант 1)
    // обращение знака производится простым путем:
    // вычисление функции, замена знака у амплитуды
    // ее единичного значения, сброс значения обратно
    for (int i = 0; i < iternum; ++i)
    {
        // обращение знака у амплитуды искомого значения
        qm.add_circuit(func, qm.range(0, qm.qubit_number()));
        qm.add_unary(gate_shift_type(0.5), n);
        qm.add_circuit(func, qm.range(0, qm.qubit_number()));
        // инверсия относительно среднего
        qm.add_circuit(average_inversion_type(n), qm.range(0, n));
    };
    //*/

    // итерации Гровера (вариант 2)
    // обращение знака производится оптимизированно:
    // перед итерациями кубит значения функции готовится в
    // состоянии 1/sqrt(2) (|0> - |1>), после зачищается
    // обратно в 0. на каждой итерации f(x) вычисляется лишь
    // единожды, умножая амплитуды аргументов на (-1)^f(x)
    // (принцип аналогичен алгоритму Дойча)
    qm.add_unary(gate_not_type(), n);
    qm.add_unary(gate_hadamard_type(), n);
    for (int i = 0; i < iternum; ++i)
    {
        // обращение знака у амплитуды искомого значения
        qm.add_circuit(func, qm.range(0, qm.qubit_number()));
        // инверсия относительно среднего
        qm.add_circuit(average_inversion_type(n), qm.range(0, n));
    };
    qm.add_unary(gate_hadamard_type(), n);
    qm.add_unary(gate_not_type(), n);

    // в конце снова вставим функцию для оценки результата
    // (результат измерения должен содержать единичное значение)
    qm.add_circuit(func, qm.range(0, qm.qubit_number()));

    // выполнение поиска
    int result;
    do
    {
        // подготовка начального состояния компьютера
        qm.prepare(0);
        // выполнение схемы
        qm.run();
        // измерение и оценка результата
        result = qm.measure();
    } while ((result & (1 << n)) == 0);

    // извлечение искомого аргумента функции
    result &= (1 << n) - 1;

    synprintf(stdout, "result: %d\n", result);
    return 0;
}
