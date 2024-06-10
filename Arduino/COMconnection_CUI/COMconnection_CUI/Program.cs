using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO.Ports;
using System.Threading;
using SlimDX.XInput;

namespace COMconnection_CUI
{
    class Program
    {
        //シリアル通信　設定
        const string _PortName = "COM11";
        const int _BaudRate = 19200;
        const Parity _Parity = Parity.None;
        const int _DataBits = 8;
        const StopBits _StopBit = StopBits.One;

        static void Main(string[] args)
        {
            SerialPort sp = new SerialPort
            {
                PortName = _PortName,
                BaudRate = _BaudRate,
                Parity = _Parity,
                DataBits = _DataBits,
                StopBits = _StopBit
            };

            //データ受信時に呼ばれるイベントを追加
            sp.DataReceived += DataReceivedHandler;


            //ゲームパット取得
            Controller con = new Controller(UserIndex.One);

            if (con.IsConnected)
            {
                Gamepad gamepad;

                while (true)
                {
                    gamepad = con.GetState().Gamepad;
                    /*
                    if (state.Gamepad.Buttons.HasFlag(GamepadButtonFlags.A))
                    {
                        sp.WriteLine("G92 S4 A90");
                    }
                    else
                    {
                        sp.WriteLine("G92 S4 A30");
                    }
                    */
                    Console.WriteLine("LJ:{0},RJ:{1}", gamepad.LeftThumbX, gamepad.RightThumbX);
                    Console.WriteLine("LB:{0},RB:{1}",
                        gamepad.Buttons.HasFlag(GamepadButtonFlags.LeftShoulder),
                        gamepad.Buttons.HasFlag(GamepadButtonFlags.RightShoulder));
                    Console.WriteLine("LT:{0},RT:{1}", gamepad.LeftTrigger, gamepad.RightTrigger);
                    Console.WriteLine("DL:{0},DU:{1},DR:{2},DD:{3}",
                        gamepad.Buttons.HasFlag(GamepadButtonFlags.DPadLeft),
                        gamepad.Buttons.HasFlag(GamepadButtonFlags.DPadUp),
                        gamepad.Buttons.HasFlag(GamepadButtonFlags.DPadRight),
                        gamepad.Buttons.HasFlag(GamepadButtonFlags.DPadDown));



                    Thread.Sleep(2000);
                }
            }
            else
            {
                Console.WriteLine("not connected");
                while (true)
                {
                    //標準入力から文字列を取得してそれをそのままシリアルポートに出力
                    sp.WriteLine(Console.ReadLine());
                }
            }
        }

        /// <summary>
        /// シリアル通信上で受信したデータを標準出力に出力
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private static void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            var sp = (SerialPort)sender;
            string data = sp.ReadExisting();
            Console.Write(data);
        }
    }
}
