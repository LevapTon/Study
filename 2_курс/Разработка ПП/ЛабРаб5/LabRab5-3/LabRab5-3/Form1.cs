using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab5_3
{
    public partial class Form1 : Form
    {
        DateTime d0, d1, d2;

        /*С заданным периодом обновляем текущую дату, время, день недели
        (особо актуально для секунд) */
        private void timer1_Tick(object sender, EventArgs e)
        {
            // В d0 записываем текущую дату и время:
            d0 = DateTime.Now;
            label1.Text = "Today is " + Convert.ToString(d0.DayOfWeek) + " " + Convert.ToString(d0);
        }

        // Кнопка «Сгенерировать расписание»
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                label4.Text = "";
                //В d1 записываем выбранную дату из DateTimePicker:
                d1 = dateTimePicker1.Value;
                // Допустим, предполагается 8 лабораторных занятий:
                for (int i = 1; i < 9; i++)
                {
                    label4.Text = label4.Text + d1.ToLongDateString() + "\n";
                    // Сдвигаем d1 на 2 недели вперед:
                    d1 = d1.AddDays(14);
                }
                //В d2 записываем время из maskedTextBox :
                d2 = Convert.ToDateTime(maskedTextBox1.Text);
                // Допустим, занимаемся 2 пары с перерывом в 10 минут:
                label5.Text = "Время занятий: " + maskedTextBox1.Text + "-" + d2.AddHours(3).AddMinutes(10).ToShortTimeString();
            }
            catch
            {
                MessageBox.Show("Проверьте корректность входных данных!");
            }
        }

        // Для кнопки «Выход»:
        private void button2_Click(object sender, EventArgs e)
        {
            // Запускаем генерацию событий Tick для второго таймера
            timer2.Enabled = true;
        }

        // С каждым тиком делаем форму все более прозрачной:
        private void timer2_Tick(object sender, EventArgs e)
        {
            if (t != 0)
            {
                this.Opacity = (float)t / 100;
                this.Refresh();//отрисовываем форму заново
                t--;
            }
            else this.Close();
        }

        int t = 100; // Степень прозрачности формы
        public Form1()
        {
            InitializeComponent();
            timer1.Enabled = true;
            timer2.Enabled = false;
            timer1.Interval = 500;
            timer1.Interval = 50;
            label1.Text = "";
            label4.Text = "";
            label5.Text = "";
        }
    }
}
