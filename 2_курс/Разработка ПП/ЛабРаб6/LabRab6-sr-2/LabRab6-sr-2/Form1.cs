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

namespace LabRab6_sr_2
{
    public partial class Form1 : Form
    {
        string filePath = @"D:\Учеба\2 курс\Разработка ПП\ЛабРаб6\LabRab6-sr-2\LabRab6-sr-2\bin\Debug\Прайс лист.txt",
            prodName;
        string[] file;
        int minPrice, maxPrice, price;
        bool flag;

        private void button1_Click(object sender, EventArgs e)
        {
            textBox3.Clear();
            flag = true;
            minPrice = Convert.ToInt32(textBox1.Text);
            maxPrice = Convert.ToInt32(textBox2.Text);
            for (int i = 2; i < file.Length; i++)
            {
                prodName = file[i].Substring(0, 12).Trim();
                price = Convert.ToInt32(file[i].Substring(13).Trim());
                if ((minPrice < price) && (price < maxPrice))
                {
                    textBox3.Text += $"{prodName, -15} {price}" + Environment.NewLine;
                    flag = false;
                }
            }
            if (flag)
                label5.Text = "Подходящих продуктов не найдено";
            else
                label5.Text = "";
        }

        public Form1()
        {
            InitializeComponent();
            label5.Text = "";
            using (StreamReader reader = new StreamReader(filePath))
            {
                file = reader.ReadToEnd().Split('\n');
            }
        }
    }
}
