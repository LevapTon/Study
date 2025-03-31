using System;

namespace Классы
{
    class Program
    {
        static void Main(string[] args)
        {
            int x, y;
            try
            {
                Console.Write("Введите число строк в отрезке [1;5]: ");
                x = Convert.ToInt32(Console.ReadLine());
            }
            catch
            {
                x = 3;
            }
            try
            {
                Console.Write("Введите число столбцов в отрезке [1;5]: ");
                y = Convert.ToInt32(Console.ReadLine());
            }
            catch
            {
                y = 3;
            }

            Matrix first = new();
            Matrix second = new Matrix();

            if (x == y)
                {
                    first = new Matrix(x);
                }
            else
                {
                    first = new Matrix(x, y);
                }

            Console.Write("Заполнить созданную матрицу автоматически? (y/n) ");
            string answer = Convert.ToString(Console.ReadLine());
            if (answer == "n") first.ManualFilling();
            
            Console.WriteLine("Первая матрица: ");
            first.PrintMatrix();
            Console.WriteLine("Вторая матрица: ");
            second.PrintMatrix();
            Console.WriteLine("Проверка вида первой матрицы: ");
            Console.Write("Матрица квадратная: ");
            Console.WriteLine(first.IsSquare());
            Console.Write("Матрица диагональная: ");
            Console.WriteLine(first.IsDiagonal());
            Console.Write("Матрица нулевая: ");
            Console.WriteLine(first.IsZero());
            Console.Write("Матрица единичная: ");
            Console.WriteLine(first.IsSingle());
            Console.Write("Матрица симметричная: ");
            Console.WriteLine(first.IsSymmetrical());
            Console.Write("Матрица верхняя треугольная: ");
            Console.WriteLine(first.IsUpperTriangular());
            Console.Write("Матрица нижняя треугольная: ");
            Console.WriteLine(first.IsLowerTriangular());
            Console.Write("Матрицы равны: ");
            Console.WriteLine(first == second);
        }
    }
}
