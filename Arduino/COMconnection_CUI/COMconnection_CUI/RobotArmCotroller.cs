using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SlimDX.XInput;

namespace COMconnection_CUI
{
    /// <summary>
    /// ロボットアームをゲームパットで操作するためのクラス
    /// </summary>
    class RobotArmCotroller
    {
        //シリアル通信　設定
        const string _PortName = "COM11";
        const int _BaudRate = 19200;
        const Parity _Parity = Parity.None;
        const int _DataBits = 8;
        const StopBits _StopBit = StopBits.One;

        //ロボットアーム設定
        const int MovableRange = 100;
        const int CenterX = 0;
        const int CenterY = 100;
        const int CenterZ = 0;

        private int CX { get; set; }
        private int CY { get; set; }
        private int CZ { get; set; }

        private SerialPort SerialPort { get; set; }
        public SerialDataReceivedEventHandler DataReceived
        {
            set
            {
                SerialPort.DataReceived += value;
            }
        }

        private Controller Controller { get; set; }

        public bool IsConnected { get; set; } = false;

        public RobotArmCotroller()
        {
            //シリアル通信クラスのインスタンス生成
            SerialPort = new SerialPort
            {
                PortName = _PortName,
                BaudRate = _BaudRate,
                Parity = _Parity,
                DataBits = _DataBits,
                StopBits = _StopBit
            };
            //ゲームパットのインスタンス生成
            Controller = new Controller(UserIndex.One);

            //初期中心座標保持
            CX = CenterX;
            CY = CenterY;
            CZ = CenterZ;
        }

        /// <summary>
        /// ロボットアームとの接続開始
        /// </summary>
        public void Start()
        {
            //シリアル通信接続
            SerialPort.Open();
            if (SerialPort.IsOpen && Controller.IsConnected)
                IsConnected = true;
        }

        /// <summary>
        /// ロボットアームとの接続終了
        /// </summary>
        public void End()
        {
            //シリアル通信切断
            SerialPort.Close();

            IsConnected = false;
        }

        /// <summary>
        /// ロボットアーム操作を処理
        /// </summary>
        public void Process()
        {
            if (IsConnected)
            {
                int x, y, z, angle;
                var gamePad = Controller.GetState().Gamepad;

                //XYZ入力
                x = CX + (MovableRange*gamePad.LeftThumbX)/short.MaxValue;
                y = CY + (MovableRange*gamePad.LeftThumbY)/short.MaxValue;
                z = CZ + (MovableRange*gamePad.RightThumbY)/short.MaxValue;

                SerialPort.WriteLine(string.Format("G90 X{0} Y{1} Z{2}",x,y,z));
            }
        }

    }
}
