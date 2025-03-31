using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab4_sr_1
{
    public partial class Form1 : Form
    {
        int[] Mas;
        int i;
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            label1.Text = "";
            label3.Text = "";
            Mas = new int[10];
            Random n = new Random();
            for (i = 0; i < 10; i++)
                Mas[i] = n.Next(-100, 100);
            // Выводим на экран:
            foreach (int elem in Mas)
                label1.Text = label1.Text + elem + " ";
        }

        private void button2_Click(object sender, EventArgs e)
        {
            for (i = 0; i < Mas.Length; i++)
            {
                if (Mas[i] == Convert.ToInt32(textBox1.Text))
                {
                    label3.Text = "Элемент " + textBox1.Text + " на позиции " + Convert.ToString(i + 1);
                    break;
                }
                else
                {
                    label3.Text = "Такого элемента нет";
                }
            }
            textBox1.Clear();
        }
    }
}
