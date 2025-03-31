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
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // Создаем объект f2, имеющий тип Form2
            Form2 f2 = new Form2();
            f2.ShowDialog(); // Показываем форму 2
            label1.Text = "Сумма равна " + Convert.ToString(f2.a + f2.b);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Form3 f3 = new Form3();
            f3.ShowDialog(); // Показываем форму 3
        }
    }
}
