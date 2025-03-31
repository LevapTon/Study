using System;

namespace Классы
{
    class Matrix
    {
        Random r = new Random();
        double[,] matrix;
        int row, col;

        public int Row  // Свойтво row
        {
            get
            {
                return row;  // Возвращает значение
            }
            set
            {
                if (value > 0 & value < 6)  // Если передаваемое значение входит в диапозон
                {
                    row = value;
                }
                else
                {
                    row = 3;  // Установка базового значения в поле
                }
            }
        }

        public int Col  // Свойтво col
        {
            get
            {
                return col;  // Возвращает значение
            }
            set
            {
                if (value > 0 & value < 6)  // Если передаваемое значение входит в диапозон
                {
                    col = value;
                }
                else
                {
                    col = 3;  // Установка базового значения в поле
                }
            }
        }

        public Matrix()  // Создание квадр. матр. 3*3
        {
            this.Row = 3;
            this.Col = 3;
            matrix = new double[this.Row, this.Col];
            for (int i = 0; i < this.Row; i++)
            {
                for (int j = 0; j < this.Col; j++)
                {
                    matrix[i, j] = r.Next(10);
                }
            }
        }

        public Matrix(int x)  // Создание квадр. матр. n*n
        {
            this.Row = x;
            this.Col = x;
            matrix = new double[this.Row, this.Row];
            for (int i = 0; i < this.Col; i++)
            {
                for (int j = 0; j < this.Row; j++)
                {
                    matrix[i, j] = r.Next(10);
                }
            }
        }

        public Matrix(int x, int y)  // Создание не квадр. матр. n*m
        {
            this.Row = x;
            this.Col = y;
            matrix = new double[this.Row, this.Row];
            for (int i = 0; i < this.Col; i++)
            {
                for (int j = 0; j < this.Row; j++)
                {
                    matrix[i, j] = r.Next(10);
                }
            }
        }

        public double this[int x, int y]  // Перегрузка индексирования матриц
        {
            get 
            {
                return matrix[x, y];
            }
            set 
            {
                matrix[x, y] = value;
            }
        }

        public void ManualFilling()  // Заполнение матрицы пользователем
        {
            for (int i = 0; i < this.Row; i++)
            {
                for (int j = 0; j < this.Col; j++)
                {
                    try
                    {
                        Console.Write("Элемент [{0},{1}]: ", i, j);
                        this[i, j] = Convert.ToDouble(Console.ReadLine());
                    }
                    catch
                    {
                        this[i, j] = 0;
                    }
                }
            }
            Console.WriteLine();
        }

        public void PrintMatrix()  // Вывод матрицы на экран
        {
            for (int i = 0; i < this.Row; i++)
            {
                for (int j = 0; j < this.Col; j++)
                {
                    Console.Write(this[i, j] + " ");
                }
                Console.WriteLine();
            }
        }

        public bool IsSquare()  // Проверка на квадратную матрицу
        {
            if (this.Row == this.Col) 
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public bool IsDiagonal()  // Проверка на диагональную матрицу
        {
            if (this.IsSquare())
            {
                bool flag = true;
                for (int i = 0; i < this.Row; i++)
                {
                    for (int j = 0; j < this.Col; j++)
                    {
                        if (i == j) continue;
                        if (this[i,j] != 0)
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (!flag) break;
                }
                if (flag) return true;
                else return false;
            }
            else return false;
        }

        public bool IsZero()  // Проверка на нулевуюую матрицу
        {
            if (this.IsSquare())
            {
                bool flag = true;
                for (int i = 0; i < this.Row; i++)
                {
                    for (int j = 0; j < this.Col; j++)
                    {
                        if (this[i,j] != 0)
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (!flag) break;
                }
                if (flag) return true;
                else return false;
            }
            else return false;
        }

        public bool IsSingle()  // Проверка на единичную матрицу
        {
            if (this.IsSquare())
            {
                bool flag = true;
                for (int i = 0; i < this.Row; i++)
                {
                    for (int j = 0; j < this.Col; j++)
                    {
                        if (i == j & this[i,j] == 1) continue;
                        if (this[i,j] != 0)
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (!flag) break;
                }
                if (flag) return true;
                else return false;
            }
            else return false;
        }

        public bool IsSymmetrical()  // Проверка на симметричную матрицу
        {
            if (this.IsSquare())
            {
                bool flag = true;
                for (int i = 0; i < this.Row; i++)
                {
                    for (int j = 0; j < this.Col; j++)
                    {
                        if (i == j) continue;
                        if (this[i,j] != this[j,i])
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (!flag) break;
                }
                if (flag) return true;
                else return false;
            }
            else return false;
        }

        public bool IsUpperTriangular()  // Проверка на верхнюю треугольную матрицу
        {
            if (this.IsSquare())
            {
                bool flag = true;
                for (int i = 0; i < this.Row; i++)
                {
                    for (int j = 0; j < this.Col; j++)
                    {
                        if (j < i & this[i,j] != 0)
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (!flag) break;
                }
                if (flag) return true;
                else return false;
            }
            else return false;
        }

        public bool IsLowerTriangular()  // Проверка на нижнюю треугольную матрицу
        {
            if (this.IsSquare())
            {
                bool flag = true;
                for (int i = 0; i < this.Row; i++)
                {
                    for (int j = 0; j < this.Col; j++)
                    {
                        if (j > i & this[i,j] != 0)
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (!flag) break;
                }
                if (flag) return true;
                else return false;
            }
            else return false;
        }

        public static bool operator == (Matrix m1, Matrix m2)  // Проверка на равенство матриц
        {
            if (m1.Row == m2.Row && m1.Col == m2.Col)
            {
                bool flag = true;
                for (int i = 0; i < m1.Row; i++)
                {
                    for (int j = 0; j < m1.Col; j++)
                    {
                        if (m1[i, j] != m2[i, j])
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (!flag) break;
                }
                if (flag) return true;
                else return false;
            }
            else return false;
        }

        public static bool operator != (Matrix m1, Matrix m2)  // Проверка на не равенство матриц
        {
            if (m1.Row == m2.Row && m1.Col == m2.Col)
            {
                bool flag = true;
                for (int i = 0; i < m1.Row; i++)
                {
                    for (int j = 0; j < m1.Col; j++)
                    {
                        if (m1[i, j] == m2[i, j])
                        {
                            flag = false;
                            break;
                        }
                    }
                    if (!flag) break;
                }
                if (flag) return true;
                else return false;
            }
            else return false;
        }
    }
}
