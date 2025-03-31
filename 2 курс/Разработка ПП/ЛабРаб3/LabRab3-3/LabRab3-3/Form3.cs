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
    public partial class Form3 : Form
    {
        public Form3()
        {
            InitializeComponent();
        }

        private void Form3_Activated(object sender, EventArgs e)
        {
            this.label1.Text = "Приложение написано струдентом потока ПИНб–21а \n Плотниковым Павлом Сергеевичем, 25.10.2022";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
