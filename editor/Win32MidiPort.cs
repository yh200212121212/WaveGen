using System;
using System.Runtime.InteropServices;


namespace jp.ropo
{
    public class Win32MidiInPort : IDisposable
    {
        private NativeMethods.MidiInProc midiInProc;
        private IntPtr handle = IntPtr.Zero;

        public Win32MidiInPort()
        {
            midiInProc = new NativeMethods.MidiInProc(MidiProc);
            handle = IntPtr.Zero;
        }

        public void Dispose()
        {
            Close();
        }

        public static int InputCount
        {
            get { return NativeMethods.midiInGetNumDevs(); }
        }

        public bool Close()
        {
            if (handle == IntPtr.Zero)
                return true;
            bool result = NativeMethods.midiInClose(handle) == NativeMethods.MMSYSERR_NOERROR;
            handle = IntPtr.Zero;
            return result;
        }

        public bool Open(int id)
        {
            return NativeMethods.midiInOpen(out handle, id, midiInProc, IntPtr.Zero, NativeMethods.CALLBACK_FUNCTION) == NativeMethods.MMSYSERR_NOERROR;
        }

        public bool Start()
        {
            return NativeMethods.midiInStart(handle) == NativeMethods.MMSYSERR_NOERROR;
        }

        public bool Stop()
        {
            return NativeMethods.midiInStop(handle) == NativeMethods.MMSYSERR_NOERROR;
        }

        private void MidiProc(IntPtr hMidiIn, int wMsg, IntPtr dwInstance, uint dwParam1, uint dwParam2)
        {
            switch (wMsg)
            {
                case NativeMethods.MM_MIM_OPEN:
                    break;
                case NativeMethods.MM_MIM_CLOSE:
                    break;
                case NativeMethods.MM_MIM_DATA:
                    OnMimData(dwParam1, dwParam2);
                    break;
                case NativeMethods.MM_MIM_LONGDATA:
                    break;
                case NativeMethods.MM_MIM_LONGERROR:
                    break;
                case NativeMethods.MM_MIM_ERROR:
                    break;
                case NativeMethods.MM_MIM_MOREDATA:
                    break;
            }
        }

        public delegate void dlgOnNoteChange(byte ch, byte note, byte velocity, bool isOn);
        public event dlgOnNoteChange OnNoteChange = null;

        private void OnMimData(uint dwData, uint dwTimeStamp)
        {
            if (OnNoteChange == null)
                return;
            byte st, dt1, dt2;

            st = (byte)((dwData) & 0xFF);	// status
            dt1 = (byte)((dwData >> 8) & 0xFF);	// data1
            dt2 = (byte)((dwData >> 16) & 0xFF);	// data2

            if (st < 0xF0)
            {
                byte ch = (byte)(st & 0xF);
                switch (st >> 4)
                {
                    case 0x8:// note off
                        OnNoteChange(ch, dt1, 0, false);
                        break;
                    case 0x9:// note on
                        OnNoteChange(ch, dt1, dt2, true);
                        break;
                    case 0xa:// polyphonic
                        break;
                    case 0xb:// control change
                        break;
                    case 0xc:// program change
                        break;
                    case 0xd:// channel pressure
                        break;
                    case 0xe:// pitch bend change
                        break;
                }
            }
        }

        public class DeviceCaps
        {
            public string deviceName;
        }
        static public DeviceCaps GetDeviceInfo(int id)
        {
            var devCaps = new NativeMethods.MIDIINCAPS();
            if (NativeMethods.midiInGetDevCaps((uint)id, ref devCaps, (uint)Marshal.SizeOf(typeof(NativeMethods.MIDIINCAPS))) != NativeMethods.MMSYSERR_NOERROR)
                return null;
            var caps = new DeviceCaps();
            caps.deviceName = devCaps.szPname;
            return caps;
        }
    }

    internal static class NativeMethods
    {
        internal const int MMSYSERR_NOERROR = 0;

        internal const int CALLBACK_FUNCTION = 0x30000;
        internal const int MM_MIM_OPEN = 0x3C1;
        internal const int MM_MIM_CLOSE = 0x3C2;
        internal const int MM_MIM_DATA = 0x3C3;
        internal const int MM_MIM_LONGDATA = 0x3C4;
        internal const int MM_MIM_ERROR = 0x3C5;
        internal const int MM_MIM_LONGERROR = 0x3C6;
        internal const int MM_MIM_MOREDATA = 0x3CC;

        internal delegate void MidiInProc(IntPtr hMidiIn, int wMsg, IntPtr dwInstance, uint dwParam1, uint dwParam2);

        [DllImport("winmm.dll")]
        internal static extern int midiInGetNumDevs();
        [DllImport("winmm.dll")]
        internal static extern int midiInClose(IntPtr hMidiIn);
        [DllImport("winmm.dll")]
        internal static extern int midiInOpen(out IntPtr lphMidiIn, int uDeviceID, MidiInProc dwCallback, IntPtr dwCallbackInstance, int dwFlags);
        [DllImport("winmm.dll")]
        internal static extern int midiInStart(IntPtr hMidiIn);
        [DllImport("winmm.dll")]
        internal static extern int midiInStop(IntPtr hMidiIn);
        [StructLayout(LayoutKind.Sequential)]
        internal struct MIDIINCAPS
        {
            public ushort wMid;
            public ushort wPid;
            public uint vDriverVersion;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public string szPname;
            public uint dwSupport;
        }
        [DllImport("winmm.dll")]
        internal static extern uint midiInGetDevCaps(uint uDeviceID, ref MIDIINCAPS lpCaps, uint uSize);
    }
}
