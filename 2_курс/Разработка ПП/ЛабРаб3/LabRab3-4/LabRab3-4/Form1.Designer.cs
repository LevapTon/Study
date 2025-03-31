
namespace LabRab3_4
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.label1 = new System.Windows.Forms.Label();
            this.действияToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.загрузитьДанныеToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.операцияToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.сложениеToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.вычитаниеToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.умножениеToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.делениеToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.выходToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.помощьToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.информацияОРазработчикеToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.действияToolStripMenuItem,
            this.помощьToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(245, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(28, 83);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(0, 17);
            this.label1.TabIndex = 1;
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // действияToolStripMenuItem
            // 
            this.действияToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.загрузитьДанныеToolStripMenuItem,
            this.операцияToolStripMenuItem,
            this.toolStripSeparator1,
            this.выходToolStripMenuItem});
            this.действияToolStripMenuItem.Name = "действияToolStripMenuItem";
            this.действияToolStripMenuItem.Size = new System.Drawing.Size(70, 20);
            this.действияToolStripMenuItem.Text = "Действия";
            // 
            // загрузитьДанныеToolStripMenuItem
            // 
            this.загрузитьДанныеToolStripMenuItem.Name = "загрузитьДанныеToolStripMenuItem";
            this.загрузитьДанныеToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.загрузитьДанныеToolStripMenuItem.Text = "Загрузить данные";
            this.загрузитьДанныеToolStripMenuItem.Click += new System.EventHandler(this.загрузитьДанныеToolStripMenuItem_Click);
            // 
            // операцияToolStripMenuItem
            // 
            this.операцияToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.сложениеToolStripMenuItem,
            this.вычитаниеToolStripMenuItem,
            this.умножениеToolStripMenuItem,
            this.делениеToolStripMenuItem});
            this.операцияToolStripMenuItem.Name = "операцияToolStripMenuItem";
            this.операцияToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.операцияToolStripMenuItem.Text = "Операция...";
            // 
            // сложениеToolStripMenuItem
            // 
            this.сложениеToolStripMenuItem.Name = "сложениеToolStripMenuItem";
            this.сложениеToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.сложениеToolStripMenuItem.Text = "Сумма";
            this.сложениеToolStripMenuItem.Click += new System.EventHandler(this.сложениеToolStripMenuItem_Click);
            // 
            // вычитаниеToolStripMenuItem
            // 
            this.вычитаниеToolStripMenuItem.Name = "вычитаниеToolStripMenuItem";
            this.вычитаниеToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.вычитаниеToolStripMenuItem.Text = "Разность";
            this.вычитаниеToolStripMenuItem.Click += new System.EventHandler(this.вычитаниеToolStripMenuItem_Click);
            // 
            // умножениеToolStripMenuItem
            // 
            this.умножениеToolStripMenuItem.Name = "умножениеToolStripMenuItem";
            this.умножениеToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.умножениеToolStripMenuItem.Text = "Произведение";
            this.умножениеToolStripMenuItem.Click += new System.EventHandler(this.умножениеToolStripMenuItem_Click);
            // 
            // делениеToolStripMenuItem
            // 
            this.делениеToolStripMenuItem.Name = "делениеToolStripMenuItem";
            this.делениеToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.делениеToolStripMenuItem.Text = "Частное";
            this.делениеToolStripMenuItem.Click += new System.EventHandler(this.делениеToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(177, 6);
            // 
            // выходToolStripMenuItem
            // 
            this.выходToolStripMenuItem.Name = "выходToolStripMenuItem";
            this.выходToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.выходToolStripMenuItem.Text = "Выход";
            this.выходToolStripMenuItem.Click += new System.EventHandler(this.выходToolStripMenuItem_Click);
            // 
            // помощьToolStripMenuItem
            // 
            this.помощьToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.информацияОРазработчикеToolStripMenuItem});
            this.помощьToolStripMenuItem.Name = "помощьToolStripMenuItem";
            this.помощьToolStripMenuItem.Size = new System.Drawing.Size(68, 20);
            this.помощьToolStripMenuItem.Text = "Помощь";
            // 
            // информацияОРазработчикеToolStripMenuItem
            // 
            this.информацияОРазработчикеToolStripMenuItem.Name = "информацияОРазработчикеToolStripMenuItem";
            this.информацияОРазработчикеToolStripMenuItem.Size = new System.Drawing.Size(237, 22);
            this.информацияОРазработчикеToolStripMenuItem.Text = "Информация о разработчике";
            this.информацияОРазработчикеToolStripMenuItem.Click += new System.EventHandler(this.информацияОРазработчикеToolStripMenuItem_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.ClientSize = new System.Drawing.Size(245, 169);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.menuStrip1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(254)));
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "Form1";
            this.Text = "Главная форма";
            this.Activated += new System.EventHandler(this.Form1_Activated);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ToolStripMenuItem действияToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem загрузитьДанныеToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem операцияToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem сложениеToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem вычитаниеToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem умножениеToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem делениеToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem выходToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem помощьToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem информацияОРазработчикеToolStripMenuItem;
    }
}

