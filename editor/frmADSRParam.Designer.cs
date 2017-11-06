namespace WaveGenEditor
{
    partial class frmADSRParam
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
            this.picGraphic = new System.Windows.Forms.PictureBox();
            this.cmdClipCopy = new System.Windows.Forms.Button();
            this.cmdClose = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txtRTime = new System.Windows.Forms.TextBox();
            this.txtSPower = new System.Windows.Forms.TextBox();
            this.txtDTime = new System.Windows.Forms.TextBox();
            this.txtATime = new System.Windows.Forms.TextBox();
            this.txtAPower = new System.Windows.Forms.TextBox();
            this.sldRTime = new System.Windows.Forms.TrackBar();
            this.sldSPower = new System.Windows.Forms.TrackBar();
            this.sldDTime = new System.Windows.Forms.TrackBar();
            this.sldATime = new System.Windows.Forms.TrackBar();
            this.sldAPower = new System.Windows.Forms.TrackBar();
            this.keyPreview = new WaveGenEditor.KeybordControl();
            ((System.ComponentModel.ISupportInitialize)(this.picGraphic)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sldRTime)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldSPower)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldDTime)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldATime)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldAPower)).BeginInit();
            this.SuspendLayout();
            // 
            // picGraphic
            // 
            this.picGraphic.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.picGraphic.BackColor = System.Drawing.Color.Black;
            this.picGraphic.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.picGraphic.ErrorImage = null;
            this.picGraphic.InitialImage = null;
            this.picGraphic.Location = new System.Drawing.Point(12, 16);
            this.picGraphic.Name = "picGraphic";
            this.picGraphic.Size = new System.Drawing.Size(404, 226);
            this.picGraphic.TabIndex = 15;
            this.picGraphic.TabStop = false;
            this.picGraphic.Paint += new System.Windows.Forms.PaintEventHandler(this.picGraphic_Paint);
            this.picGraphic.Resize += new System.EventHandler(this.picGraphic_Resize);
            // 
            // cmdClipCopy
            // 
            this.cmdClipCopy.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmdClipCopy.Location = new System.Drawing.Point(481, 298);
            this.cmdClipCopy.Name = "cmdClipCopy";
            this.cmdClipCopy.Size = new System.Drawing.Size(110, 23);
            this.cmdClipCopy.TabIndex = 10;
            this.cmdClipCopy.Text = "In MML syntax copy";
            this.cmdClipCopy.UseVisualStyleBackColor = true;
            this.cmdClipCopy.Click += new System.EventHandler(this.cmdClipCopy_Click);
            // 
            // cmdClose
            // 
            this.cmdClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmdClose.Location = new System.Drawing.Point(597, 298);
            this.cmdClose.Name = "cmdClose";
            this.cmdClose.Size = new System.Drawing.Size(75, 23);
            this.cmdClose.TabIndex = 11;
            this.cmdClose.Text = "Close up";
            this.cmdClose.UseVisualStyleBackColor = true;
            this.cmdClose.Click += new System.EventHandler(this.cmdClose_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.txtRTime);
            this.groupBox1.Controls.Add(this.txtSPower);
            this.groupBox1.Controls.Add(this.txtDTime);
            this.groupBox1.Controls.Add(this.txtATime);
            this.groupBox1.Controls.Add(this.txtAPower);
            this.groupBox1.Controls.Add(this.sldRTime);
            this.groupBox1.Controls.Add(this.sldSPower);
            this.groupBox1.Controls.Add(this.sldDTime);
            this.groupBox1.Controls.Add(this.sldATime);
            this.groupBox1.Controls.Add(this.sldAPower);
            this.groupBox1.Location = new System.Drawing.Point(422, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(250, 275);
            this.groupBox1.TabIndex = 19;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Parameter";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(14, 222);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(34, 12);
            this.label5.TabIndex = 29;
            this.label5.Text = "rTime";
            this.label5.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 171);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(42, 12);
            this.label4.TabIndex = 28;
            this.label4.Text = "sPower";
            this.label4.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 117);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(36, 12);
            this.label3.TabIndex = 27;
            this.label3.Text = "dTime";
            this.label3.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 69);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(36, 12);
            this.label2.TabIndex = 26;
            this.label2.Text = "aTime";
            this.label2.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(42, 12);
            this.label1.TabIndex = 25;
            this.label1.Text = "aPower";
            this.label1.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // txtRTime
            // 
            this.txtRTime.Location = new System.Drawing.Point(8, 237);
            this.txtRTime.MaxLength = 10;
            this.txtRTime.Name = "txtRTime";
            this.txtRTime.Size = new System.Drawing.Size(40, 19);
            this.txtRTime.TabIndex = 8;
            this.txtRTime.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.txtRTime.TextChanged += new System.EventHandler(this.txtParamTextChanged);
            // 
            // txtSPower
            // 
            this.txtSPower.Location = new System.Drawing.Point(8, 186);
            this.txtSPower.MaxLength = 10;
            this.txtSPower.Name = "txtSPower";
            this.txtSPower.Size = new System.Drawing.Size(40, 19);
            this.txtSPower.TabIndex = 6;
            this.txtSPower.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.txtSPower.TextChanged += new System.EventHandler(this.txtParamTextChanged);
            // 
            // txtDTime
            // 
            this.txtDTime.Location = new System.Drawing.Point(8, 132);
            this.txtDTime.MaxLength = 10;
            this.txtDTime.Name = "txtDTime";
            this.txtDTime.Size = new System.Drawing.Size(40, 19);
            this.txtDTime.TabIndex = 4;
            this.txtDTime.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.txtDTime.TextChanged += new System.EventHandler(this.txtParamTextChanged);
            // 
            // txtATime
            // 
            this.txtATime.Location = new System.Drawing.Point(8, 84);
            this.txtATime.MaxLength = 10;
            this.txtATime.Name = "txtATime";
            this.txtATime.Size = new System.Drawing.Size(40, 19);
            this.txtATime.TabIndex = 2;
            this.txtATime.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.txtATime.TextChanged += new System.EventHandler(this.txtParamTextChanged);
            // 
            // txtAPower
            // 
            this.txtAPower.Location = new System.Drawing.Point(8, 33);
            this.txtAPower.MaxLength = 10;
            this.txtAPower.Name = "txtAPower";
            this.txtAPower.Size = new System.Drawing.Size(40, 19);
            this.txtAPower.TabIndex = 0;
            this.txtAPower.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.txtAPower.TextChanged += new System.EventHandler(this.txtParamTextChanged);
            // 
            // sldRTime
            // 
            this.sldRTime.Location = new System.Drawing.Point(54, 222);
            this.sldRTime.Maximum = 100;
            this.sldRTime.Name = "sldRTime";
            this.sldRTime.Size = new System.Drawing.Size(190, 45);
            this.sldRTime.TabIndex = 9;
            this.sldRTime.TickFrequency = 0;
            this.sldRTime.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.sldRTime.ValueChanged += new System.EventHandler(this.trackBarValueChanged);
            // 
            // sldSPower
            // 
            this.sldSPower.Location = new System.Drawing.Point(54, 171);
            this.sldSPower.Maximum = 100;
            this.sldSPower.Name = "sldSPower";
            this.sldSPower.Size = new System.Drawing.Size(190, 45);
            this.sldSPower.TabIndex = 7;
            this.sldSPower.TickFrequency = 0;
            this.sldSPower.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.sldSPower.ValueChanged += new System.EventHandler(this.trackBarValueChanged);
            // 
            // sldDTime
            // 
            this.sldDTime.Location = new System.Drawing.Point(54, 120);
            this.sldDTime.Maximum = 100;
            this.sldDTime.Name = "sldDTime";
            this.sldDTime.Size = new System.Drawing.Size(190, 45);
            this.sldDTime.TabIndex = 5;
            this.sldDTime.TickFrequency = 0;
            this.sldDTime.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.sldDTime.ValueChanged += new System.EventHandler(this.trackBarValueChanged);
            // 
            // sldATime
            // 
            this.sldATime.Location = new System.Drawing.Point(54, 69);
            this.sldATime.Maximum = 100;
            this.sldATime.Name = "sldATime";
            this.sldATime.Size = new System.Drawing.Size(190, 45);
            this.sldATime.TabIndex = 3;
            this.sldATime.TickFrequency = 0;
            this.sldATime.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.sldATime.ValueChanged += new System.EventHandler(this.trackBarValueChanged);
            // 
            // sldAPower
            // 
            this.sldAPower.Location = new System.Drawing.Point(54, 18);
            this.sldAPower.Maximum = 100;
            this.sldAPower.Name = "sldAPower";
            this.sldAPower.Size = new System.Drawing.Size(190, 45);
            this.sldAPower.TabIndex = 1;
            this.sldAPower.TickFrequency = 0;
            this.sldAPower.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.sldAPower.ValueChanged += new System.EventHandler(this.trackBarValueChanged);
            // 
            // keyPreview
            // 
            this.keyPreview.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.keyPreview.BackColor = System.Drawing.Color.White;
            this.keyPreview.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.keyPreview.KeysCount = ((byte)(88));
            this.keyPreview.Location = new System.Drawing.Point(12, 248);
            this.keyPreview.Name = "keyPreview";
            this.keyPreview.Size = new System.Drawing.Size(404, 39);
            this.keyPreview.TabIndex = 20;
            this.keyPreview.TabStop = false;
            this.keyPreview.EventNoteOn += new WaveGenEditor.KeybordControl.EventHandlerNoteOnOff(this.keyPreview_EventNoteOn);
            this.keyPreview.EventNoteOff += new WaveGenEditor.KeybordControl.EventHandlerNoteOnOff(this.keyPreview_EventNoteOff);
            this.keyPreview.EventChangeType += new WaveGenEditor.KeybordControl.EventHandlerChangeType(this.keyPreview_EventChangeType);
            // 
            // frmADSRParam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(684, 333);
            this.Controls.Add(this.keyPreview);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.cmdClose);
            this.Controls.Add(this.cmdClipCopy);
            this.Controls.Add(this.picGraphic);
            this.DoubleBuffered = true;
            this.MinimumSize = new System.Drawing.Size(700, 371);
            this.Name = "frmADSRParam";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "Parameter Edit ADSR";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.frmADSRParam_FormClosing);
            this.Load += new System.EventHandler(this.frmADSRParam_Load);
            ((System.ComponentModel.ISupportInitialize)(this.picGraphic)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.sldRTime)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldSPower)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldDTime)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldATime)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldAPower)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox picGraphic;
        private System.Windows.Forms.Button cmdClipCopy;
        private System.Windows.Forms.Button cmdClose;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtRTime;
        private System.Windows.Forms.TextBox txtSPower;
        private System.Windows.Forms.TextBox txtDTime;
        private System.Windows.Forms.TextBox txtATime;
        private System.Windows.Forms.TextBox txtAPower;
        private System.Windows.Forms.TrackBar sldRTime;
        private System.Windows.Forms.TrackBar sldSPower;
        private System.Windows.Forms.TrackBar sldDTime;
        private System.Windows.Forms.TrackBar sldATime;
        private System.Windows.Forms.TrackBar sldAPower;
        private KeybordControl keyPreview;
    }
}