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

// извлечение периода из результата измерения
// если не найдено подходящее число, возвращается 0
int extract(int num, int denom, int limit)
{
    assert(num >= 0 && denom > 0 && limit > 0);

    int rc = 0;
    // начальные значения для рекуррентных соотношений
    int ppre = 1, pcur = num / denom;
    int qpre = 0, qcur = 1;
    // раскладываем число в цепную дробь
    int rpre = denom, rcur = num % denom;
    while (rcur != 0)
    {
        // числитель очередной подходящей дроби
        int pnxt = (rpre / rcur) * pcur + ppre;
        ppre = pcur;
        pcur = pnxt;
        // знаменатель очередной подходящей дроби
        int qnxt = (rpre / rcur) * qcur + qpre;
        qpre = qcur;
        qcur = qnxt;

        // проверяем выход за границы
        if (qcur >= limit)
            break;
        else
        {
            // проверяем удовлетворение условию
            if (2 * abs(num * qcur - denom * pcur) <= qcur)
                rc = qcur;
            // очередной остаток (алгоритм Евклида)
            int rnxt = rpre % rcur;
            rpre = rcur;
            rcur = rnxt;
        };
    };
    return rc;
}

// поиск одного из множителей числа (алгоритм Шора)
int factorize(int number)
{
    assert(number > 2);

    int numlength, arglength, base, order, factor;
    numlength = int(ceil(log(1.0 * number) / log(2.0)));
    arglength = int(ceil(2.0 * log(1.0 * number) / log(2.0)));

    for (;;)
    {
        quantum_machine_type qm(arglength + 3 * numlength);

        // случайный выбор основания из диапазона [2, number)
        base = int(random<double>() * (number - 2)) + 2;

        // если числа не взаимно просты, возвращаем НОД
        factor = gcd(base, number);
        if (factor > 1)
            break;

        // составление квантовой схемы
        qm.add_circuit(
            circuit_hadamard_type(arglength),
            qm.range(0, arglength));
        qm.add_circuit(
            circuit_powmod_type(arglength, numlength, base, number),
            qm.range(0, qm.qubit_number()));
        qm.add_circuit(
            circuit_fourier_type(arglength),
            qm.range(0, arglength));

        // выполнение и измерение
        qm.prepare(0);
        qm.run();
        int result = qm.measure() & ((1 << arglength) - 1);

        // извлечение периода из результата измерения
        order = extract(result, 1 << arglength, number);

        // если, действительно, получен период
        // и если он четный
        if (order > 0 &&
            powmod(base, order, number) == 1 &&
            (order & 1) == 0)
        {
            // вычисление делителя исходного числа
            factor = gcd(
                powmod(base, order / 2, number) + number - 1,
                number);
            // если нашли нетривиальный, заканчиваем поиск
            if (factor > 1)
                break;
        };
    };
    return factor;
}


int main(int argc, char *argv[])
{
    int factor = factorize(15);
    synprintf(stdout, "factor: %d\n", factor);
    return 0;
}

