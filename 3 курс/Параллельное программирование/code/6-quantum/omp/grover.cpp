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

// ����� ������������ ������� �� ��������� � 1 ���
// |x> |0>  -->  |x> |f(x)>
// � ������ ������� ������� ����������� ��� ��������,
// ���� �������� ����� ��������� ����� num
class func_type: public circuit_type
{
public:
    func_type(int n, int num):
        circuit_type(n + 1)
    {
        assert(n > 0 && num >= 0 && num < (1 << n));
        // ��������� �������� �����, ��������������� ����� ���������
        for (int i = 0; i < n; ++i)
        {
            if ((num & (1 << i)) == 0)
                add_unary(gate_not_type(), i);
        };

        // �������� �������� ������� � ������, ���� ��� ����� = 1
        pinlist_type argpin = range(0, n), pin = range(n, qubit_number());
        pin.insert(pin.end(), argpin.begin(), argpin.end());
        add_circuit(circuit_ccccnot_type(qubit_number()), pin);

        // ������� ����� ��������� � �������������� ��������
        for (int i = 0; i < n; ++i)
        {
            if ((num & (1 << i)) == 0)
                add_unary(gate_not_type(), i);
        };
    }
};

// ����� ����������� ������������ ������� ����� Z
class controlled_zpauli_type: public circuit_type
{
public:
    controlled_zpauli_type(int n):
        circuit_type(n)
    {
        // ���� ����� �������� ����� ������ ����
        if (n > 1)
        {
            // ���������� ���������� ����� - ����� �����������
            controlled_zpauli_type lower(n - 1);
            add_circuit(
                circuit_controlled_type(lower),
                range(0, qubit_number()));
        }
        else
            // � ��������� ������ ����������� ����� - Z-�������
            add_unary(gate_shift_type(0.5), 0);
    }
};

// ����� �������� �������� ������������ �� �������� ��������
class average_inversion_type: public circuit_type
{
public:
    average_inversion_type(int n):
        circuit_type(n)
    {
        pinlist_type pin = range(0, qubit_number());
        // ��������������� �������������� �����-�������
        add_circuit(circuit_hadamard_type(qubit_number()), pin);

        // ��������� �������� ���� �������
        for (int i = 0; i < qubit_number(); ++i)
            add_unary(gate_not_type(), i);

        // ����������� ����������� Z-�������
        // ����� �������� �� ����� �����, �� ������ �� �������
        add_circuit(controlled_zpauli_type(qubit_number()), pin);

        // �������� �������� �������
        for (int i = 0; i < qubit_number(); ++i)
            add_unary(gate_not_type(), i);

        // ��� ���� "�������" �������� ������������ ��������,
        // ���� � ������ ������ ��� �� �����, ��������� �����
        // ����������� ����� ������� ������� ���������� ���������
        /*
        // ��������� ����� � ���� ��������
        add_unary(gate_shift_type(0.5), 0);
        add_unary(gate_not_type(), 0);
        add_unary(gate_shift_type(0.5), 0);
        add_unary(gate_not_type(), 0);
        //*/

        // �������������� �������������� �����-�������
        add_circuit(circuit_hadamard_type(qubit_number()), pin);
    }
};

int main(int argc, char *argv[])
{
    // ����� ��������� �������
    const int n = 12;
    // ���������� �������� �������
    const int iternum = int(0.25 * M_PI * sqrt(1.0 * (1 << n)) + 0.5);

    // ����������� �������� ������� �� n �������
    func_type func(n, 219);

    // ��������� ���������
    quantum_machine_type qm(n + 1);

    // ����� �����-������� �� n ������� ���������
    qm.add_circuit(circuit_hadamard_type(n), qm.range(0, n));

    /*
    // �������� ������� (������� 1)
    // ��������� ����� ������������ ������� �����:
    // ���������� �������, ������ ����� � ���������
    // �� ���������� ��������, ����� �������� �������
    for (int i = 0; i < iternum; ++i)
    {
        // ��������� ����� � ��������� �������� ��������
        qm.add_circuit(func, qm.range(0, qm.qubit_number()));
        qm.add_unary(gate_shift_type(0.5), n);
        qm.add_circuit(func, qm.range(0, qm.qubit_number()));
        // �������� ������������ ��������
        qm.add_circuit(average_inversion_type(n), qm.range(0, n));
    };
    //*/

    // �������� ������� (������� 2)
    // ��������� ����� ������������ ���������������:
    // ����� ���������� ����� �������� ������� ��������� �
    // ��������� 1/sqrt(2) (|0> - |1>), ����� ����������
    // ������� � 0. �� ������ �������� f(x) ����������� ����
    // ��������, ������� ��������� ���������� �� (-1)^f(x)
    // (������� ���������� ��������� �����)
    qm.add_unary(gate_not_type(), n);
    qm.add_unary(gate_hadamard_type(), n);
    for (int i = 0; i < iternum; ++i)
    {
        // ��������� ����� � ��������� �������� ��������
        qm.add_circuit(func, qm.range(0, qm.qubit_number()));
        // �������� ������������ ��������
        qm.add_circuit(average_inversion_type(n), qm.range(0, n));
    };
    qm.add_unary(gate_hadamard_type(), n);
    qm.add_unary(gate_not_type(), n);

    // � ����� ����� ������� ������� ��� ������ ����������
    // (��������� ��������� ������ ��������� ��������� ��������)
    qm.add_circuit(func, qm.range(0, qm.qubit_number()));

    // ���������� ������
    int result;
    do
    {
        // ���������� ���������� ��������� ����������
        qm.prepare(0);
        // ���������� �����
        qm.run();
        // ��������� � ������ ����������
        result = qm.measure();
    } while ((result & (1 << n)) == 0);

    // ���������� �������� ��������� �������
    result &= (1 << n) - 1;

    synprintf(stdout, "result: %d\n", result);
    return 0;
}
