using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO.Ports;

namespace RobotArmController_CUI_ver1
{
    class Program
    {
        static void Main(string[] args)
        {
            RobotArmController robotArmController = new RobotArmController();
            robotArmController.Start();
            if (robotArmController.IsConnected)
            {
                robotArmController.DataReceived = (sender,e) => 
                {
                    var serial = sender as SerialPort;
                    //Console.Write(serial.ReadExisting());
                };
                while (true)
                {
                    robotArmController.Process();

                    Thread.Sleep(100);
                }
            }
        }
    }
}
