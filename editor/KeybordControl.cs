using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using jp.ropo;

namespace WaveGenEditor
{
    public partial class KeybordControl : UserControl
    {
        private Bitmap backBuffer;
        private int onNote;
        private float keyPitch;
        private byte keysCount = 88;
        private bool isCNoteDark = true;

        [Description("Specifies the number of keyboard")]
        [DefaultValue(88)]
        public byte KeysCount
        {
            get { return keysCount; }
            set
            {
                if (value == keysCount)
                    return;
                keysCount = value;
                drawKeys();
            }
        }
        [Description("True sound of little black")]
        [DefaultValue(true)]
        public bool CNoteDark
        {
            get { return isCNoteDark; }
            set
            {
                if (value == isCNoteDark)
                    return;
                isCNoteDark = value;
                drawKeys();
            }
        }
        public delegate void EventHandlerNoteOnOff(object sender, KeyNoteEventArgs e);
        [Description("Occurs when a key is depressed.")]
        public event EventHandlerNoteOnOff EventNoteOn;
        [Description("When the key is released")]
        public event EventHandlerNoteOnOff EventNoteOff;

        public delegate void EventHandlerChangeType(object sender, byte type);
        [Description("The waveform generation changed at will")]
        public event EventHandlerChangeType EventChangeType;

        public class KeyNoteEventArgs
        {
            public KeyNoteEventArgs(byte note, byte velocity=127)
            {
                this.note = note;
                this.velocity = velocity;
            }
            /// <summary>
            /// Key number
            /// </summary>
            public byte note;

            /// <summary>
            /// Velocity 0-127
            /// </summary>
            public byte velocity;
        }

        private Win32MidiInPort midIn = new Win32MidiInPort();
        private byte midiInLastOnKey = byte.MaxValue;
        private int midiInDev = -1;
        private byte midiVelocityConst = byte.MaxValue;
        private bool MidiInAttach()
        {
            if( midiInDev >= 0 )
            {
                if (midIn.Open(midiInDev) == false)
                    return false;
                midIn.OnNoteChange += delegate(byte ch, byte note, byte velocity, bool isOn)
                {
                    BeginInvoke((MethodInvoker)delegate
                    {
                        if (ch != 0)
                            return;
                        if (isOn)
                        {
                            NoteOn(note, midiVelocityConst == byte.MaxValue ? velocity : midiVelocityConst);
                            midiInLastOnKey = note;
                        }
                        else
                        {
                            if (midiInLastOnKey == note)
                                NoteOff();
                        }
                    });
                };
                midIn.Start();
            }
            return true;
        }
        private void MidiInDetach()
        {
            midIn.Close();
        }

        public KeybordControl()
        {
            onNote = -1;
            InitializeComponent();
        }

        private bool GetNotePosition(int note, out Rectangle rect)
        {
            rect = new Rectangle();
            int[] xTbl = new int[] { 0, 4, 10, 16, 20, 30, 34, 40, 45, 50, 56, 60 };

            int x = xTbl[note % 12];
            bool isBlack = (x % 10) != 0;
            rect.X = (int)((note / 12) * 70) + x;
            rect.Y = -1;
            rect.Width = (int)(isBlack ? 8 : 10);
            rect.Height = (int)(isBlack ? this.Height * 0.6f : this.Height);

            rect.X = (int)Math.Ceiling(rect.X * (float)keyPitch / 10);
            rect.Width = (int)Math.Ceiling(rect.Width * (float)keyPitch / 10);

            return isBlack;
        }

        private void drawKeys()
        {
            if (backBuffer != null)
                backBuffer.Dispose();
            int w = this.Width;
            int h = this.Height;

            backBuffer = new Bitmap(w, h);
            Graphics grp = Graphics.FromImage(backBuffer);


            // 
            keyPitch = w / ((keysCount / 12f) * 7f);

            //
            Rectangle rect;
            for (int note = 0; note < keysCount; note++)
            {
                if (!GetNotePosition(note, out rect))
                {
                    if (note == onNote)
                    {
                        grp.FillRectangle(Brushes.LightCoral, rect);
                    }
                    else
                    {
                        if (isCNoteDark && note % 12 == 0)
                            grp.FillRectangle(Brushes.Gainsboro, rect);
                        else
                            grp.FillRectangle(Brushes.White, rect);
                    }
                    grp.DrawRectangle(Pens.Black, rect.X, rect.Y, rect.Width, rect.Height);
                }
            }
            for (int note = 0; note < keysCount; note++)
            {
                if (GetNotePosition(note, out rect))
                {
                    if (note == onNote)
                    {
                        grp.FillRectangle(Brushes.Firebrick, rect);
                    }
                    else
                    {
                        grp.FillRectangle(Brushes.Black, rect);
                    }
                    grp.DrawRectangle(Pens.White, rect.X, rect.Y, rect.Width, rect.Height);
                }
            }

            // Discarded resources
            grp.Dispose();

            // Redraw
            this.Invalidate();
        }

        private void KeybordControl_Resize(object sender, EventArgs e)
        {
            if (backBuffer != null)
            {
                backBuffer.Dispose();
                backBuffer = null;
            }
            drawKeys();
        }

        private void KeybordControl_Paint(object sender, PaintEventArgs e)
        {
            if (backBuffer == null)
                return;

            e.Graphics.DrawImageUnscaled(backBuffer, 0, 0);
        }

        private int HitNote(int x, int y)
        {
            int pressNote = -1;
            for (int note = 0; note < keysCount; note++)
            {
                Rectangle rect;
                bool isBlack = GetNotePosition(note, out rect);
                if (rect.Contains(x, y))
                {
                    if (isBlack || pressNote != -1)
                        return note;
                    pressNote = note;
                }
            }
            return pressNote;
        }

        private void NoteOn(int note, byte velocity)
        {
            if (note == onNote)
                return;
            if (onNote != -1)
                NoteOff();
            if (note == -1)
                return;
            onNote = note;
            if (EventNoteOn != null)
                EventNoteOn(this, new KeyNoteEventArgs((byte)onNote, velocity));
            drawKeys();
        }

        private void NoteOff()
        {
            onNote = -1;
            if (EventNoteOff != null)
                EventNoteOff(this, new KeyNoteEventArgs((byte)onNote));
            drawKeys();
        }

        private void KeybordControl_MouseMove(object sender, MouseEventArgs e)
        {
            if ((e.Button & System.Windows.Forms.MouseButtons.Left) == 0)
                return;
            NoteOn(HitNote(e.X, e.Y),127);
        }

        private void KeybordControl_MouseDown(object sender, MouseEventArgs e)
        {
            if ((e.Button & System.Windows.Forms.MouseButtons.Left) == 0)
                return;
            this.Capture = true;
            NoteOn(HitNote(e.X, e.Y), 127);
        }

        private void KeybordControl_MouseUp(object sender, MouseEventArgs e)
        {
            if ((e.Button & System.Windows.Forms.MouseButtons.Left) == 0)
                return;
            this.Capture = false;
            if (onNote == -1)
                return;
            NoteOff();
        }

        private void ChangeGenType(byte type)
        {
            if (EventChangeType == null)
                return;

            ToolStripMenuItem[] items = { cmdChangeType0, cmdChangeType1, cmdChangeType2, cmdChangeType3, cmdChangeType4, cmdChangeType5 };
            if (type >= items.Length)
                return;

            if (items[type].Checked)
                return;

            for (int i = 0; i < items.Length; i++)
            {
                items[i].Checked = (type == i);
            }

            EventChangeType(this, type);
        }

        private void cmdChangeType0_Click(object sender, EventArgs e)
        {
            ChangeGenType(0);
        }

        private void cmdChangeType1_Click(object sender, EventArgs e)
        {
            ChangeGenType(1);
        }

        private void cmdChangeType2_Click(object sender, EventArgs e)
        {
            ChangeGenType(2);
        }

        private void cmdChangeType3_Click(object sender, EventArgs e)
        {
            ChangeGenType(3);
        }

        private void cmdChangeType4_Click(object sender, EventArgs e)
        {
            ChangeGenType(4);
        }

        private void cmdChangeType5_Click(object sender, EventArgs e)
        {
            ChangeGenType(5);
        }

        private void contextMenu_Opening(object sender, CancelEventArgs e)
        {
            if (EventChangeType == null)
                e.Cancel = true;
        }

        private void cmdSelectMidiInDev_Click(object sender, EventArgs e)
        {
            var dlg = new frmMidiDevice();
            MidiInDetach();
            dlg.SelectedIndex = midiInDev + 1;
            dlg.VelocityConst = midiVelocityConst;
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                midiInDev = dlg.SelectedIndex - 1;
                midiVelocityConst = dlg.VelocityConst;
            }
            if (MidiInAttach() == false)
            {
                MessageBox.Show("Failed to open MIDI device", this.Parent.Text);
                midiInDev = -1;
            }

        }
    }
}
