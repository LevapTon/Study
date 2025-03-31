using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab1_sr3
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int num, res = 1;
            try
            {
                num = Convert.ToInt32(textBox1.Text);
                for (int i = num; i > 1; i--)
                {
                    res *= i;
                }
                label3.Text = "Ответ: " + res.ToString();
            }
            catch
            {
                MessageBox.Show("Проверьте правильность введенных данных", "Внимание!");
            }
        }
    }
}
