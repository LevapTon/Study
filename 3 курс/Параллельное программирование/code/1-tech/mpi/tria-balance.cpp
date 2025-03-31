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
    // количество разбиений
    int n;
    // сторона треугольного основания призмы
    double a = 1.0;

    // инициализация MPI
    MPI_Init(&argc, &argv);

    // получение количества разбиений из командной строки
    if (argc == 2 && (n = strtol(argv[1], NULL, 0)) > 0)
    {
        int rank, size;
        // получение количества процессов и номера текущего процесса
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        double h = a / n, locv = 0.0;

        int nleft, nright;
        // вычисляем левую границу интервала
        nleft = int(sqrt(1.0 * rank / size) * n);
        // сдвигаем ее между процессами, получаем правую
        MPI_Status status;
        MPI_Sendrecv(
            &nleft, 1, MPI_INT, (rank + size - 1) % size, 0,
            &nright, 1, MPI_INT, (rank + 1) % size, 0,
            MPI_COMM_WORLD, &status);
        // корректируем правую границу у последнего процесса
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

        // объединяем суммы
        double v = 0.0;
        MPI_Reduce(
            &locv, &v, 1, MPI_DOUBLE,
            MPI_SUM, MASTER_PROC_ID, MPI_COMM_WORLD);

        // вывод результата в одном процессе
        if (rank == MASTER_PROC_ID)
            fprintf(stdout, "%.16f\n", v);

        // успешное завершение
        rc = 0;
    }
    else
        fprintf(stderr, "error: iternum is not specified\n");

    MPI_Finalize();

    return rc;
}
