using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SlimDX.XInput;

namespace RobotArmController_CUI_ver1
{
    /// <summary>
    /// ロボットアームをゲームパットで操作するためのクラス
    /// </summary>
    class RobotArmController
    {
        //シリアル通信　設定
        const string _PortName = "COM12";
        const int _BaudRate = 115200;
        const Parity _Parity = Parity.None;
        const int _DataBits = 8;
        const StopBits _StopBit = StopBits.One;

        private SerialPort SerialPort { get; set; }
        public SerialDataReceivedEventHandler DataReceived
        {
            set
            {
                SerialPort.DataReceived += value;
            }
        }

        //ロボットアーム設定
        const int MovableRange = 100;
        const float AdjustingSpeed = 50; // (mm/s)
        const int CenterX = 0;
        const int CenterY = 150;
        const int CenterZ = 0;
        const float AdjustingAngularSpeed = 30;  //(deg/s)
        const int InitAngle = 40;
        const int AngularRange = 50;    //(deg)

        private System.Diagnostics.Stopwatch Stopwatch { get; set; }

        private int CX { get; set; }
        private int CY { get; set; }
        private int CZ { get; set; }
        private int Angle { get; set; }

        //ゲームパット用設定
        private Controller Controller { get; set; }

        public bool IsConnected { get; set; } = false;

        public RobotArmController()
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

            //インターバル計測用ストップウォッチのインスタンス生成
            Stopwatch = new System.Diagnostics.Stopwatch();

            //初期中心座標保持
            CX = CenterX;
            CY = CenterY;
            CZ = CenterZ;

            //手の開閉度合い設定
            Angle = InitAngle;
        }

        /// <summary>
        /// ロボットアームとの接続開始
        /// </summary>
        public void Start()
        {
            //シリアル通信接続
            while (!SerialPort.IsOpen)
            {
                SerialPort.Open();

            }
            if (SerialPort.IsOpen && Controller.IsConnected)
            {
                IsConnected = true;
                //計測開始
                Stopwatch.Start();
            }

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
                //計測中止
                Stopwatch.Stop();

                //XYZ入力
                x = CX + (MovableRange * gamePad.LeftThumbX) / short.MaxValue;
                y = CY + (MovableRange * gamePad.LeftThumbY) / short.MaxValue;
                z = CZ + (MovableRange * gamePad.RightThumbY) / short.MaxValue;

                SerialPort.WriteLine(string.Format("G90 X{0} Y{1} Z{2} S", x, y, z));

                //中心位置の調整XYZ
                if (gamePad.Buttons.HasFlag(GamepadButtonFlags.DPadLeft)) CX -= (int)(AdjustingSpeed * Stopwatch.ElapsedMilliseconds) / 1000;
                if (gamePad.Buttons.HasFlag(GamepadButtonFlags.DPadRight)) CX += (int)(AdjustingSpeed * Stopwatch.ElapsedMilliseconds) / 1000;
                if (gamePad.Buttons.HasFlag(GamepadButtonFlags.DPadDown)) CY -= (int)(AdjustingSpeed * Stopwatch.ElapsedMilliseconds) / 1000;
                if (gamePad.Buttons.HasFlag(GamepadButtonFlags.DPadUp)) CY += (int)(AdjustingSpeed * Stopwatch.ElapsedMilliseconds) / 1000;
                if (gamePad.Buttons.HasFlag(GamepadButtonFlags.A)) CZ -= (int)(AdjustingSpeed * Stopwatch.ElapsedMilliseconds) / 1000;
                if (gamePad.Buttons.HasFlag(GamepadButtonFlags.Y)) CZ += (int)(AdjustingSpeed * Stopwatch.ElapsedMilliseconds) / 1000;

                //手の開閉調整
                if (gamePad.Buttons.HasFlag(GamepadButtonFlags.LeftShoulder)) Angle -= (int)(AdjustingAngularSpeed * Stopwatch.ElapsedMilliseconds) / 1000;
                if (gamePad.Buttons.HasFlag(GamepadButtonFlags.RightShoulder)) Angle += (int)(AdjustingAngularSpeed * Stopwatch.ElapsedMilliseconds) / 1000;

                //計測再開
                Stopwatch.Reset();
                Stopwatch.Start();

                //中心位置をリセットするかXY,Z
                if (gamePad.Buttons.HasFlag(GamepadButtonFlags.LeftThumb))
                {
                    CX = CenterX;
                    CY = CenterY;
                }
                if (gamePad.Buttons.HasFlag(GamepadButtonFlags.RightThumb)) CZ = CenterZ;

                //手の操作
                angle = Angle + AngularRange * (gamePad.RightTrigger - gamePad.LeftTrigger) / byte.MaxValue;
                if (angle < 0)
                    angle = 0;
                SerialPort.WriteLine(string.Format("G92 S4 A{0}", angle));

            }
        }

    }
}
