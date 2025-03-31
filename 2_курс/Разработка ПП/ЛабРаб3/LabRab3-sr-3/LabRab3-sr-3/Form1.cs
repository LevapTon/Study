using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab3_sr_3
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            trackBar1.Maximum = 100;
            trackBar1.Minimum = 0;
            trackBar1.Value = 0;
            trackBar1.TickFrequency = 10;
            trackBar2.Maximum = 100;
            trackBar2.Minimum = 0;
            trackBar2.Value = 100;
            trackBar2.RightToLeft = RightToLeft.Yes;
            trackBar2.TickFrequency = 10;
            label1.Text = Convert.ToString(trackBar1.Value) + "%";
            label2.Text = Convert.ToString(trackBar2.Value) + "%";
        }

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            float a;
            a = pictureBox1.Image.PhysicalDimension.Width / 5;
            label1.Text = Convert.ToString(trackBar1.Value) + "%";
            pictureBox1.Width = (Int16)(a / 100 * trackBar1.Value);
        }

        private void trackBar2_ValueChanged(object sender, EventArgs e)
        {
            float b;
            b = pictureBox1.Image.PhysicalDimension.Height / 3;
            label2.Text = Convert.ToString(100 - trackBar2.Value) + "%";
            pictureBox1.Height = (Int16)(b / 100 * (100 - trackBar2.Value));
        }
    }
}
