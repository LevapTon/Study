using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab1_sr4
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            int a, b;
            label1.Text = "Ниже даны два числа" + "\n" + "Укажите их произведение";
            Random r = new Random();
            a = r.Next(-10, 10);
            b = r.Next(-10, 10);
            label3.Text = a.ToString();
            label4.Text = b.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int num1, num2, answ;
            try
            {
                num1 = Convert.ToInt32(label3.Text);
                num2 = Convert.ToInt32(label4.Text);
                answ = Convert.ToInt32(textBox3.Text);
                if (num1 * num2 == answ) label2.Text = "Верно";
                else label2.Text = "Не верно";
            }
            catch
            {
                MessageBox.Show("Проверьте правильность введенных данных", "Внимание!");
            }
        }
    }
}
