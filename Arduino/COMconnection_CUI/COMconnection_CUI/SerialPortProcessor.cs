using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Threading;

using System.IO.Ports;

namespace COMconnection_CUI
{
    class SerialPortProcessor
    {
        private SerialPort myPort = null;

        public String PortName { get; set; }
        public int BaudRate { get; set; }
        public Parity Parity { get; set; }
        public int DataBits { get; set; }
        public StopBits StopBits { get; set; }

        public SerialPortProcessor()
        {

        }

        public void Start()
        {
            myPort = new SerialPort(PortName, BaudRate, Parity, DataBits, StopBits);
            myPort.Open();
        }

        public void WriteData(byte[] buffer)
        {
            myPort.Write(buffer, 0, buffer.Length);
        }


        public byte[] ReceiveData()
        {
            if (myPort == null)
            {
                return null;
            }

            try
            {
                int rbyte = myPort.BytesToRead;
                byte[] buffer = new byte[rbyte];
                int read = 0;

                while (read < rbyte)
                {
                    int length = myPort.Read(buffer, read, rbyte - read);
                    read += length;
                }
                return buffer;
            }
            catch
            {
            }
            return null;
        }

        public void Close()
        {
            if (myPort != null)
            {
                myPort.Close();
            }
        }
    }
}
