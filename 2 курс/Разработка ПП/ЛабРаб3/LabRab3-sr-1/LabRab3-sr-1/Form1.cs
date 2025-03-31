using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab3_sr_1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void крупныйToolStripMenuItem_Click(object sender, EventArgs e)
        {
            label1.Font = new Font("Times New Roman", 30);
        }

        private void среднийToolStripMenuItem_Click(object sender, EventArgs e)
        {
            label1.Font = new Font("Times New Roman", 20);
        }

        private void мелкийToolStripMenuItem_Click(object sender, EventArgs e)
        {
            label1.Font = new Font("Times New Roman", 10);
        }

        private void чёрныйToolStripMenuItem_Click(object sender, EventArgs e)
        {
            label1.ForeColor = Color.Black;
        }

        private void красныйToolStripMenuItem_Click(object sender, EventArgs e)
        {
            label1.ForeColor = Color.Red;
        }

        private void синийToolStripMenuItem_Click(object sender, EventArgs e)
        {
            label1.ForeColor = Color.Blue;
        }

        private void зеленыйToolStripMenuItem_Click(object sender, EventArgs e)
        {
            label1.ForeColor = Color.Green;
        }
    }
}
