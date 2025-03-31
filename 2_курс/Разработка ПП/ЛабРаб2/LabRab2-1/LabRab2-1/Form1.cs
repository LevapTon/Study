using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab2_1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            double cost = 0, sum;
            switch (comboBox1.SelectedIndex)
            {
                case 0: cost = 18.9; break;
                case 1: cost = 20.3; break;
                case 2: cost = 22; break;
                case 3: cost = 17.6; break;
            }
            sum = Convert.ToDouble(numericUpDown1.Value) * cost;
            label3.Text = "К оплате " + sum.ToString("C");
        }
    }
}
