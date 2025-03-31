using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab4_1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            // Переходим в многострочный режим
            textBox1.Multiline = true;
            textBox2.Multiline = true;
            textBox2.Visible = false;
            label1.Text = "";
            label2.Text = "";
            label3.Text = "";
            label4.Text = "";
            label5.Text = "";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Создаем полосу прокрутки
            textBox2.ScrollBars = ScrollBars.Vertical;
            // Задаем высоту поля вывода
            textBox2.Height = 150;
            textBox2.Text = textBox1.Text;
            textBox2.Visible = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            double sum = 0;
            double max, min;
            int i, n;
            try
            {
                // количество элементов:
                n = textBox1.Lines.Length - 1;
                //минус один, т.к. последний Enter создает лишнюю пустую строку
                label1.Text = "Количество элементов:" + Convert.ToString(n);
                // Находим сумму:
                for (i = 0; i < n; i++)
                    sum = sum + Convert.ToDouble(textBox1.Lines[i]);
                label2.Text = "Сумма: " + Convert.ToString(sum);
                // Находим среднее значение:
                label3.Text = "Среднее значение: " +
                Convert.ToString(sum / n);
                //Находим максимум и минимум по стандартному алгоритму:
                max = min = Convert.ToDouble(textBox1.Lines[0]);
                for (i = 0; i < n; i++)
                {
                    if (Convert.ToDouble(textBox1.Lines[i]) > max)
                        max = Convert.ToDouble(textBox1.Lines[i]);
                    if (Convert.ToDouble(textBox1.Lines[i]) < min)
                        min = Convert.ToDouble(textBox1.Lines[i]);
                }
                label4.Text = "Наибольшее значение: " +
                Convert.ToString(max);
                label5.Text = "Наименьшее значение: " +
                Convert.ToString(min);
            }
            catch
            {
                label1.Text = "Проверьте коректность входных данных"; 
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
            textBox1.Focus();
            textBox2.Visible = false;
            label1.Text = label2.Text = label3.Text =
            label4.Text = label5.Text = "";
        }
    }
}
