using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab6_3
{
    public partial class Form1 : Form
    {
        string fname = "";
        public Form1()
        {
            InitializeComponent();
            textBox1.Multiline = true;
            textBox1.Height = 180;
            textBox1.Width = 220;
            textBox1.ScrollBars = ScrollBars.Vertical;
            textBox1.Visible = false;
            openFileDialog1.DefaultExt = "txt";
            openFileDialog1.Filter = "текстовый документ | *.txt";
            saveFileDialog1.DefaultExt = "txt";
            saveFileDialog1.Filter = "текстовый документ | *.txt";
        }

        private void создатьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            fname = "";
            textBox1.Text = "";
            textBox1.Visible = true;
        }

        private void сохранитьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog1.FileName = "";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
                fname = openFileDialog1.FileName;
            try
            {
                System.IO.StreamReader potok = new
                System.IO.StreamReader(fname);
                textBox1.Text = potok.ReadToEnd();
                textBox1.Visible = true;
                potok.Close();
            }
            catch
            {
                MessageBox.Show("Ошибка чтения файла");
            }
        }

        private void выходToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void сохранитьToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
                fname = saveFileDialog1.FileName;
            try
            {
                System.IO.FileInfo fi = new System.IO.FileInfo(fname);
                System.IO.StreamWriter potok = fi.CreateText();
                potok.Write(textBox1.Text);
                potok.Close();
            }
            catch
            {
                MessageBox.Show("Ошибка сохранения файла");
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int i;
            double max, min;
            string m1 = ""; //для промежуточного сохранения дат
            string m2 = "";
            try
            {
            // за min и max принимаем первое значение температуры:
                max = min = Convert.ToDouble(textBox1.Lines[0].Substring(11));
                for (i = 0; i < textBox1.Lines.Length; i++)
                {
                    if (textBox1.Lines[i].Length > 0)
                    {
                        if (Convert.ToDouble(textBox1.Lines[i].Substring(11)) > max)
                        {
                            max = Convert.ToDouble(textBox1.Lines[i].Substring(11));
                            m1 = textBox1.Lines[i].Substring(0, 8);
                        }
                        if (Convert.ToDouble(textBox1.Lines[i].Substring(11)) < min)
                        {
                            min = Convert.ToDouble(textBox1.Lines[i].Substring(11));
                            m2 = textBox1.Lines[i].Substring(0, 8);
                        }
                    }
                }
                textBox1.Text = "Максимальная температура: " + Convert.ToString(max) + Environment.NewLine + "наблюдалась " + m1 + Environment.NewLine + "Минимальная температура: " + Convert.ToString(min) + Environment.NewLine + "наблюдалась " + m2;
            } // try
            catch
            {
                MessageBox.Show("Некорректные данные", "Внимание!");
            }
        }
    }
}
