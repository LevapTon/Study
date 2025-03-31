using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab3_3
{
    public partial class Form2 : Form
    {
        public double a, b;
        public Form2()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.a = Convert.ToDouble(textBox1.Text);
            this.b = Convert.ToDouble(textBox2.Text);
            this.Close();
        }
    }
}
