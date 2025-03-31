using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab2_3
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void radioButton1_Click(object sender, EventArgs e)
        {
            label2.Text = "Сумма равна " + Convert.ToString(Convert.ToDouble(textBox1.Text) + Convert.ToDouble(textBox2.Text));
        }

        private void radioButton2_Click(object sender, EventArgs e)
        {
            label2.Text = "Разность равна " + Convert.ToString(Convert.ToDouble(textBox1.Text) - Convert.ToDouble(textBox2.Text));
        }

        private void radioButton3_Click(object sender, EventArgs e)
        {
            label2.Text = "Произведение равно " + Convert.ToString(Convert.ToDouble(textBox1.Text) * Convert.ToDouble(textBox2.Text));
        }

        private void radioButton4_Click(object sender, EventArgs e)
        {
            if (Convert.ToDouble(textBox2.Text) != 0)
                label2.Text = "Частное равно " + Convert.ToString(Convert.ToDouble(textBox1.Text) / Convert.ToDouble(textBox2.Text));
            else label2.Text = "Попытка деления на ноль!";
        }
    }
}
