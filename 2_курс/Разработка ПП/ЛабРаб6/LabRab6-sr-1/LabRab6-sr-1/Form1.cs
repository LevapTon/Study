using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace LabRab6_sr_1
{
    public partial class Form1 : Form
    {
        string filePath = @"D:\Учеба\2 курс\Разработка ПП\ЛабРаб6\LabRab6-sr-1\LabRab6-sr-1\bin\Debug\Оценки.txt",
            studName;
        string[] file;
        int gradeOne, gradeTwo;

        private void button1_Click(object sender, EventArgs e)
        {
            for (int i = 2; i <file.Length; i++)
            {
                studName = file[i].Substring(0, 10).Trim();
                gradeOne = Convert.ToInt32(file[i].Substring(20, 10).Trim());
                gradeTwo = Convert.ToInt32(file[i].Substring(30).Trim());
                textBox1.Text += $"Студент: {studName} Средний балл: {Convert.ToDecimal((gradeOne + gradeTwo) / 2):0.00}" + Environment.NewLine;
            }
        }

        public Form1()
        {
            InitializeComponent();
            using (StreamReader reader = new StreamReader(filePath))
            {
                file = reader.ReadToEnd().Split('\n');
            }
        }
    }
}
