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

namespace LabRab7_3
{
    public partial class Form1 : Form
    {
        // Объявляем массив численых значений:
        private double[] K;
        // Объявляем массив подписей:
        private string[] L;
        // Сумма элементов массива чисел:
        private double sum;
        // Количество элементов в файле:
        private int n = 0;
        private string path = @"D:\Учеба\2 курс\Разработка ПП\ЛабРаб7\LabRab7-3\LabRab7-3\bin\Debug\Данные.txt";
        // Объявляем поток для чтения:
        public StreamReader potok;
        public Form1()
        {
            InitializeComponent();
            // задаем цвет формы:
            this.BackColor = System.Drawing.Color.White;
            try
            { // Создаем объект stream типа FileStream:
                FileStream stream = new FileStream(path, FileMode.Open, FileAccess.Read);
                //(естественно, файл уже должен быть создан)
                // создаем поток для чтения и считываем данные:
                potok = new
                StreamReader(stream);
                //подсчитываем в переменной n количество записей в файле:
                string t = potok.ReadLine();
                while (t != null)
                {
                    t = potok.ReadLine();
                    n++;
                }
                /* Устанавливаем указатель на начало потока (т.к.
                методы ReadLine() сместили его в конец файла) */
                stream.Seek(0, SeekOrigin.Begin);
                // инициализируем массивы
                K = new double[n];
                L = new string[n];
                int i = 0;
                t = potok.ReadLine();
                while (t != null)
                {
                    // записываем числа до пробела в массив:
                    K[i] = Convert.ToDouble(t.Substring(0, t.IndexOf(" ")));
                    // Прибавляем к общей сумме:
                    sum += K[i];
                    //считываем текст после пробела, заносим в массив L:
                    L[i] = t.Substring(t.IndexOf(" "));
                    // Считываем следующую строку
                    t = potok.ReadLine();
                    i++;
                } // для while
                  // закрываем поток
                potok.Close();
                // если записей нет
                if (K.Length == 0)
                    MessageBox.Show("Файл исходных данных пуст");
                else this.Paint += new PaintEventHandler(drawDiagram);
            } // для try
              // Обработка исключений:
              // файл исходных данных не найден:
            catch (FileNotFoundException exc)
            {
                MessageBox.Show("Файл исходных данных не найден" + "\n" + exc.Message);
            }
            // ошибка формата исходных данных
            catch (FormatException exc)
            {
                MessageBox.Show("Ошибка формата исходных данных" + "\n" + exc.Message);
            }
            // прочие исключения (выводим текст ошибки):
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message);
            }
        }

        // Вписываем процедуру:
        private void drawDiagram(object sender, PaintEventArgs e)
        {
            // Графическая поверхность:
            Graphics g = e.Graphics;
            // Задаем шрифт заголовка:
            Font font1 = new Font("Arial", 11,
            FontStyle.Bold);
            // Задаем шрифт легенды:
            Font font2 = new
            Font("Tahoma", 10, FontStyle.Regular);
            /* Выводим заголовок черной кистью, начиная с
            координат (20,10): */
            g.DrawString("Диаграмма популярности интернет - браузеров", font1, Brushes.Black, 20, 10);
            // диаметр сектора круговой диаграммы
            int d = (int)(this.ClientSize.Height * 4 / 5 - 40);
            // Начальная точка:
            int x = 30;
            int y = (int)((this.ClientSize.Height - d) / 2 + 10);
            /* координаты верхнего левого угла местоположения
            легенды */
            int lx = 60 + d;
            int ly = y + (int)((d - n * 20 + 10) / 2);
            // начальная точка дуги сектора
            int M = -90;
            // угол дуги сектора
            int alpha;
            // кисть для заливки сектора диаграммы
            Brush fBrush = Brushes.White;
            // рисуем диаграмму
            for (int i = 0; i < K.Length; i++)
            { // Определяем угол:
                alpha = (int)(360 * (K[i] / sum)) + 1;
                /* определяем цвет секторов (допустим,
                предполагается максимум 10 секторов)*/
                switch (i)
                {
                    case 0: fBrush = Brushes.Azure; break;
                    case 1: fBrush = Brushes.LightGreen; break;
                    case 2: fBrush = Brushes.DeepPink; break;
                    case 3: fBrush = Brushes.Violet; break;
                    case 4: fBrush = Brushes.MistyRose; break;
                    case 5: fBrush = Brushes.RoyalBlue; break;
                    case 6: fBrush = Brushes.SteelBlue; break;
                    case 7: fBrush = Brushes.Chocolate; break;
                    case 8: fBrush = Brushes.LightGray; break;
                    case 9: fBrush = Brushes.Gold; break;
                }
                // сектор диаграммы
                g.FillPie(fBrush, x, y, d, d, M, alpha);
                // граница сектора
                g.DrawPie(System.Drawing.Pens.Black, x, y, d, d, M, alpha);
                // начальная точка дуги для следующего сектора
                M = M + (int)(360 * (K[i] / sum)) + 1;
                // Легенда:
                // рисуем закрашенный прямоугольник:
                g.FillRectangle(fBrush, lx, ly + i * 20, 20, 10);
                // рисуем черный контур прямоугольника:
                g.DrawRectangle(System.Drawing.Pens.Black, lx, ly
                + i * 20, 20, 10);
                /* подпись легенды (текст из массива L + значение
                в процентах): */
                g.DrawString(L[i] + ":" + Convert.ToDouble(K[i] / sum * 100).ToString("N") + "%", font2, Brushes.Black, lx + 20, ly + i * 20 - 4);
            }
        }

        private void Form1_SizeChanged(object sender, EventArgs e)
        {
            this.Refresh();
        }
    }
}
