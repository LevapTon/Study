using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab4_sr_2
{
    public partial class Form1 : Form
    {
        const int n = 3; // n- число столбцов
        const int m = 3; // m- число строк
        int i, j;
        int mat1, mat2, mat3, mat4, mat5, mat6, det; // для нахождения определителя

        private void button1_Click(object sender, EventArgs e)
        {
            label1.Text = "";
            dataGridView1.DefaultCellStyle.BackColor = Color.White;
            // Заполняем ячейки:
            Random r = new Random();
            for (i = 0; i < n; i++)
                for (j = 0; j < m; j++)
                    dataGridView1.Rows[i].Cells[j].Value =
                    (r.Next(-5, 5)).ToString();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            mat1 = Convert.ToInt32(dataGridView1.Rows[0].Cells[0].Value) * Convert.ToInt32(dataGridView1.Rows[1].Cells[1].Value) *
                Convert.ToInt32(dataGridView1.Rows[2].Cells[2].Value);
            mat2 = Convert.ToInt32(dataGridView1.Rows[2].Cells[0].Value) * Convert.ToInt32(dataGridView1.Rows[0].Cells[1].Value) *
                Convert.ToInt32(dataGridView1.Rows[2].Cells[2].Value);
            mat3 = Convert.ToInt32(dataGridView1.Rows[0].Cells[2].Value) * Convert.ToInt32(dataGridView1.Rows[1].Cells[0].Value) *
                Convert.ToInt32(dataGridView1.Rows[2].Cells[1].Value);
            mat4 = Convert.ToInt32(dataGridView1.Rows[2].Cells[0].Value) * Convert.ToInt32(dataGridView1.Rows[1].Cells[1].Value) *
                Convert.ToInt32(dataGridView1.Rows[0].Cells[2].Value);
            mat5 = Convert.ToInt32(dataGridView1.Rows[0].Cells[0].Value) * Convert.ToInt32(dataGridView1.Rows[1].Cells[2].Value) *
                Convert.ToInt32(dataGridView1.Rows[2].Cells[1].Value);
            mat6 = Convert.ToInt32(dataGridView1.Rows[1].Cells[0].Value) * Convert.ToInt32(dataGridView1.Rows[0].Cells[1].Value) *
                Convert.ToInt32(dataGridView1.Rows[2].Cells[2].Value);
            det = mat1 + mat2 + mat3 - mat4 - mat5 - mat6;
            label1.Text = "Определитель равен " + Convert.ToString(det);
        }

        public Form1()
        {
            InitializeComponent();
            dataGridView1.BackgroundColor = this.BackColor;
            dataGridView1.BorderStyle = BorderStyle.None;
            dataGridView1.ColumnCount = n;
            dataGridView1.RowCount = m;
            dataGridView1.ColumnHeadersVisible = false;
            dataGridView1.RowHeadersVisible = false;
            dataGridView1.ScrollBars = ScrollBars.None;
            dataGridView1.Width = 320;
            dataGridView1.Height = 160;
            dataGridView1.Font = new Font("Arial", 14);
        }
    }
}
