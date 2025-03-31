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
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

#define EPS             0.01
enum { MASTER_PROC_ID = 0 };

int main(int argc, char *argv[])
{
    int rc = 2;
    // ���������� ��������
    int n;

    // ������������� MPI
    MPI_Init(&argc, &argv);

    // ��������� ���������� �������� �� ��������� ������
    if (argc == 2 && (n = strtol(argv[1], NULL, 0)) > 0)
    {
        int rank, size;
        double span, speed, allspeed;
        int nloc, allnloc, nright;

        // ��������� ���������� ��������� � ������ �������� ��������
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        // ���������� ��������
        span = MPI_Wtime();
        // ... ��������� ��������
        {
            double sum = 0.0;
            for (int i = 0; i < 1000; ++i)
                sum += ((i & 1) ? - 1.0 : 1.0) / ((i << 1) | 1);
        };
        span = MPI_Wtime() - span;
        speed = 1.0 / span;
        // �������� ������������� �����������
        if (MPI_Wtick() / span > EPS)
            fprintf(stderr, "warning: sample iteration is too small\n");

        // ���������� nloc
        MPI_Allreduce(
            &speed, &allspeed, 1, MPI_DOUBLE,
            MPI_SUM, MPI_COMM_WORLD);
        nloc = int(floor(n * (speed / allspeed) + 0.5));
        // ������������� nloc ��� rank == 0, ���� ���������
        MPI_Reduce(
            &nloc, &allnloc, 1, MPI_INT,
            MPI_SUM, MASTER_PROC_ID, MPI_COMM_WORLD);
        if (rank == MASTER_PROC_ID && allnloc != n)
            nloc += n - allnloc;
        // ���������� ������ ������� ��������� ��������
        MPI_Scan(
            &nloc, &nright, 1, MPI_INT,
            MPI_SUM, MPI_COMM_WORLD);

        // ���������� ����� ��
        double locsum = 0.0;
        double sum = 0.0;

        for (int i = nright - nloc; i < nright; ++i)
            locsum += ((i & 1) ? - 1.0 : 1.0) / ((i << 1) | 1);

        MPI_Allreduce(
            &locsum, &sum, 1, MPI_DOUBLE,
            MPI_SUM, MPI_COMM_WORLD);
        sum *= 4.0;

        // ����� ���������� � ����� ��������
        if (rank == MASTER_PROC_ID)
            fprintf(stdout, "%.16f\n", sum);

        // �������� ����������
        rc = 0;
    }
    else
        fprintf(stderr, "error: iternum is not specified\n");

    MPI_Finalize();

    return rc;
}
