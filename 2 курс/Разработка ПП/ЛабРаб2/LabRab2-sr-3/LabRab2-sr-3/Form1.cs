using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab2_sr_3
{
    public partial class Form1 : Form
    {
        int  msec = 0, count = 10;

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (msec == 99)
            {
                count--;
                label1.Text = count.ToString();
                msec = 0;
            }
            else msec++;

            if (count == 0)
            {
                timer1.Enabled = false;
            }

            if (timer1.Enabled != true)
            {
                MessageBox.Show("ПУСК", "Внимание!");
                this.Close();
            }
        }

        public Form1()
        {
            InitializeComponent();
            label1.Text = Convert.ToString(count);
            timer1.Interval = 10;
            timer1.Enabled = true;
        }
    }
}
