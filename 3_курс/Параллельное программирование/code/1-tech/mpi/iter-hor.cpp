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

#include "../../common/zmatrix.hxx"

using namespace std;
using namespace z;

enum { NUM_ITER = 20, MASTER_PROC_ID = 0 };

int main(int argc, char *argv[])
{
    int rc = 2;

    MPI_Init(&argc, &argv);

    // ������� ������������� ��������� u(x) - int(0, 1, x * y * u(y), dy) = 2/3 * x
    // ������ ���� u(x) = x, x in [0, 1)
    const int n = 5000;

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (n % size == 0)
    {
        int nloc = n / size;

        // ������� (��������� �����)
        matrix_type<double> aloc(nloc, n);
        for (int i = 0; i < aloc.vsize(); ++i)
            for (int j = 0; j < aloc.hsize(); ++j)
                aloc(i, j) = ((i + rank * nloc + 0.5) / n) * ((j + 0.5) / n) / n;

        // ������ ��������� �������������
        vector_type<double> f(n);
        for (int i = 0; i < f.vsize(); ++i)
            f(i) = 2.0 / 3.0 * (i + 0.5) / n;

        // ������ �������� �����������
        vector_type<double> u = f;
        // ���������� ���������� ������� ��������
        for (int i = 0; i < NUM_ITER; ++i)
        {
            vector_type<double> uloc = aloc * u;
            MPI_Allgather(
                &uloc(0), uloc.vsize(), MPI_DOUBLE,
                &u(0), nloc, MPI_DOUBLE,
                MPI_COMM_WORLD);
            u += f;
        };

        // ����� �����������
        if (rank == MASTER_PROC_ID)
        {
            for (int i = 0; i < u.vsize(); ++i)
                fprintf(stdout, "%f\n", u(i));
        };

        // �������� ����������
        rc = 0;
    }
    else
        if (rank == MASTER_PROC_ID)
            fprintf(stderr, "error: matrix size (%d) isn't divisible by procnum\n", n);

    MPI_Finalize();
    return rc;
}
