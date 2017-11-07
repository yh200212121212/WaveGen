using System;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using System.Text;
using WaveGenEditor.Properties;

namespace WaveGenEditor
{
    /// <summary>
    /// The startup form
    /// </summary>
    public partial class frmMain : Form
    {
        private WaveGenIF waveGen = null;
        private bool isWaveSave = false;
        public void Add()
        {
            // 
            // keyPreview
            // 
            this.keyPreview.BackColor = System.Drawing.Color.White;
            this.keyPreview.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.keyPreview.KeysCount = ((byte)(88));
            this.keyPreview.Location = new System.Drawing.Point(12, 33);
            this.keyPreview.Name = "keyPreview";
            this.keyPreview.Size = new System.Drawing.Size(357, 35);
            this.keyPreview.TabIndex = 5;
            this.keyPreview.TabStop = false;
            this.keyPreview.EventNoteOn += new WaveGenEditor.KeybordControl.EventHandlerNoteOnOff(this.keyPreview_EventNoteOn);
            this.keyPreview.EventNoteOff += new WaveGenEditor.KeybordControl.EventHandlerNoteOnOff(this.keyPreview_EventNoteOff);
            this.keyPreview.EventChangeType += new WaveGenEditor.KeybordControl.EventHandlerChangeType(this.keyPreview_EventChangeType);
            this.Controls.Add(this.keyPreview);
        }
        /// <summary>
        /// Constructor
        /// </summary>
        public frmMain()
        {
            Add();
            InitializeComponent();
        }

        /// <summary>
        /// Loading platform
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void frmMain_Load(object sender, EventArgs e)
        {
            rchMML.LanguageOption = RichTextBoxLanguageOptions.UIFonts;
            rchMML.Font = new Font(Resources.DefaultFontName, 9);
            waveGen = new WaveGenIF();
            waveGen.CreateWaveGen(Handle);
        }

        /// <summary>
        /// Short cut key handling
        /// </summary>
        /// <param name="msg"></param>
        /// <param name="keyData"></param>
        /// <returns></returns>
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            // F5 key[Play]
            if ((int)keyData == (int)Keys.F5)
            {
                cmdPlay.Checked = !cmdPlay.Checked;
                return true;
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        /// <summary>
        /// [Open]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdOpenFile_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK != cmnOpenDlg.ShowDialog())
                return;

            rchMML.Text = File.ReadAllText(cmnOpenDlg.FileName, Encoding.GetEncoding("shift_jis"));
            cmnSaveDlg.FileName = cmnOpenDlg.FileName;
            stsNotify.Text = Resources.Loaded;
        }

        /// <summary>
        /// [Save]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdSaveFile_Click(object sender, EventArgs e)
        {
            if (cmnSaveDlg.FileName.Trim() == "")
            {
                if (DialogResult.OK != cmnSaveDlg.ShowDialog())
                    return;
            }
            MMLSave(cmnSaveDlg.FileName);
        }

        /// <summary>
        /// Save MML
        /// </summary>
        /// <param name="filename">File name</param>
        private void MMLSave(string filename)
        {
            try
            {
                File.WriteAllText(filename, rchMML.Text, Encoding.GetEncoding("shift_jis"));
                stsNotify.Text = Resources.Saved;
                cmnSaveDlg.FileName = cmnOpenDlg.FileName = cmnSaveDlg.FileName;
            }
            catch (Exception)
            {
            }
        }


        /// <summary>
        /// [Save as]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdSaveAsFile_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK != cmnSaveDlg.ShowDialog())
                return;

            MMLSave(cmnSaveDlg.FileName);
        }

        /// <summary>
        /// [End]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdAppExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// [Web]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdOpenGithub_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/ropo/WaveGen");
        }

        /// <summary>
        /// [Web]
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdOpenDL_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/ropo/WaveGen/downloads");
        }

        private void cmdOpenMMLref_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/ropo/WaveGen/wiki/MML");
        }

        /// <summary>
        /// Destruction form
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>

        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (waveGen != null)
            {
                waveGen.Release();
                waveGen = null;
            }
        }

        /// <summary>
        /// Play
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdPlay_CheckedChanged(object sender, EventArgs e)
        {
            if (cmdPlay.Checked)
            {
                if (chkAutoSave.Checked && isWaveSave == false)
                    cmdSaveFile.PerformClick();

                cmdPlay.Text = Resources.StopButtonText + " (F5)";
                Int32 errorCode = 0;
                UInt32 errorLine = 0;
                if (waveGen.CompileMML(rchMML.Text, ref errorCode, ref errorLine, isWaveSave, dlgPlayFinished_, this))
                {
                    stsNotify.Text = Resources.MMLerror.Replace("_LINE_", errorLine.ToString()).Replace("_MSG_", WaveGenIF.GetErrorString(errorCode));
                    MoveLine((int)errorLine);
                    System.Media.SystemSounds.Exclamation.Play();
                    cmdPlay.Checked = false;
                    return;
                }
                stsNotify.Text = "";
            }
            else
            {
                if (waveGen != null)
                    waveGen.Stop();
                cmdPlay.Text = Resources.PlayButtonText + " (F5)";
            }
        }
        static void dlgPlayFinished_(WaveGenIF WaveGen, object parent)
        {
            frmMain pThis = (frmMain)parent;
            pThis.dlgPlayfinished(WaveGen);
        }
        delegate void dlgPlayfinishedInvoke(WaveGenIF WaveGen);
        void dlgPlayfinished(WaveGenIF WaveGen)
        {
            BeginInvoke((MethodInvoker)delegate
            {
                if (cmdPlay.Checked)
                    cmdPlay.Checked = false;
            });
        }

        /// <summary>
        /// Jump to a specified row
        /// </summary>
        /// <param name="line">Jump to the line</param>
        private void MoveLine(int line)
        {
            string str = rchMML.Text;

            int row = 1, startPos = 0, endPos;
            for (endPos = 0; (endPos = str.IndexOf('\n', startPos)) > -1; row++)
            {
                if (line == row)
                {
                    rchMML.SelectionStart = startPos;
                    return;
                }

                startPos = endPos + 1;
            }
        }

        /// <summary>
        /// Write the recording WaveFile
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cmdWriteWaveFile_Click(object sender, EventArgs e)
        {
            if (cmdPlay.Checked)
                cmdPlay.Checked = false;

            if (DialogResult.OK != cmnWavSaveDlg.ShowDialog())
                return;

            waveGen.SetWaveFileName(cmnWavSaveDlg.FileName);

            isWaveSave = true;
            cmdPlay.Checked = true;
            isWaveSave = false;
        }
        private void cmdADSRParamEdit_Click(object sender, EventArgs e)
        {
            if (cmdADSRParamEdit.Checked)
                return;

            frmADSRParam frm = new frmADSRParam();
            frm.FormClosing += new FormClosingEventHandler(delegate { cmdADSRParamEdit.Checked = false; });
            cmdADSRParamEdit.Checked = true;
            frm.Show(this);
        }

        private void cmdEditCopy_Click(object sender, EventArgs e)
        {
            rchMML.Copy();
        }

        private void cmdEditPaste_Click(object sender, EventArgs e)
        {
            rchMML.Paste();
        }

        private void cmdEditCut_Click(object sender, EventArgs e)
        {
            rchMML.Cut();
        }

        private void cmdMenuPlay_Click(object sender, EventArgs e)
        {
            cmdPlay.Checked = !cmdPlay.Checked;
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

        private void cmdPreviewPiano_CheckedChanged(object sender, EventArgs e)
        {
            if (cmdPreviewPiano.Checked)
            {
                keyPreview.Visible = true;
            }
            else
            {
                keyPreview.Visible = false;
            }
        }

        private void aboutToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            AboutBox1 aboutbox = new AboutBox1();
            aboutbox.Show();
        }
    }
}
