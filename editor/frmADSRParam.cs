using System;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace WaveGenEditor
{
    public partial class frmADSRParam : Form
    {
        private Bitmap backBuffer;
        private bool isValueCahnged = false;
        private WaveGenIF waveGen = null;

        public frmADSRParam()
        {
            InitializeComponent();
        }

        private void cmdClose_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void frmADSRParam_Load(object sender, EventArgs e)
        {
            waveGen = new WaveGenIF();
            waveGen.CreateWaveGen(Handle);

            txtAPower.Text = "1.00";
            txtATime.Text = "0.05";
            txtDTime.Text = "0.03";
            txtSPower.Text = "0.80";
            txtRTime.Text = "0.10";

            drawADSR(picGraphic);
        }

        private void frmADSRParam_FormClosing(object sender, FormClosingEventArgs e)
        {
            waveGen.Release();
            backBuffer.Dispose();
        }

        private void drawADSR(PictureBox picbox)
        {
            float aPower, aTime, dTime, sPower, sTime, rTime;

            try
            {
                aPower = float.Parse(txtAPower.Text);
                aTime = float.Parse(txtATime.Text);
                dTime = float.Parse(txtDTime.Text);
                sPower = float.Parse(txtSPower.Text);
                rTime = float.Parse(txtRTime.Text);
            }
            catch
            {
                return;
            }

            if (backBuffer != null)
                backBuffer.Dispose();
            int w = picbox.Width;
            int h = picbox.Height;

            backBuffer = new Bitmap(w, h);
            Graphics grp = Graphics.FromImage(backBuffer);

            float totalTime, maxPower, xDiv, yDiv;
            sTime = (aTime + dTime + rTime) / 1.5f;
            if (sTime <= 0)
                sTime = 1;
            totalTime = aTime + dTime + rTime + sTime;
            maxPower = (aPower > sPower) ? aPower : sPower;
            xDiv = totalTime / w;
            yDiv = maxPower / h;

            // To make a graph vertex
            PointF[] points = new PointF[5];
            points[0].X = 0;
            points[0].Y = h - 1;
            points[1].X = aTime / xDiv;
            points[1].Y = h - aPower / yDiv;
            points[2].X = (aTime + dTime) / xDiv;
            points[2].Y = h - sPower / yDiv;
            points[3].X = (aTime + dTime + sTime) / xDiv;
            points[3].Y = points[2].Y;
            points[4].X = w - 1;
            points[4].Y = points[0].Y;

            Font guideFont = new Font("ＭＳ ゴシック", 9);
            // Draw!
            Rectangle wh = new Rectangle(0, 0, w, h);
            LinearGradientBrush gbBack = new LinearGradientBrush(wh, Color.DimGray, Color.Black, LinearGradientMode.Vertical);
            grp.FillRectangle(gbBack, wh);
            gbBack.Dispose();
            LinearGradientBrush gbGraph = new LinearGradientBrush(wh, Color.SteelBlue, Color.DodgerBlue, LinearGradientMode.Vertical);
            grp.FillPolygon(gbGraph, points);
            gbGraph.Dispose();

            DrawClipString(grp, guideFont, "aPower", Brushes.White, picbox.ClientRectangle, points[1]);
            DrawClipString(grp, guideFont, "sPower", Brushes.White, picbox.ClientRectangle, points[2]);

            picbox.Invalidate();
        }

        public void DrawClipString(Graphics grp, Font font, string str, Brush brush, Rectangle frame, PointF pos)
        {
            DrawClipString(grp, font, str, brush, frame, new Point((int)pos.X, (int)pos.Y));
        }

        public void DrawClipString(Graphics grp, Font font, string str, Brush brush, Rectangle frame, Point pos)
        {
            grp.DrawString(str, font, brush, CaclClipString(grp, font, str, frame, pos));
        }

        public Point CaclClipString(Graphics grp, Font font, string str, Rectangle frame, Point pos)
        {
            SizeF sz = grp.MeasureString(str, font);
            pos.X -= (int)(sz.Width / 2);
            pos.Y -= (int)(sz.Height / 2);
            if (pos.X - sz.Width < 0)
                pos.X = 0;
            else if (pos.X + sz.Width > frame.Width)
                pos.X = frame.Width - (int)sz.Width;
            if (pos.Y - sz.Height < 0)
                pos.Y = 0;
            else if (pos.Y + sz.Height > frame.Height)
                pos.Y = frame.Height - (int)sz.Height;
            return pos;
        }

        private void trackBarValueChanged(object sender, EventArgs e)
        {
            TrackBar control = (TrackBar)sender;
            TextBox textBox = null;
            switch (control.Name)
            {
                case "sldAPower": textBox = txtAPower; break;
                case "sldATime": textBox = txtATime; break;
                case "sldDTime": textBox = txtDTime; break;
                case "sldSPower": textBox = txtSPower; break;
                case "sldRTime": textBox = txtRTime; break;
            }
            if (textBox != null && isValueCahnged == false)
            {
                isValueCahnged = true;
                textBox.Text = (control.Value / 100.0f).ToString("F2");
                isValueCahnged = false;
            }
        }

        private void txtParamTextChanged(object sender, EventArgs e)
        {
            TextBox control = (TextBox)sender;
            TrackBar trackBar = null;
            switch (control.Name)
            {
                case "txtAPower": trackBar = sldAPower; break;
                case "txtATime": trackBar = sldATime; break;
                case "txtDTime": trackBar = sldDTime; break;
                case "txtSPower": trackBar = sldSPower; break;
                case "txtRTime": trackBar = sldRTime; break;
            }
            if (trackBar != null)
            {
                try
                {
                    int val = (int)(float.Parse(control.Text) * 100);
                    if (isValueCahnged == false && val >= trackBar.Minimum && val <= trackBar.Maximum)
                    {
                        isValueCahnged = true;
                        trackBar.Value = val;
                        isValueCahnged = false;
                    }
                    drawADSR(picGraphic);

                    float aPower = float.Parse(txtAPower.Text);
                    float aTime = float.Parse(txtATime.Text);
                    float dTime = float.Parse(txtDTime.Text);
                    float sPower = float.Parse(txtSPower.Text);
                    float rTime = float.Parse(txtRTime.Text);
                    waveGen.PreviewADSR(aPower, aTime, dTime, sPower, rTime);
                }
                catch
                {
                }
            }
        }

        private void picGraphic_Paint(object sender, PaintEventArgs e)
        {
            if (backBuffer == null)
                return;

            e.Graphics.DrawImageUnscaled(backBuffer, 0, 0);
        }

        private void picGraphic_Resize(object sender, EventArgs e)
        {
            if (backBuffer != null)
            {
                backBuffer.Dispose();
                backBuffer = null;
            }
            drawADSR((PictureBox)sender);
        }

        private void cmdClipCopy_Click(object sender, EventArgs e)
        {
            string text = "ADSR( ";
            try
            {
                text += float.Parse(txtAPower.Text).ToString() + ", "
                     + float.Parse(txtATime.Text).ToString() + ", "
                     + float.Parse(txtDTime.Text) + ", "
                     + float.Parse(txtSPower.Text) + ", "
                     + float.Parse(txtRTime.Text) + " )";
            }
            catch
            {
                return;
            }

            Clipboard.SetText(text);
        }

        private void keyPreview_EventNoteOff(object sender, KeybordControl.KeyNoteEventArgs e)
        {
            waveGen.PreviewNoteOff();
        }

        private void keyPreview_EventNoteOn(object sender, KeybordControl.KeyNoteEventArgs e)
        {
            waveGen.PreviewNoteOn(e.note, e.velocity);
        }

        private void keyPreview_EventChangeType(object sender, byte type)
        {
            waveGen.PreviewGenType(type);
        }

    }
}
