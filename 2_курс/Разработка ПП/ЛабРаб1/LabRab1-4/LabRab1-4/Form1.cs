using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab1_4
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar.Equals((char)13)) textBox2.Focus();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
            textBox2.Text = "";
            label3.Text = "";
            textBox1.Focus();
        }

        private void textBox2_KeyPress(object sender, KeyPressEventArgs e)
        {
            double a, b, c;
            if (e.KeyChar.Equals((char)13))
            {
                try
                {
                    a = Convert.ToDouble(textBox1.Text);
                    b = Convert.ToDouble(textBox2.Text);
                    c = Math.Abs(a - b);
                    if (a > b) label3.Text = "Пожалуйста, доплатите " + c.ToString("C");  // "С" - денежный формат
                    if (a < b) label3.Text = "Ваша сдача " + c.ToString("C") + " Спасибо за покупку!";
                    if (a == b) label3.Text = "Спасибо за покупку!";
                }
                catch
                {
                    MessageBox.Show("Проверьте правильность входдных данных", "Внимание!");
                }
            }
        }
    }
}
