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

    int sqrtsize = int(floor(sqrt(1.0 * size) + 0.5));
    if (sqrtsize * sqrtsize == size && n % sqrtsize == 0)
    {
        int nloc = n / sqrtsize;
        const int ndims = 2;

        MPI_Comm cart, subcart;
        // создаем коммуникатор с двумерной декартовой топологией
        int dim[ndims] = {sqrtsize, sqrtsize};
        int period[ndims] = {true, true};
        MPI_Cart_create(MPI_COMM_WORLD, ndims, dim, period, false, &cart);
        // получаем двумерные координаты нашего процесса
        int coords[ndims], &icoord = coords[0], &jcoord = coords[1];
        MPI_Cart_coords(cart, rank, ndims, coords);
        // разбиваем декартов коммуникатор по строкам
        int split[ndims] = {false, true};
        MPI_Cart_sub(cart, split, &subcart);
        // получим группы (они нужны для трансляции рангов)
        MPI_Group gcart, gsubcart;
        MPI_Comm_group(cart, &gcart);
        MPI_Comm_group(subcart, &gsubcart);

        // правая матрица - дискретное преобразование Хартли (локальная часть)
        matrix_type<double> rloc(nloc, nloc);
        for (int i = 0; i < rloc.vsize(); ++i)
            for (int j = 0; j < rloc.hsize(); ++j)
                rloc(i, j) = M_SQRT2 * cos(2.0 * M_PI / n * (i + icoord * nloc) * (j + jcoord * nloc) - 0.25 * M_PI);
        // левая матрица - обратное ДПХ (локальная часть)
        matrix_type<double> lloc(nloc, nloc);
        for (int i = 0; i < lloc.vsize(); ++i)
            for (int j = 0; j < lloc.hsize(); ++j)
                lloc(i, j) = rloc(i, j) / n;
        // матрица - результат умножения (локальная часть)
        // конструктор инициализирует нулевыми значениями
        matrix_type<double> mloc(nloc, nloc);

        // выполнение умножения R на L слева (M = L * R)
        for (int s = 0; s < sqrtsize; ++s)
        {
            // вычислим ранг процесса-источника для рассылки
            // блока левой матрицы среди строки процессов
            int src, dst, subroot;
            MPI_Cart_shift(cart, 1, jcoord - (icoord + s), &src, &dst);
            MPI_Group_translate_ranks(gcart, 1, &src, gsubcart, &subroot);

            // разошлем блок левой матрицы всем процессам строки
            matrix_type<double> lcopy(lloc.vsize(), lloc.hsize());
            if (rank == src)
                lcopy = lloc;
            MPI_Bcast(
                &lcopy(0, 0),
                lloc.vsize() * lloc.hsize(), MPI_DOUBLE,
                subroot,
                subcart);

            // перемножим локальные блоки левой и правой матрицы
            matrix_type<double> res = lcopy * rloc;
            // сохраним квадратный блок результата
            mloc += res;

            // вычислим следующий и предыдущий ранги
            // для сдвига по столбцу процессов
            MPI_Cart_shift(cart, 0, -1, &src, &dst);
            // сдвинем rloc между процессами в столбце
            MPI_Status status;
            MPI_Sendrecv_replace(
                &rloc(0, 0),
                rloc.vsize() * rloc.hsize(), MPI_DOUBLE,
                dst, 0,
                src, 0,
                cart, &status);
        };

        MPI_Group_free(&gsubcart);
        MPI_Group_free(&gcart);
        MPI_Comm_free(&subcart);
        MPI_Comm_free(&cart);

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
            fprintf(stderr, "error: matrix size (%d) isn't divisible by sqrt(procnum)\n", n);

    MPI_Finalize();
    return rc;
}
