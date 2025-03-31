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

    // решение интегрального уравнения u(x) - int(0, 1, x * y * u(y), dy) = 2/3 * x
    // должно быть u(x) = x, x in [0, 1)
    const int n = 5000;

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (n % size == 0)
    {
        int nloc = n / size;

        // матрица (локальная часть)
        matrix_type<double> aloc(n, nloc);
        for (int i = 0; i < aloc.vsize(); ++i)
            for (int j = 0; j < aloc.hsize(); ++j)
                aloc(i, j) = ((i + 0.5) / n) * ((j + rank * nloc + 0.5) / n) / n;

        // вектор свободных коэффициентов (локальная часть)
        vector_type<double> floc(nloc);
        for (int i = 0; i < floc.vsize(); ++i)
            floc(i) = 2.0 / 3.0 * (i + rank * nloc + 0.5) / n;

        // инициализация размеров областей для рассеивания
        vector_type<int> nlocs(size);
        for (int i = 0; i < nlocs.vsize(); ++i)
            nlocs(i) = nloc;

        // вектор текущего приближения (локальная часть)
        vector_type<double> uloc = floc;
        // выполнение нескольких простых итераций
        for (int i = 0; i < NUM_ITER; ++i)
        {
            vector_type<double> upart = aloc * uloc;
            MPI_Reduce_scatter(
                &upart(0), &uloc(0), &nlocs(0), MPI_DOUBLE,
                MPI_SUM, MPI_COMM_WORLD);
            uloc += floc;
        };

        // вывод результатов
        vector_type<double> u(n);
        MPI_Gather(
            &uloc(0), uloc.vsize(), MPI_DOUBLE,
            &u(0), nloc, MPI_DOUBLE,
            MASTER_PROC_ID, MPI_COMM_WORLD);
        if (rank == MASTER_PROC_ID)
        {
            for (int i = 0; i < u.vsize(); ++i)
                fprintf(stdout, "%f\n", u(i));
        };

        // успешное завершение
        rc = 0;
    }
    else
        if (rank == MASTER_PROC_ID)
            fprintf(stderr, "error: matrix size (%d) isn't divisible by procnum\n", n);

    MPI_Finalize();
    return rc;
}
