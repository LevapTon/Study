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
        // задаем размеры двумерной сетки
        int dims[2] = {0}, &vsize = dims[0], &hsize = dims[1];
        MPI_Dims_create(size, 2, dims);

        // локальные размерности по вертикали и горизонтали
        int vnloc = n / vsize;
        int hnloc = n / hsize;
        // вертикальный и горизонтальный номера текущего процесса
        int vrank = rank / hsize;
        int hrank = rank % hsize;

        // коммуникаторы строк процессов и столбцов процессов
        MPI_Comm vcomm, hcomm;
        MPI_Comm_split(MPI_COMM_WORLD, hrank, rank, &vcomm);
        MPI_Comm_split(MPI_COMM_WORLD, vrank, rank, &hcomm);

        // матрица (локальная часть)
        matrix_type<double> aloc(vnloc, hnloc);
        for (int i = 0; i < aloc.vsize(); ++i)
            for (int j = 0; j < aloc.hsize(); ++j)
                aloc(i, j) = ((i + vrank * vnloc + 0.5) / n) * ((j + hrank * hnloc + 0.5) / n) / n;

        // вектор свободных коэффициентов (локальная часть)
        vector_type<double> floc(hnloc);
        for (int i = 0; i < floc.vsize(); ++i)
            floc(i) = 2.0 / 3.0 * (i + hrank * hnloc + 0.5) / n;

        // инициализация размеров областей для рассеивания
        vector_type<int> hnlocs(hsize);
        for (int i = 0; i < hnlocs.vsize(); ++i)
            hnlocs(i) = hnloc;

        // вектор текущего приближения (локальная часть)
        vector_type<double> uloc = floc;
        // выполнение нескольких простых итераций
        for (int i = 0; i < NUM_ITER; ++i)
        {
            vector_type<double> upartloc = aloc * uloc;
            vector_type<double> upart(n);
            MPI_Allgather(
                &upartloc(0), upartloc.vsize(), MPI_DOUBLE,
                &upart(0), vnloc, MPI_DOUBLE,
                vcomm);
            MPI_Reduce_scatter(
                &upart(0), &uloc(0), &hnlocs(0), MPI_DOUBLE,
                MPI_SUM, hcomm);
            uloc += floc;
        };

        // вывод результатов
        vector_type<double> u(n);
        // собираем во всех процессах левого столбца
        MPI_Gather(
            &uloc(0), uloc.vsize(), MPI_DOUBLE,
            &u(0), hnloc, MPI_DOUBLE,
            MASTER_PROC_ID, hcomm);
        // выводим только из верхнего
        if (rank == MASTER_PROC_ID)
        {
            for (int i = 0; i < u.vsize(); ++i)
                fprintf(stdout, "%f\n", u(i));
        };

        // уничтожение созданных коммуникаторов
        MPI_Comm_free(&hcomm);
        MPI_Comm_free(&vcomm);

        // успешное завершение
        rc = 0;
    }
    else
        if (rank == MASTER_PROC_ID)
            fprintf(stderr, "error: matrix size (%d) isn't divisible by procnum\n", n);

    MPI_Finalize();
    return rc;
}
