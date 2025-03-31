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

// схема классической однобитной функции (1 из 4 вариантов)
// |x> |0>  -->  |x> |f(x)>
class func_type: public circuit_type
{
public:
    enum type {
        CONSTANT00,
        BALANCED01,
        BALANCED10,
        CONSTANT11
    };
    func_type(type t):
        circuit_type(2)
    {
        if (t == BALANCED01 || t == BALANCED10)
            add_binary(gate_cnot_type(), 1, 0);
        if (t == BALANCED10 || t == CONSTANT11)
            add_unary(gate_not_type(), 1);
    }
};

// схема изменения знака по значению функции
// |x> |0>  -->  (-1)^f(x) |x> |0>
class flipsign_type: public circuit_type
{
public:
    flipsign_type(func_type::type t):
        circuit_type(2)
    {
        add_unary(gate_not_type(), 1);
        add_unary(gate_hadamard_type(), 1);
        add_binary(func_type(t), 0, 1);
        add_unary(gate_hadamard_type(), 1);
        add_unary(gate_not_type(), 1);
    }
};

int main(int argc, char *argv[])
{
    // исследуемая функция
    func_type::type functype = func_type::BALANCED01;

    // двухкубитный квантовый компьютер
    quantum_machine_type qm(2);
    // схема решения задачи Дойча
    qm.add_unary(gate_hadamard_type(), 0);
    qm.add_binary(flipsign_type(functype), 0, 1);
    qm.add_unary(gate_hadamard_type(), 0);

    // подготовка начального состояния компьютера
    qm.prepare(0);
    // выполнение схемы
    qm.run();
    // измерение результата
    int result = qm.measure();
    // интерпретация результата
    bool isbalanced = (result != 0);

    synprintf(stdout, "%s\n", isbalanced ? "balanced" : "constant");
    return 0;
}
