using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace LabRab7_2
{
    public partial class Form1 : Form
    {
        // массив Kurs будет содержать курсы доллара:
        private double[] Kurs;
        // Объявляем поток для чтения:
        public StreamReader potok;
        string path = @"D:\Учеба\2 курс\Разработка ПП\ЛабРаб7\LabRab7-2\LabRab7-2\bin\Debug\Данные.txt";
        public Form1()
        {
            InitializeComponent();
            this.BackColor = System.Drawing.Color.White;
            try
            { //Создаем объект stream типа FileStream:
            FileStream stream = new FileStream(path, FileMode.Open,FileAccess.Read);
            /*(естественно, файл уже должен быть создан по этому адресу) */
            // создаем поток для чтения и считываем данные;
            potok = new System.IO.StreamReader(stream);
                /* подсчитываем в переменной n количество записей в
                файле: */
                int n = 0;
                string t = potok.ReadLine();
                while (t != null)
                {
                    t = potok.ReadLine();
                    n++;
                }
                /* Устанавливаем указатель на начало потока(т.к.
                методы ReadLine() сместили его в конец файла): */
                stream.Seek(0, SeekOrigin.Begin);
                // инициализируем массив
                Kurs = new double[n];
                /* считываем записи о курсах доллара в пременную t,
                затем конвертируем в числовой формат и переносим в
                массив Kurs: */
                int i = 0;
                t = potok.ReadLine();
                while (t != null)
                {
                    // записываем считанное число в массив
                    Kurs[i] = Convert.ToDouble(t);
                    t = potok.ReadLine();
                    i++;
                }
                // закрываем поток
                potok.Close();
                // если записей нет
                if (Kurs.Length == 0)
                    MessageBox.Show("Файл исходных данных пуст", "График",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                else this.Paint += new
                PaintEventHandler(drawDiagram);
                /* процедура прорисовки диаграммы drawDiagram будет
                вызываться каждый раз, когда будет происходить
                перерисовка формы */
            } // (закрыли блок try)
                // Обработка исключений:
                // файл исходных данных не найден:
            catch (FileNotFoundException exc)
            {
                MessageBox.Show("Файл исходных данных не найден" + "\n" + exc.Message);
            }
            // ошибка формата исходных данных
            catch (FormatException exc)
            {
                MessageBox.Show("Ошибка формата исходных данных" +"\n"+exc.Message);
            }
            // прочие исключения (выводим текст ошибки):
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message);
            }
        }

        private void drawDiagram(object sender, PaintEventArgs e)
        { // Графическая поверхность:
            Graphics g = e.Graphics;
            // Задаем шрифт заголовка:
            Font font1 = new Font("Arial", 11, FontStyle.Bold);
            // Задаем шрифт для подписи значений:
            Font font2 = new Font("Tahoma", 10, FontStyle.Regular);
            /*Выводим заголовок черной кистью, начиная с
            координат (20,10): */
            g.DrawString("Изменение курса доллара", font1,
            Brushes.Black, 20, 10);
            /* Определяем расстояние между узлами графика по оси
            x: */
            int d = (int)((this.ClientSize.Width - 20) / (Kurs.Length - 1));
            /* Ищем максимальное и минимальное значения (от этих
            значений будем отталкиваться при определении
            вертикальных координат узлов) */
            double maxKurs = Kurs[0];
            double minKurs = Kurs[0];
            for (int i = 0; i < Kurs.Length; i++)
            {
                if (Kurs[i] > maxKurs) maxKurs = Kurs[i];
                if (Kurs[i] < minKurs) minKurs = Kurs[i];
            }
            // Рисуем график:
            int x1, y1, x2, y2;
            // Задаем координаты первой точки:
            x1 = 8;
            y1 = this.ClientSize.Height - 20 - (int)((this.ClientSize.Height - 70) * (Kurs[0] - minKurs) / (maxKurs - minKurs));
            // Рисуем прямоугольник- метку первого узла:
            g.DrawRectangle(Pens.Black, x1 - 2, y1 - 2, 4, 4);
            // Определяем координаты следующих точек:
            for (int i = 1; i < Kurs.Length; i++)
            {
                x2 = 8 + i * d;
                y2 = this.ClientSize.Height - 20 - (int)((this.ClientSize.Height - 70) * (Kurs[i] - minKurs) / (maxKurs - minKurs));
                // Рисуем прямоугольник- метку следующего узла:
                g.DrawRectangle(Pens.Black, x2 - 2, y2 - 2, 4, 4);
                // Соединяем узлы линией:
                g.DrawLine(Pens.Black, x1, y1, x2, y2);
                // Подписываем значения:
                g.DrawString(Convert.ToString(Kurs[i - 1]),
                font2, Brushes.Black, x1 - 5, y1 - 20);
                x1 = x2;
                y1 = y2;
            } // для for
              // Подписываем последний узел графика:
            g.DrawString(Convert.ToString(Kurs[Kurs.Length - 1]), font2, Brushes.Black, x1 - 20, y1 - 20);
        }

        private void Form1_SizeChanged(object sender, EventArgs e)
        {
            this.Refresh();
        }
    }
}
