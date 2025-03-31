using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab7_1
{
    public partial class Form1 : Form
    {
        public Point[] Points = new Point[10];
        public Form1()
        {
            InitializeComponent();
            dataGridView1.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
            dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
            this.BackColor = Color.White;
            this.Text = "Отрисовка линий и кривых";
            this.ClientSize = new
            Size(470, 400);
            // Цвет фона таблицы задаем такой же, как у формы:
            dataGridView1.BackgroundColor = this.BackColor;
            // Убираем внешнюю границу компонента:
            dataGridView1.BorderStyle = BorderStyle.None;
            dataGridView1.ColumnCount = 11;
            dataGridView1.RowCount = 2;
            // убираем шапку таблицы:
            dataGridView1.ColumnHeadersVisible = false;
            dataGridView1.RowHeadersVisible = false;
            // Убираем полосу прокрутки:
            dataGridView1.ScrollBars = ScrollBars.None;
            // Задаем ширину таблицы
            dataGridView1.Width = 350;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int x = 20, y = 20;
            Random r = new Random();
            dataGridView1.Rows[0].Cells[0].Value = "x";
            dataGridView1.Rows[1].Cells[0].Value = "y";
            for (int i = 0; i < 10; i++)
                {
                    Points[i] = new Point(x, y);
                    x += 50;
                    y = r.Next(300);
                    dataGridView1.Rows[0].Cells[i + 1].Value =
                    x.ToString();
                    dataGridView1.Rows[1].Cells[i + 1].Value =
                    y.ToString();
                }
            this.Refresh();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            if (checkBox1.Checked)
            { // Рисуем кривую линию
                Pen P = new Pen(Color.Blue, 2);
                e.Graphics.DrawCurve(P, Points);
            }
            else
            { // Рисуем ломанную линию
                Pen p = new Pen(Color.Blue, 2);
                e.Graphics.DrawLines(p, Points);
            }
        }
    }
}
