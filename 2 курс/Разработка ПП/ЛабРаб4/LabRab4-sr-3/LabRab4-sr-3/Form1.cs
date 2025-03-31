using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab4_sr_3
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            ((DataGridViewComboBoxColumn)dataGridView1.Columns[1]).Items.AddRange(new string[] { "ИС-31", "ИС-32" });
            // Задаем цвет фона компонента:
            dataGridView1.BackgroundColor = this.BackColor;
            // Убираем рамку:
            dataGridView1.BorderStyle = BorderStyle.None;
            // Задаем размеры таблицы:
            dataGridView1.Width = 760;
            dataGridView1.Height = 160;
            // Создаем вертикальную полосу прокрутки:
            dataGridView1.ScrollBars = ScrollBars.Vertical;
            // Задаем выравнивание текста в ячейках по центру:
            dataGridView1.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int i, countOne = 0, countTwo = 0;
            // Если у нас 5 начальных колонок, формируем еще одну
            if (dataGridView1.ColumnCount == 5)
                dataGridView1.Columns.Add("Column5",
                "Допуск к экзамену");
            dataGridView1.Columns[5].Width = 155;
            /* Смотрим во всех строках на результаты сдачи
            лабораторных: */
            for (i = 0; i < dataGridView1.RowCount; i++)
            {
                /* dataGridView1.Rows[i].Cells[2].Value возращает значение "флага",
                 а именно: true, если пользователь выбрал ячейку и false, если нет.
                код "dataGridView1.Rows[i].Cells[2].Value != null" изначально дан с ошибкой*/
                if (Convert.ToBoolean(dataGridView1.Rows[i].Cells[2].Value) == true
                    && Convert.ToBoolean(dataGridView1.Rows[i].Cells[3].Value) == true
                    && Convert.ToBoolean(dataGridView1.Rows[i].Cells[4].Value) == true)
                {
                    dataGridView1.Rows[i].Cells[5].Value = "Допущен";
                    dataGridView1.Rows[i].DefaultCellStyle.BackColor = Color.MistyRose;
                    if (Convert.ToString(dataGridView1.Rows[i].Cells[1].Value) == "ИС-31")
                    {
                        countOne += 1;
                    }
                    if (Convert.ToString(dataGridView1.Rows[i].Cells[1].Value) == "ИС-32")
                    {
                        countTwo += 1;
                    }
                }
                else
                {
                    dataGridView1.Rows[i].Cells[5].Value = "";
                    dataGridView1.Rows[i].DefaultCellStyle.BackColor = Color.White;
                }
            }
            label2.Text = "В группе ИС-31 допущено: " + Convert.ToString(countOne);
            label3.Text = "В группе ИС-32 допущено: " + Convert.ToString(countTwo);
        }
    }
}
