using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            label2.Text = "";  // удаление ненужного текста
        }

        private void textBox1_TextChanged(object sender, EventArgs e)  // обработка нажатия по кнопке
        {  // если поле не пустое – будет выведено приветствие
            if (textBox1.Text != "") label2.Text = "Здраствуйте, " + textBox1.Text + "!";
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }
    }
}
