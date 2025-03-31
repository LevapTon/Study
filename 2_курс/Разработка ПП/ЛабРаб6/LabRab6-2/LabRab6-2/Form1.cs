using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab6_2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            /*Задаем многострочнй режим
            textBox1.Multiline = true;
            //Задаем ширину и высоту
            textBox1.Width = 180;
            textBox1.Height = 150;
            // Создаем вертикальную полосу прокрутки:
            textBox1.ScrollBars = ScrollBars.Vertical;
            textBox1.Visible = false;
            label2.Text = "";
            // Формируем элементы списка:
            comboBox1.Items.Add("Январь");
            comboBox1.Items.Add("Февраль");
            comboBox1.Items.Add("Март");
            comboBox1.Items.Add("Апрель");
            comboBox1.Items.Add("Май");
            comboBox1.Items.Add("Июнь");
            comboBox1.Items.Add("Июль");
            comboBox1.Items.Add("Август");
            comboBox1.Items.Add("Сентябрь");
            comboBox1.Items.Add("Октябрь");
            comboBox1.Items.Add("Ноябрь");
            comboBox1.Items.Add("Декабрь");*/
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // Создаем поток для чтения:
            System.IO.StreamReader potok;
            /* Пытаемся считать данные из файла meteo.txt
            (путь до него, разумеется, пропишите каждый свой)*/
            try
            {
                potok = new System.IO.StreamReader(@"D:\Учеба\2 курс\Разработка ПП\ЛабРаб6\LabRab6-1\LabRab6-1\bin\Debug\meteo.txt", Encoding.GetEncoding(1251));
                //Считываем весь текст до конца
                textBox1.Text = potok.ReadToEnd();
                potok.Close();
                // Делаем поле видимым
                textBox1.Visible = true;
            }
            catch
            {
                MessageBox.Show("Файл исходных данных не найден", "Внимание!");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            {//Запоминаем номер выбранного месяца в переменную m:
                int m = comboBox1.SelectedIndex + 1;
                // (+1, т.к. нумерация элементов списка с нуля)
                int n = 0;//количество записей по выбранному месяцу
                int i;
                double sum = 0;
                // В цикле просматриваем все строки:
                for (i = 0; i < textBox1.Lines.Length; i++)
                /* если строка не пустая- сравниваем номер месяца
                в ней c m */
                {
                    if (textBox1.Lines[i].Length > 0)
                        if (Convert.ToInt16(textBox1.Lines[i].Substring(3, 2)) == m)
                        {
                            n++;
                            sum += Convert.ToDouble
                            (textBox1.Lines[i].Substring(textBox1.Lines[i].IndexOf
                            (" ")));
                            /* определили позицию пробела в строке, все символы
                            после этого пробела преобразовали в вещественное
                            число и просуммировали с общей суммой */
                        }
                }
                if (n == 0)
                    label2.Text = "В файле нет данных о температуре за " + comboBox1.Text;
                else label2.Text = "Средняя температура за " + comboBox1.Text + ": " + (sum / n).ToString("N");
            }

        }
    }    
}
