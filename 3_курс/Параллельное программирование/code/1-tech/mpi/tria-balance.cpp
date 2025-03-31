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


#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

enum { MASTER_PROC_ID = 0 };

double f(double x, double y)
{
    return sqrt(1.0 - x * x - y * y);
}

int main(int argc, char *argv[])
{
    int rc = 2;
    // ���������� ���������
    int n;
    // ������� ������������ ��������� ������
    double a = 1.0;

    // ������������� MPI
    MPI_Init(&argc, &argv);

    // ��������� ���������� ��������� �� ��������� ������
    if (argc == 2 && (n = strtol(argv[1], NULL, 0)) > 0)
    {
        int rank, size;
        // ��������� ���������� ��������� � ������ �������� ��������
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        double h = a / n, locv = 0.0;

        int nleft, nright;
        // ��������� ����� ������� ���������
        nleft = int(sqrt(1.0 * rank / size) * n);
        // �������� �� ����� ����������, �������� ������
        MPI_Status status;
        MPI_Sendrecv(
            &nleft, 1, MPI_INT, (rank + size - 1) % size, 0,
            &nright, 1, MPI_INT, (rank + 1) % size, 0,
            MPI_COMM_WORLD, &status);
        // ������������ ������ ������� � ���������� ��������
        if (rank == size - 1)
            nright = n;

        for (int i = nleft; i < nright; ++i)
        {
            for (int j = 0; j <= i; ++j)
            {
                double x = (n - i - 0.5) * h;
                double y = (j + 0.5) * h;
                double s = h * h * ((j < i) ? 1.0 : 0.5);
                locv += f(x, y) * s;
            };
        };

        // ���������� �����
        double v = 0.0;
        MPI_Reduce(
            &locv, &v, 1, MPI_DOUBLE,
            MPI_SUM, MASTER_PROC_ID, MPI_COMM_WORLD);

        // ����� ���������� � ����� ��������
        if (rank == MASTER_PROC_ID)
            fprintf(stdout, "%.16f\n", v);

        // �������� ����������
        rc = 0;
    }
    else
        fprintf(stderr, "error: iternum is not specified\n");

    MPI_Finalize();

    return rc;
}
