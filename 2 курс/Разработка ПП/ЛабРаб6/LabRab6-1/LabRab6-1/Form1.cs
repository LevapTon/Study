using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab6_1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        // Устанавливаем сегодняшнюю дату на monthCalendar:
            monthCalendar1.TodayDate = DateTime.Now;
        }
        // Проверка вносимых символов в поле textBox1:
        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (char.IsDigit(e.KeyChar) || (e.KeyChar.ToString() == "-" && textBox1.Text == "") 
                || (e.KeyChar.ToString() == "," && textBox1.Text.IndexOf(",") == -1)) e.Handled = false;
            else e.Handled = true;
        }
        /* То есть: вносимый символ может быть цифрой, или
        знаком «-» (если символ первый), или запятой(если
        запятой до этого не было)*/
        // Для кнопки « Внести данные в файл » :
        private void button1_Click(object sender, EventArgs e)
        {
            DateTime d;
            // Получим информацию о файле meteo.txt :
            // Cоздаем объект fi типа FileInfo:
            System.IO.FileInfo fi = new System.IO.FileInfo(Application.StartupPath + "\\meteo.txt");
            // Создаем поток для записи:
            System.IO.StreamWriter potok;
            /* Если файл meteo.txt найден, будем добавлять в
            него записи, иначе создаем файл: */
            if (fi.Exists) potok = fi.AppendText();
            else potok = fi.CreateText();
            /* Переменной d задаем значение, равное выбранной
            в календаре дате: */
            d = monthCalendar1.SelectionStart;
            /* переводим значение переменной d в строку с
            кратким форматом записи даты: d.ToShortDateString(),
            заносим эту строку, знак пробела и значение
            температуры в поток:*/
            potok.WriteLine(d.ToShortDateString() + " " + textBox1.Text);
            potok.Close();
            textBox1.Text = "";
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}

