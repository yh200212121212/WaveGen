namespace WaveGenEditor
{
    partial class KeybordControl
    {
        /// <summary> 
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region コンポーネント デザイナーで生成されたコード

        /// <summary> 
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を 
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.contextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.cmdChangeType0 = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdChangeType1 = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdChangeType2 = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdChangeType3 = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdChangeType4 = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdChangeType5 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.cmdSelectMidiInDev = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // contextMenu
            // 
            this.contextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmdChangeType0,
            this.cmdChangeType1,
            this.cmdChangeType2,
            this.cmdChangeType3,
            this.cmdChangeType4,
            this.cmdChangeType5,
            this.toolStripMenuItem1,
            this.cmdSelectMidiInDev});
            this.contextMenu.Name = "contextMenu";
            this.contextMenu.Size = new System.Drawing.Size(225, 186);
            this.contextMenu.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenu_Opening);
            // 
            // cmdChangeType0
            // 
            this.cmdChangeType0.Checked = true;
            this.cmdChangeType0.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cmdChangeType0.Name = "cmdChangeType0";
            this.cmdChangeType0.Size = new System.Drawing.Size(224, 22);
            this.cmdChangeType0.Text = "@0 Rectangular wave";
            this.cmdChangeType0.Click += new System.EventHandler(this.cmdChangeType0_Click);
            // 
            // cmdChangeType1
            // 
            this.cmdChangeType1.Name = "cmdChangeType1";
            this.cmdChangeType1.Size = new System.Drawing.Size(224, 22);
            this.cmdChangeType1.Text = "@1 Triangle wave";
            this.cmdChangeType1.Click += new System.EventHandler(this.cmdChangeType1_Click);
            // 
            // cmdChangeType2
            // 
            this.cmdChangeType2.Name = "cmdChangeType2";
            this.cmdChangeType2.Size = new System.Drawing.Size(224, 22);
            this.cmdChangeType2.Text = "@2 Sawtooth wave";
            this.cmdChangeType2.Click += new System.EventHandler(this.cmdChangeType2_Click);
            // 
            // cmdChangeType3
            // 
            this.cmdChangeType3.Name = "cmdChangeType3";
            this.cmdChangeType3.Size = new System.Drawing.Size(224, 22);
            this.cmdChangeType3.Text = "@3 sine wave";
            this.cmdChangeType3.Click += new System.EventHandler(this.cmdChangeType3_Click);
            // 
            // cmdChangeType4
            // 
            this.cmdChangeType4.Name = "cmdChangeType4";
            this.cmdChangeType4.Size = new System.Drawing.Size(224, 22);
            this.cmdChangeType4.Text = "@4 Long FC noises";
            this.cmdChangeType4.Click += new System.EventHandler(this.cmdChangeType4_Click);
            // 
            // cmdChangeType5
            // 
            this.cmdChangeType5.Name = "cmdChangeType5";
            this.cmdChangeType5.Size = new System.Drawing.Size(224, 22);
            this.cmdChangeType5.Text = "@5 Short FC noises";
            this.cmdChangeType5.Click += new System.EventHandler(this.cmdChangeType5_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(221, 6);
            // 
            // cmdSelectMidiInDev
            // 
            this.cmdSelectMidiInDev.Name = "cmdSelectMidiInDev";
            this.cmdSelectMidiInDev.Size = new System.Drawing.Size(224, 22);
            this.cmdSelectMidiInDev.Text = "On the MIDI IN device settings";
            this.cmdSelectMidiInDev.Click += new System.EventHandler(this.cmdSelectMidiInDev_Click);
            // 
            // KeybordControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ContextMenuStrip = this.contextMenu;
            this.DoubleBuffered = true;
            this.Name = "KeybordControl";
            this.Size = new System.Drawing.Size(285, 39);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.KeybordControl_Paint);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.KeybordControl_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.KeybordControl_MouseMove);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.KeybordControl_MouseUp);
            this.Resize += new System.EventHandler(this.KeybordControl_Resize);
            this.contextMenu.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip contextMenu;
        private System.Windows.Forms.ToolStripMenuItem cmdChangeType0;
        private System.Windows.Forms.ToolStripMenuItem cmdChangeType1;
        private System.Windows.Forms.ToolStripMenuItem cmdChangeType2;
        private System.Windows.Forms.ToolStripMenuItem cmdChangeType3;
        private System.Windows.Forms.ToolStripMenuItem cmdChangeType4;
        private System.Windows.Forms.ToolStripMenuItem cmdChangeType5;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem cmdSelectMidiInDev;
    }
}
