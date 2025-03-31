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

// ���������� ������� �� ���������� ���������
// ���� �� ������� ���������� �����, ������������ 0
int extract(int num, int denom, int limit)
{
    assert(num >= 0 && denom > 0 && limit > 0);

    int rc = 0;
    // ��������� �������� ��� ������������ �����������
    int ppre = 1, pcur = num / denom;
    int qpre = 0, qcur = 1;
    // ������������ ����� � ������ �����
    int rpre = denom, rcur = num % denom;
    while (rcur != 0)
    {
        // ��������� ��������� ���������� �����
        int pnxt = (rpre / rcur) * pcur + ppre;
        ppre = pcur;
        pcur = pnxt;
        // ����������� ��������� ���������� �����
        int qnxt = (rpre / rcur) * qcur + qpre;
        qpre = qcur;
        qcur = qnxt;

        // ��������� ����� �� �������
        if (qcur >= limit)
            break;
        else
        {
            // ��������� �������������� �������
            if (2 * abs(num * qcur - denom * pcur) <= qcur)
                rc = qcur;
            // ��������� ������� (�������� �������)
            int rnxt = rpre % rcur;
            rpre = rcur;
            rcur = rnxt;
        };
    };
    return rc;
}

// ����� ������ �� ���������� ����� (�������� ����)
int factorize(int number)
{
    assert(number > 2);

    int numlength, arglength, base, order, factor;
    numlength = int(ceil(log(1.0 * number) / log(2.0)));
    arglength = int(ceil(2.0 * log(1.0 * number) / log(2.0)));

    for (;;)
    {
        quantum_machine_type qm(arglength + 3 * numlength);

        // ��������� ����� ��������� �� ��������� [2, number)
        base = int(random<double>() * (number - 2)) + 2;

        // ���� ����� �� ������� ������, ���������� ���
        factor = gcd(base, number);
        if (factor > 1)
            break;

        // ����������� ��������� �����
        qm.add_circuit(
            circuit_hadamard_type(arglength),
            qm.range(0, arglength));
        qm.add_circuit(
            circuit_powmod_type(arglength, numlength, base, number),
            qm.range(0, qm.qubit_number()));
        qm.add_circuit(
            circuit_fourier_type(arglength),
            qm.range(0, arglength));

        // ���������� � ���������
        qm.prepare(0);
        qm.run();
        int result = qm.measure() & ((1 << arglength) - 1);

        // ���������� ������� �� ���������� ���������
        order = extract(result, 1 << arglength, number);

        // ����, �������������, ������� ������
        // � ���� �� ������
        if (order > 0 &&
            powmod(base, order, number) == 1 &&
            (order & 1) == 0)
        {
            // ���������� �������� ��������� �����
            factor = gcd(
                powmod(base, order / 2, number) + number - 1,
                number);
            // ���� ����� �������������, ����������� �����
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

