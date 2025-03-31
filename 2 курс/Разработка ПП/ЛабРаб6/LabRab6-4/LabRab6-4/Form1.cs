using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab6_4
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            panel1.Visible = false;
            textBox3.Multiline = true;
            textBox3.Width = 270;
            textBox3.Height = 160;
            // (Размеры при необходимости измените)
            textBox3.Visible = false;
            // Задаем вид вводимых символов
            textBox2.PasswordChar = '*';
            label3.Text = "";
            label4.Text = "";
            button4.Visible = false;
        }
        // Кнопка «Просмотреть состояние счета»
        private void button1_Click(object sender, EventArgs e)
        {
            panel1.Visible = true;
            // Создаем поток для чтения:
            System.IO.StreamReader potokR;
            /* Пытаемся считать данные из файла «Данные по
            клиентам.txt» Путь до него пользователю программы
            знать не нужно, поэтому диалоговое окно открытия
            файла использовать не будем (путь пропишем
            программно). Всю информацию помещаем в поле textBox1
            */
            try
            { // Путь до файла укажите каждый свой
                potokR = new System.IO.StreamReader(@"D:\Учеба\2 курс\Разработка ПП\ЛабРаб6\LabRab6-4\LabRab6-4\bin\Debug\Данные по клиентам.txt");
                textBox3.Text = potokR.ReadToEnd();
                potokR.Close();
            }
            catch
            {
                MessageBox.Show("Ошибка чтения данных!", "Внимание!");
            }
        }
        // Кнопка «OK»
        private void button2_Click(object sender, EventArgs e)
        {
            /* Нулевая и первая строка файла-заголовки
            таблицы, их не смотрим (поэтому цикл с двух).
            Если в строке есть текст, сравниваем логин и пароль в
            полях textBox2 textBox3 с соответствующими данными в
            файле. Метод Trim() отбрасывает все пробелы с начала
            и конца подстрок, в которых находятся ФИО, пароли и
            суммы. Начальный файл мы сформировали так, что ФИО
            клиента находится где-то в промежутке с нулевой до 14
            позиции, пароль- с двадцать первой до тридцать первую
            позицию, сумма - примерно с тридцать шестой позиции
            (начать смотреть можно и раньше)*/
            int flag = 0;
            //(flag =1, если логин подошел к паролю, иначе flag = 0)
            try
            {
                for (int i = 2; i < textBox3.Lines.Length; i++)
                    if (textBox3.Lines[i].Length > 0)
                        if ((textBox3.Lines[i].Substring(0, 14).Trim() == textBox1.Text) 
                            && (textBox3.Lines[i].Substring(20, 10).Trim() == textBox2.Text))
                        {
                            label3.Text = "Уважаемый " + textBox1.Text + ", на вашем счете " + textBox3.Lines[i].Substring(35).Trim() + " рублей";
                            flag = 1;
                        }
                /* Пусть логин администратора системы-
                administrator, пароль- master */
                if (textBox1.Text == "administrator" && textBox2.Text == "master")
                    { /* Становятся доступны для просмотра все
                        данные, кнопка «Сохранить изменения в исходном
                        файле», надпись «Информация по всем счетам» */
                        label4.Text = "Информация по всем счетам";
                        textBox3.Visible = true;
                        button4.Visible = true;
                        flag = 1;
                    }
                    // Если логин не подошел к паролю:
                if (flag == 0) label3.Text = "Проверьте имя пользователя и пароль!";
            }
            catch
            {
                MessageBox.Show("Ошибка обработки данных!", "Внимание!");
            }
        }
        // Кнопка «Очистить»
        private void button3_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
            textBox2.Text = "";
            label3.Text = "";
            label4.Text = "";
            textBox3.Visible = false;
            button4.Visible = false;
            textBox1.Focus();
        }
        // Кнопка « Сохранить изменения в исходном файле »
        private void button4_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Перезаписать файл?", "Сохранение", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
            {
                System.IO.StreamWriter potokW;
                try
                {
                    potokW = new
                    System.IO.StreamWriter(@"D:\Учеба\2 курс\Разработка ПП\ЛабРаб6\LabRab6-4\LabRab6-4\bin\Debug\Данные по клиентам.txt", false);
                    potokW.Write(textBox3.Text);
                    potokW.Close();
                    MessageBox.Show("Данные были обновлены", "Внимание!");
                }
                catch
                {
                    MessageBox.Show("Ошибка сохранения файла", "Внимание!");
                }
            }
        }
    }
}
