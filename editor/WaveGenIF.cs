using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
namespace WaveGenEditor
{
    class WaveGenIF
    {
        private IntPtr hHandle;

        public WaveGenIF()
        {
            cbInstancePlayFinished = cbPlayFinished;
        }
        public bool CreateWaveGen(IntPtr hWnd)
        {
            try
            {
                hHandle = DllCreateWaveGen(hWnd);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "WaveGenEditor", MessageBoxButtons.OK, MessageBoxIcon.Question);
                return true;
            }
            return hHandle == null;
        }
        public void Release()
        {
            if (hHandle == IntPtr.Zero)
                return;
            DllReleaseWaveGen(hHandle);
            hHandle = IntPtr.Zero;
        }
        public bool CompileMML(string mmlString, ref int errorCode, ref UInt32 errorLine, bool isWave, delegatePlayFinished dlgPlayFinished, object parent)
        {
            if (hHandle == IntPtr.Zero)
                return true;
            lock (this)
            {
                CallbackParams info = new CallbackParams(this, parent, dlgPlayFinished);
                int hash = this.GetHashCode();
                callbacks[hash] = info;
                return DllCompileMML(hHandle, mmlString, ref errorCode, ref errorLine, (byte)(isWave ? 1 : 0), cbInstancePlayFinished, hash);
            }
        }
        public void SetWaveFileName(string fileName)
        {
            if (hHandle == IntPtr.Zero)
                return;

            DllSetWaveFileName(hHandle, fileName);
        }
        public static string GetErrorString(int errorCode)
        {
            try
            {
                StringBuilder errorMessage = new StringBuilder(512);
                DllGetErrorString(errorCode, errorMessage, errorMessage.Capacity);
                return errorMessage.ToString();
            }
            catch (Exception e)
            {
                return e.Message;
            }
        }

        public void Stop()
        {
            if (hHandle == IntPtr.Zero)
                return;
            DllStop(hHandle);
        }

        public void PreviewNoteOn(byte note,byte velocity)
        {
            if (hHandle == IntPtr.Zero)
                return;
            DllPreviewNoteOn(hHandle, note, velocity);
        }

        public void PreviewNoteOff()
        {
            if (hHandle == IntPtr.Zero)
                return;
            DllPreviewNoteOff(hHandle);
        }

        public void PreviewGenType(byte type)
        {
            if (hHandle == IntPtr.Zero)
                return;
            DllPreviewGenType(hHandle, type);
        }

        public void PreviewADSR(float aPower, float aTime, float dTime, float sPower, float rTime)
        {
            if (hHandle == IntPtr.Zero)
                return;
            DllPreviewSetADSR(hHandle, aPower, aTime, dTime, sPower, rTime);
        }

        public delegate void delegatePlayFinished(WaveGenIF wg, object pParam);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void callbackPlayFinished(IntPtr pParam);
        private readonly callbackPlayFinished cbInstancePlayFinished;
        public class CallbackParams
        {
            public CallbackParams(WaveGenIF _this, object parent, delegatePlayFinished dlg)
            {
                m_this = _this;
                m_parent = parent;
                m_dlg = dlg;
            }
            public void Exec()
            {
                m_dlg(m_this, m_parent);
            }
            WaveGenIF m_this;
            object m_parent;
            delegatePlayFinished m_dlg;
        }
        private void cbPlayFinished(IntPtr pParam)
        {
            lock (this)
            {
                CallbackParams param = callbacks[pParam.ToInt32()];
                if (param == null)
                    return;
                param.Exec();
            }
        }

        private static Dictionary<int, CallbackParams> callbacks = new Dictionary<int, CallbackParams>();

#if DEBUG
        const string DLLNAME = "../../../WaveGenDll/Debug/WaveGenDll.dll";
#else
        const string DLLNAME = "WaveGenDll.dll";
#endif

        [DllImport(DLLNAME, EntryPoint = "CreateWaveGen")]
        private extern static IntPtr DllCreateWaveGen(IntPtr hWnd);
        [DllImport(DLLNAME, EntryPoint = "ReleaseWaveGen")]
        private extern static void DllReleaseWaveGen(IntPtr hInterface);
        [DllImport(DLLNAME, EntryPoint = "CompileMML", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.I1)]
        private extern static bool DllCompileMML(IntPtr hInterface, String mmlString, ref Int32 pErrorCode, ref UInt32 pErrorLine, byte isWave, callbackPlayFinished dlgPlayFinished, Int32 param);
        [DllImport(DLLNAME, EntryPoint = "GetErrorString", CharSet = CharSet.Unicode)]
        private extern static void DllGetErrorString(Int32 pErrorCode, StringBuilder errorMessage, Int32 nMaxCount);
        [DllImport(DLLNAME, EntryPoint = "Stop")]
        private extern static void DllStop(IntPtr hInterface);
        [DllImport(DLLNAME, EntryPoint = "SetWaveFileName", CharSet = CharSet.Unicode)]
        private extern static void DllSetWaveFileName(IntPtr hInterface, String fileName);
        [DllImport(DLLNAME, EntryPoint = "PreviewNoteOn")]
        private extern static void DllPreviewNoteOn(IntPtr hInterface, byte note,byte velocity);
        [DllImport(DLLNAME, EntryPoint = "PreviewNoteOff")]
        private extern static void DllPreviewNoteOff(IntPtr hInterface);
        [DllImport(DLLNAME, EntryPoint = "PreviewGenType")]
        private extern static void DllPreviewGenType(IntPtr hInterface, byte type);
        [DllImport(DLLNAME, EntryPoint = "PreviewSetADSR")]
        private extern static void DllPreviewSetADSR(IntPtr hInterface, float aPower, float aTime, float dTime, float sPower, float rTime);
    }
}
