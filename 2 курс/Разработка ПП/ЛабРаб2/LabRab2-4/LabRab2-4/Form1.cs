using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab2_4
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            double kursD, kursE, sum, result = 0;
            kursD = Convert.ToDouble(textBox1.Text);
            kursE = Convert.ToDouble(textBox2.Text);
            sum = Convert.ToDouble(textBox3.Text);
            switch (comboBox1.SelectedIndex)
            {
                case 0:  //из rus
                {
                    if (radioButton1.Checked)  //rus -> rus
                    {
                            result = sum;
                            label5.Text = textBox3.Text + "RUS->" + result.ToString("N") + " RUS";
                    }
                    if (radioButton2.Checked)  //rus -> usd
                    {
                            result = sum / kursD;
                            label5.Text = textBox3.Text + "RUS->" + result.ToString("N") + " USD";
                    }
                    if (radioButton3.Checked)  //rus -> eur
                    {
                            result = sum / kursE;
                            label5.Text = textBox3.Text + "RUS->" + result.ToString("N") + " EUR";
                    }
                } break;
                case 1:  //из usd
                    {
                        if (radioButton1.Checked)  //usd -> rus
                        {
                            result = sum * kursD;
                            label5.Text = textBox3.Text + "USD->" + result.ToString("N") + " RUS";
                        }
                        if (radioButton2.Checked)  //usd -> usd
                        {
                            result = sum;
                            label5.Text = textBox3.Text + "USD->" + result.ToString("N") + " USD";
                        }
                        if (radioButton3.Checked)  //usd -> eur
                        {
                            result = (kursD / kursE) * sum;
                            label5.Text = textBox3.Text + "USD->" + result.ToString("N") + " EUR";
                        }
                    }
                    break;
                case 2:  //из eur
                    {
                        if (radioButton1.Checked)  //eur -> rus
                        {
                            result = sum * kursE;
                            label5.Text = textBox3.Text + "EUR->" + result.ToString("N") + " RUS";
                        }
                        if (radioButton2.Checked)  //eur -> usd
                        {
                            result = (kursE / kursD) * sum;
                            label5.Text = textBox3.Text + "EUR->" + result.ToString("N") + " USD";
                        }
                        if (radioButton3.Checked)  //eur -> eur
                        {
                            result = sum;
                            label5.Text = textBox3.Text + "EUR->" + result.ToString("N") + " EUR";
                        }
                    }
                    break;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
