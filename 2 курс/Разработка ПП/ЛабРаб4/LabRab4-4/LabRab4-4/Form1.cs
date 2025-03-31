using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LabRab4_4
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            /* Создаем элементы списка comboBox для второй
            колонки:(не забудьте, индексация строк и столбцов
            начинается с нуля) */
            ((DataGridViewComboBoxColumn)dataGridView1.Columns[1]
            ).Items.AddRange(new string[] { "ИС-31", "ИС-32" });
            // Задаем цвет фона компонента:
            dataGridView1.BackgroundColor = this.BackColor;
            // Убираем рамку:
            dataGridView1.BorderStyle = BorderStyle.None;
            // Задаем размеры таблицы:
            dataGridView1.Width = 1000;
            dataGridView1.Height = 160;
            // Создаем вертикальную полосу прокрутки:
            dataGridView1.ScrollBars = ScrollBars.Vertical;
            // Задаем выравнивание текста в ячейках по центру:
            dataGridView1.DefaultCellStyle.Alignment =
            DataGridViewContentAlignment.MiddleCenter;
            // Все элементы оформления можете и перенастроить
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int i;
            // Если у нас 5 начальных колонок, формируем еще одну
            if (dataGridView1.ColumnCount == 5)
                dataGridView1.Columns.Add("Column5",
                "Допуск к экзамену");
            dataGridView1.Columns[5].Width = 140;
            /* Смотрим во всех строках на результаты сдачи
            лабораторных: */
            for (i = 0; i < dataGridView1.RowCount; i++)
                if (dataGridView1.Rows[i].Cells[2].Value != null
                && dataGridView1.Rows[i].Cells[3].Value != null
                && dataGridView1.Rows[i].Cells[4].Value != null)
                { // Если все сдано- пишем «Допущен» и
                  // закрашиваем строку:
                    dataGridView1.Rows[i].Cells[5].Value= "Допущен";
                    dataGridView1.Rows[i].DefaultCellStyle.BackColor = Color.MistyRose;
                }
        }
    }
}
