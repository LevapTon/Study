using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab1_sr1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            double x1, y1, x2, y2, x3, y3, s, p, st1, st2, st3;
            try
            {
                x1 = Convert.ToDouble(textBox1.Text);
                y1 = Convert.ToDouble(textBox2.Text);
                x2 = Convert.ToDouble(textBox3.Text);
                y2 = Convert.ToDouble(textBox4.Text);
                x3 = Convert.ToDouble(textBox5.Text);
                y3 = Convert.ToDouble(textBox6.Text);
                st1 = Math.Sqrt(Math.Pow((x2 - x1), 2) + Math.Pow((y2 - y1), 2));
                st2 = Math.Sqrt(Math.Pow((x3 - x2), 2) + Math.Pow((y3 - y2), 2));
                st3 = Math.Sqrt(Math.Pow((x1 - x3), 2) + Math.Pow((y1 - y3), 2));
                if ((st1 + st2 > st3) & (st1 + st3 > st2) & (st2 + st3 > st1))
                {
                    p = st1 + st2 + st3;
                    s = Math.Abs(0.5 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)));
                    label7.Text = "Периметр: " + p.ToString("N") + "\n" + "Площадь: " + s.ToString("N");
                }
                else
                {
                    label7.Text = "Треугольник не существует";
                }
            }
            catch
            {
                MessageBox.Show("Проверьте правильность входных данных", "Внимание!");
            }
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar.Equals((char)13)) textBox2.Focus();
        }

        private void textBox2_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar.Equals((char)13)) textBox3.Focus();
        }

        private void textBox3_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar.Equals((char)13)) textBox4.Focus();
        }

        private void textBox4_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar.Equals((char)13)) textBox5.Focus();
        }

        private void textBox5_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar.Equals((char)13)) textBox6.Focus();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
            textBox2.Text = "";
            textBox3.Text = "";
            textBox4.Text = "";
            textBox5.Text = "";
            textBox6.Text = "";
            textBox1.Focus();
            label7.Text = "";
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
