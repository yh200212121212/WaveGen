namespace WaveGenEditor
{
    partial class frmMidiDevice
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.cmdOK = new System.Windows.Forms.Button();
            this.cmdCancel = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.cmbMidiin = new System.Windows.Forms.ComboBox();
            this.chkUnuseVelocity = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // cmdOK
            // 
            this.cmdOK.Location = new System.Drawing.Point(335, 86);
            this.cmdOK.Name = "cmdOK";
            this.cmdOK.Size = new System.Drawing.Size(75, 23);
            this.cmdOK.TabIndex = 0;
            this.cmdOK.Text = "OK";
            this.cmdOK.UseVisualStyleBackColor = true;
            this.cmdOK.Click += new System.EventHandler(this.cmdOK_Click);
            // 
            // cmdCancel
            // 
            this.cmdCancel.Location = new System.Drawing.Point(254, 86);
            this.cmdCancel.Name = "cmdCancel";
            this.cmdCancel.Size = new System.Drawing.Size(75, 23);
            this.cmdCancel.TabIndex = 1;
            this.cmdCancel.Text = "Cancel";
            this.cmdCancel.UseVisualStyleBackColor = true;
            this.cmdCancel.Click += new System.EventHandler(this.cmdCancel_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.chkUnuseVelocity);
            this.groupBox1.Controls.Add(this.cmbMidiin);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(398, 68);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "MIDI In";
            // 
            // cmbMidiin
            // 
            this.cmbMidiin.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbMidiin.FormattingEnabled = true;
            this.cmbMidiin.Location = new System.Drawing.Point(15, 18);
            this.cmbMidiin.Name = "cmbMidiin";
            this.cmbMidiin.Size = new System.Drawing.Size(377, 20);
            this.cmbMidiin.TabIndex = 0;
            // 
            // chkUnuseVelocity
            // 
            this.chkUnuseVelocity.AutoSize = true;
            this.chkUnuseVelocity.Location = new System.Drawing.Point(15, 44);
            this.chkUnuseVelocity.Name = "chkUnuseVelocity";
            this.chkUnuseVelocity.Size = new System.Drawing.Size(133, 16);
            this.chkUnuseVelocity.TabIndex = 3;
            this.chkUnuseVelocity.Text = "Fixed velocity";
            this.chkUnuseVelocity.UseVisualStyleBackColor = true;
            // 
            // frmMidiDevice
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(422, 115);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.cmdCancel);
            this.Controls.Add(this.cmdOK);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "frmMidiDevice";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "MIDI device selection";
            this.Load += new System.EventHandler(this.frmMidiDevice_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button cmdOK;
        private System.Windows.Forms.Button cmdCancel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox cmbMidiin;
        private System.Windows.Forms.CheckBox chkUnuseVelocity;
    }
}