using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab5_sr_3
{
    public partial class Form1 : Form
    {
        DateTime curDate;
        int p = 0;
        public Form1()
        {
            InitializeComponent();
            textBox1.HideSelection = false;
            curDate = DateTime.Now;
            textBox1.Text += $"{curDate.AddDays(-2).ToShortDateString()} Сходить к зубному\n";
            textBox1.Text += $"{curDate.AddDays(-1).ToShortDateString()} Купить перчатки\n";
            textBox1.Text += $"{curDate.ToShortDateString()} Принять гостей\n";
            textBox1.Text += $"{curDate.AddDays(1).ToShortDateString()} Полить цветы\n";
            textBox1.Text += $"{curDate.AddDays(2).ToShortDateString()} Починить телевизор\n";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text.IndexOf(Convert.ToString(curDate.ToShortDateString()), p) >= 0)
            {
                if (p <= textBox1.Text.Length)
                {
                    textBox1.SelectionStart = textBox1.Text.IndexOf(Convert.ToString(curDate.ToShortDateString()), p);
                    textBox1.SelectionLength = Convert.ToString(curDate.ToShortDateString()).Length;
                    p = textBox1.SelectionStart + 1;
                }
            }
            else
            {
                MessageBox.Show("Поиск не дал результатов");
                p = 0;
            }
            p = 0;
        }
    }
}
