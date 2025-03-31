using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab1_sr2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            double old_num, new_num, cost, bill, spend;
            try
            {
                old_num = Convert.ToDouble(textBox1.Text);
                new_num = Convert.ToDouble(textBox2.Text);
                cost = Convert.ToDouble(textBox3.Text);
                if (new_num - old_num > 0)
                {
                    spend = new_num - old_num;
                    bill = spend * cost;
                    label5.Text = "Вами потреблено " + spend.ToString() + " кВт" + "\n" +
                        "На сумму " + bill.ToString("C");
                }
                else label5.Text = "Некорректно введены данные";
            }
            catch
            {
                MessageBox.Show("Проверьте правильность введенных данных", "Внимание!");
            }
        }
    }
}
