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
    public partial class Form2 : Form
    {
        public string name;
        public double cost;

        public Form2()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                this.name = Convert.ToString(textBox1.Text);
                this.cost = Convert.ToDouble(textBox2.Text);
                textBox1.Clear();
                textBox2.Clear();
                this.Close();
            }
            catch
            {
                MessageBox.Show("Проверьте корректность входных данных!", "Внимание!");
            }
        }
    }
}
