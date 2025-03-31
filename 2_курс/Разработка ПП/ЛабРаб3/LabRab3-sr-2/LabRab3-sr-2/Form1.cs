using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab3_sr_2
{
    public partial class Form1 : Form
    {
        Form2 f2 = new Form2();
        double sum = 0;
        string name, res;
        double cost;
        public Form1()
        {
            InitializeComponent();
            label1.Text = "";
            label2.Text = "";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            f2.ShowDialog();
            name = f2.name;
            cost = f2.cost;
            res += name + " " + Convert.ToString(cost) + "\n";
            label1.Text = res;
            sum += cost;
            label2.Text = "Итого: " + Convert.ToString(sum) + Convert.ToString("р.");
        }
    }
}
