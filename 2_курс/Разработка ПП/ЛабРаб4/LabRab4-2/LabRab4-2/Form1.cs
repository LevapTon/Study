using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab4_2
{
    public partial class Form1 : Form
    {
        int[] Mas;
        int i;
        public Form1()
        {
            InitializeComponent();
            label1.Text = "";
            label2.Text = "";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Mas = new int[10];
            Random n = new Random();
            for (i = 0; i < 10; i++)
                Mas[i] = n.Next(1000);
            // Выводим на экран:
            foreach (int elem in Mas)
                label1.Text = label1.Text + elem + " ";
        }

        private void radioButton1_Click(object sender, EventArgs e)
        {
            try
            {
                Array.Sort(Mas);
                label2.Text = "";
                foreach (int elem in Mas)
                    label2.Text = label2.Text + elem + " ";
            }
            catch
            {
                label2.Text = "Вы еще не заполнили массив данными!";
            }
        }

        private void radioButton2_Click(object sender, EventArgs e)
        {
            try
            {
                Array.Sort(Mas);
                Array.Reverse(Mas);
                label2.Text = "";
                foreach (int elem in Mas)
                    label2.Text = label2.Text + elem + " ";
            }
            catch
            {
                label2.Text = "Вы еще не заполнили массив данными!";
            }
        }
    }
}
