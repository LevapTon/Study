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
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

#define EPS             0.01
enum { MASTER_PROC_ID = 0 };

int main(int argc, char *argv[])
{
    int rc = 2;
    // количество итераций
    int n;

    // инициализация MPI
    MPI_Init(&argc, &argv);

    // получение количества итераций из командной строки
    if (argc == 2 && (n = strtol(argv[1], NULL, 0)) > 0)
    {
        int rank, size;
        double span, speed, allspeed;
        int nloc, allnloc, nright;

        // получение количества процессов и номера текущего процесса
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        // вычисление скорости
        span = MPI_Wtime();
        // ... оценочная итерация
        {
            double sum = 0.0;
            for (int i = 0; i < 1000; ++i)
                sum += ((i & 1) ? - 1.0 : 1.0) / ((i << 1) | 1);
        };
        span = MPI_Wtime() - span;
        speed = 1.0 / span;
        // проверка относительной погрешности
        if (MPI_Wtick() / span > EPS)
            fprintf(stderr, "warning: sample iteration is too small\n");

        // вычисление nloc
        MPI_Allreduce(
            &speed, &allspeed, 1, MPI_DOUBLE,
            MPI_SUM, MPI_COMM_WORLD);
        nloc = int(floor(n * (speed / allspeed) + 0.5));
        // корректировка nloc для rank == 0, если требуется
        MPI_Reduce(
            &nloc, &allnloc, 1, MPI_INT,
            MPI_SUM, MASTER_PROC_ID, MPI_COMM_WORLD);
        if (rank == MASTER_PROC_ID && allnloc != n)
            nloc += n - allnloc;
        // вычисление правой границы интервала индексов
        MPI_Scan(
            &nloc, &nright, 1, MPI_INT,
            MPI_SUM, MPI_COMM_WORLD);

        // вычисление числа пи
        double locsum = 0.0;
        double sum = 0.0;

        for (int i = nright - nloc; i < nright; ++i)
            locsum += ((i & 1) ? - 1.0 : 1.0) / ((i << 1) | 1);

        MPI_Allreduce(
            &locsum, &sum, 1, MPI_DOUBLE,
            MPI_SUM, MPI_COMM_WORLD);
        sum *= 4.0;

        // вывод результата в одном процессе
        if (rank == MASTER_PROC_ID)
            fprintf(stdout, "%.16f\n", sum);

        // успешное завершение
        rc = 0;
    }
    else
        fprintf(stderr, "error: iternum is not specified\n");

    MPI_Finalize();

    return rc;
}
