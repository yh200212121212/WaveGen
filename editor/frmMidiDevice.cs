using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using jp.ropo;

namespace WaveGenEditor
{
    public partial class frmMidiDevice : Form
    {
        /// <summary>
        /// The device is selected as the index
        /// </summary>
        public int SelectedIndex { get; set; }
        /// <summary>
        /// If fixed velocity. If the byte enable to MaxValue specified velocity to 127 .
        /// </summary>
        public byte VelocityConst { get; set; }

        public frmMidiDevice()
        {
            InitializeComponent();
            SelectedIndex = 0;
        }

        private void frmMidiDevice_Load(object sender, EventArgs e)
        {
            int count = Win32MidiInPort.InputCount;
            cmbMidiin.Items.Add("0: No");

            for( int i=0; i<count; i++ ) {
                var caps = new Win32MidiInPort.DeviceCaps();
                caps = Win32MidiInPort.GetDeviceInfo(i);
                if (caps == null)
                    cmbMidiin.Items.Add((i + 1).ToString() + ": Unknown device");
                else
                    cmbMidiin.Items.Add( (i+1).ToString() + ": " + caps.deviceName);
            }
            cmbMidiin.SelectedIndex = SelectedIndex;
            chkUnuseVelocity.Checked = (VelocityConst != byte.MaxValue);
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            SelectedIndex = cmbMidiin.SelectedIndex;
            VelocityConst = chkUnuseVelocity.Checked ? (byte)127 : byte.MaxValue;
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }

        private void cmdCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }
    }
}
