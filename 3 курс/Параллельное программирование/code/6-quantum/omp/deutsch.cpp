/* ------------------------------------------------------------------------- */
/*  ������ ���� �������� ������ ��������� �������� �������,                  */
/*  �������������� �������� ���������� �������:                              */
/*  ������� �.�.                                                             */
/*  ������ ������������� ����������������. - �.: �����-�����, 2012. - 384 �. */
/*  ISBN 978-5-91359-102-9                                                   */
/*                                                                           */
/*  ��� � ���� �������, ����������� � ���� �������� ������ �������������     */
/*  ���� ��� ������������ � ���������� ���������������� ����������           */
/*  ������������ ��������, � ����� ��� ���������� ���������� �������������.  */
/*  ������������� ����� ���� � �������� ������ ��� �������� ��������         */
/*  ���������, ������ ������� ��������� � ����� �������������� ����          */
/*  �� ������ ����� � ���� ������������.                                     */
/*  �������� ������ ��������������� "��� ����", ��� ����� �� �� �� ����      */
/*  ����� ��� ������� �������� ����������� � ������������� ����������.       */
/*                                                                           */
/*  Copyright � 2008-2011 ������� �.�.                                       */
/* ------------------------------------------------------------------------- */


#include "zquantum.hxx"
#include "../../common/synprintf.hxx"


using namespace std;
using namespace z;
using namespace z::quantum;

// ����� ������������ ���������� ������� (1 �� 4 ���������)
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

// ����� ��������� ����� �� �������� �������
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
    // ����������� �������
    func_type::type functype = func_type::BALANCED01;

    // ������������ ��������� ���������
    quantum_machine_type qm(2);
    // ����� ������� ������ �����
    qm.add_unary(gate_hadamard_type(), 0);
    qm.add_binary(flipsign_type(functype), 0, 1);
    qm.add_unary(gate_hadamard_type(), 0);

    // ���������� ���������� ��������� ����������
    qm.prepare(0);
    // ���������� �����
    qm.run();
    // ��������� ����������
    int result = qm.measure();
    // ������������� ����������
    bool isbalanced = (result != 0);

    synprintf(stdout, "%s\n", isbalanced ? "balanced" : "constant");
    return 0;
}
