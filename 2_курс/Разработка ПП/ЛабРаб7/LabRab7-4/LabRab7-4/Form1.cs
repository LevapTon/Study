using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab7_4
{
    public partial class Form1 : Form
    {
        /* переменные sky и plane будут содержать
        изображения неба и самолета */
        private Bitmap sky, plane;
        // рабочая поверхность
        private Graphics g;
        /* приращение координаты по X определяет
        скорость полета - чем больше будет приращение dx за
        некоторый интервал времени, тем выше сскорость */
        private int dx;
        // прямоугоьная область, в которой находится самолет
        private Rectangle rct;
        // генератор случайных чисел
        private Random r;

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            { // загружаем битовые образы
              // небо:
                sky = new Bitmap(@"D:\Учеба\2 курс\Разработка ПП\ЛабРаб7\LabRab7-4\LabRab7-4\bin\Debug\Sky.jpg");
                // самолет:
                plane = new Bitmap(@"D:\Учеба\2 курс\Разработка ПП\ЛабРаб7\LabRab7-4\LabRab7-4\bin\Debug\Plane.png");
                plane.MakeTransparent();
                /*Системная палитра определяет один цвет в качестве
                стандартно прозрачного, или альфа цвета. Метод
                MakeTransparent() делает стандартно прозрачные цвета
                просвечиваемыми для данного объекта Bitmap. Иными
                словами, пусть будет видно только самолет, без его
                белого прямоугольного фона */
            }
            catch (Exception exc)
            {
                MessageBox.Show("Ошибка загрузки битовых образов \n"+exc.ToString(), "Полет в облаках",MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.Close();
                return;
            }
            // фоновый рисунок формы:
            BackgroundImage = new Bitmap(@"D:\Учеба\2 курс\Разработка ПП\ЛабРаб7\LabRab7-4\LabRab7-4\bin\Debug\Sky.jpg");
            /* задаем размер формы, соответствующий фоновому
            рисунку: */
            this.ClientSize = new Size
            (BackgroundImage.Width, BackgroundImage.Height);
            // делаем границу окна фиксированной
            this.FormBorderStyle =
            FormBorderStyle.FixedSingle;
            // блокируем кнопку "развернуть"
            this.MaximizeBox = false;
            /* g - графическая поверхность, на которой будем
            формировать рисунок */
            g = Graphics.FromImage(BackgroundImage);
            // инициализируем генератор случайных чисел
            r = new Random();
            // начальное положение области с самолетом
            rct.X = -40;
            rct.Y = 20 + r.Next(20); //Координата по y случайна
            rct.Width = plane.Width;
            rct.Height = plane.Height;
            /* скорость полета определяется периодом сигналов
            таймера и величиной приращения dx */
            dx = 2;// скорость полета - 2 пикселя/тик таймера
            timer1.Interval = 20;
            timer1.Enabled = true;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            // стираем изображение самолета путем заполнения всей области фоном:
            g.DrawImage(sky, 0, 0);
            // изменяем положение самолета
            // Если не достигли границы, выполняем приращение по координате Х:
            if (rct.X < this.ClientRectangle.Width) rct.X += dx;
            else
            { /* если достигли границы, задаем положение
                самолета заново */
                rct.X = -40;
                rct.Y = 20 + r.Next(this.ClientSize.Height -
                40 - plane.Height);
                // Задаем скорость - от 2 до 6 пикселей/тик таймера
                dx = 2 + r.Next(5);
            }
            // рисуем самолет на рабочей поверхности
            g.DrawImage(plane, rct.X, rct.Y);
            // Задаем скрытие самолета в облаках:
            // Рисуем прямоугоьную рамку
            Rectangle rct2 = new Rectangle(20, 20, sky.Width -
            40, sky.Height - 40);
            g.DrawRectangle(Pens.Black, rct2.X, rct2.Y, rct2.Width
            - 1, rct2.Height - 1);
            // обновляем область
            this.Invalidate(rct2);
        }

        public Form1()
        {
            InitializeComponent();
            this.Text = "Полет в облаках";
        }
    }
}
