using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using SlimDX.XInput;
using SlimDX.DirectInput;

namespace GamePadTest
{
    class Program
    {
        static void Main(string[] args)
        {
            /*
            DirectInput directInput = new DirectInput();
            var list = directInput.GetDevices(DeviceClass.GameController,DeviceEnumerationFlags.AttachedOnly);

            if (list.Any())
            {
                Console.WriteLine("Any");
                Joystick gamepad = new Joystick(directInput,list[0].InstanceGuid);
                gamepad.Acquire();
                while (true)
                {
                    var state = gamepad.GetCurrentState();

                    var buttons = state.GetButtons();

                    foreach (var b in buttons)
                    {
                        Console.Write("{0} ", b);
                    }
                    Console.WriteLine();
                    Thread.Sleep(300);
                }
            }
            */

            Controller con = new Controller(UserIndex.One);
            if (con.IsConnected)
            {
                var state = con.GetState();

                while (true)
                {
                    state = con.GetState();
                    Console.WriteLine("A:{0}", state.Gamepad.Buttons.HasFlag(GamepadButtonFlags.A));

                    Thread.Sleep(1000);
                }
            }
            else
            {
                Console.WriteLine("not connected");
            }

        }
    }
}
