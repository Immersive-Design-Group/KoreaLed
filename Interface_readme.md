Parameters:
1.X_step: Range 1-10 (integer only). This value represents the number of steps the motor controlling the camera will rotate with each command, which can be used to control the speed.
2.X_span: Range 0.5-10 (can be a float or an integer). This value represents the time interval between each movement of the motor controlling the camera, which can be used to control the speed.
3.X_time: Range 1-100 (integer only). This value represents the total number of steps the motor controlling the camera will move, which can be used to control the range of rotation.
Important: When setting the values related to X, ensure that the value of X_time is divisible by the value of X_step. (for example, 100/10, 50/5, 23/1, 44/2)
4.Y_step: Range 1-60 (integer only). This value represents the total number of steps the motor controlling the entire structure will move, which can be used to control the range of rotation.
5.Port name: Specify the port name associated with the PCB.
6.Baud rate: Fixed at 115200.

Buttons:
1.Update: Uploads the current parameter values for execution.
2.Sleep: Resets all motor positions.
3.Scan: Executes the command sequence.