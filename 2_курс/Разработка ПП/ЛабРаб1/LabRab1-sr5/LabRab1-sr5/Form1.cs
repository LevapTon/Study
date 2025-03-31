using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab1_sr5
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            label5.Text = "0";
            label6.Text = "0";
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            double num; 
            int count1, count2;
            try
            {
                count1 = Convert.ToInt32(label5.Text);
                count2 = Convert.ToInt32(label6.Text);
                if (e.KeyChar.Equals((char)13))
                {
                    num = Convert.ToDouble(textBox1.Text);
                    if (num > 0)
                    {
                        count1++;
                        label5.Text = Convert.ToString(count1);
                    }
                    else if (num < 0)
                    {
                        count2++;
                        label6.Text = Convert.ToString(count2);
                    }
                    else
                    {
                        this.Close();
                    }
                }
            }
            catch
            {
                MessageBox.Show("Проверьте правильность входных данных!", "Внимание!");
            }
        }
    }
}
