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
#define _USE_MATH_DEFINES
#include <cmath>

#include "../../common/zmatrix.hxx"

using namespace std;
using namespace z;

enum { MASTER_PROC_ID = 0 };

int main(int argc, char *argv[])
{
    int rc = 2;

    MPI_Init(&argc, &argv);

    const int n = 256;

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (n % size == 0)
    {
        int nloc = n / size;
        // правая матрица - дискретное преобразование Хартли (локальная часть)
        matrix_type<double> rloc(n, nloc);
        for (int i = 0; i < rloc.vsize(); ++i)
            for (int j = 0; j < rloc.hsize(); ++j)
                rloc(i, j) = M_SQRT2 * cos(2.0 * M_PI / n * i * (j + rank * nloc) - 0.25 * M_PI);
        // левая матрица - обратное ДПХ (локальная часть)
        matrix_type<double> lloc(nloc, n);
        for (int i = 0; i < lloc.vsize(); ++i)
            for (int j = 0; j < lloc.hsize(); ++j)
                lloc(i, j) = rloc(j, i) / n;
        // матрица - результат умножения (локальная часть)
        matrix_type<double> mloc(n, nloc);

        // вычисление рангов следующего и предыдущего процессов
        int next = (rank + 1) % size;
        int prev = (rank + size - 1) % size;
        // выполнение умножения R на L слева (M = L * R)
        for (int s = 0; s < size; ++s)
        {
            // умножить вертикальный rloc на горизонтальный lloc
            matrix_type<double> res = lloc * rloc;
            // сохранить квадратный блок результата в mloc
            copy(
                &res(0, 0),
                &res(0, 0) + res.vsize() * res.hsize(),
                &mloc(((rank + s) % size) * nloc, 0));
            // сдвинуть lloc между процессами
            MPI_Status status;
            MPI_Sendrecv_replace(
                &lloc(0, 0), lloc.vsize() * lloc.hsize(), MPI_DOUBLE,
                prev, (rank + s) % size,
                next, (rank + s + 1) % size,
                MPI_COMM_WORLD, &status);
        };

        // вывод матрицы результата частями, должна быть единичная
        for (int s = 0; s < size; ++s)
        {
            MPI_Barrier(MPI_COMM_WORLD);
            if (rank == s)
            {
                fprintf(stdout, "%d\n", s);
                for (int i = 0; i < mloc.vsize(); ++i)
                {
                    for (int j = 0; j < mloc.hsize(); ++j)
                        fprintf(stdout, "%f ", mloc(i, j));
                    fprintf(stdout, "\n");
                };
            };
            fflush(stdout);
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
