Automatic card dealer

Features:

- Central Pololu gear motor with quadrature encoder for motion control about the central axis. Can rotate the entire robot to deal cards to different players
    - Can be controller either with PID controller with a quintic path planner (to go to a commanded position), or can be manually controlled with the knob on the control box
    - PID is accurate to <2 degrees.
    - Encoder signals are processed manually using the PCNT driver library with 4x decoding
    - Has limit switches to automatically home and find limits
- VCNT2030 proximity sensor to detect when a card has been dealt
- Drone BLDC with a mounted O-ring on a circle to grip and shoot the card once it has been fed
- Feeder motor (JGY-070 worm gearmotor) is attached to a rod with printer rollers, which slowly feed a card out to the launcher BLDC motor
- Card holder height can be adjusted by 4 screws until the feeder motor works perfectly
- Has emergency stop button
- Control box has 3 buttons (mechanical keyboard switches), a 320x200 TFT display controlled over SPI, and a rotary encoder. The rotary encoder uses an AS5600 under the hood.
- All controlled by a single ESP32-S3

Code:

Code for microcontroller is in code-esp32. See the readme.

plot-pid folder has code to plot values emitted from the esp32 over time. this works by reading the serial port. an arbitrary number of graphs can be plotted with an arbitrary number of lines on any graph.

Pictures/videos:

Demo videos:

<include media/demo/deal-demo.MOV. say it demos PID going to multiple positions, shooting a card, then going to the next position in series to simulate dealing cards to a player. the VCNT2030 proximity sensor allows for closed loop control of the card dealer since we know when a card was actually dealt. mention this uses an earlier version of the robot.>
<include media/demo/teleoperation.mov. say it shows controlling the central axis motor using the knob.>
<include media/demo/homing-sequence.mov. say it shows the homing sequence>
<include media/demo/pid-tuning.mov. say it shows a menu where you can tune the PID parameters (K_d, K_d, K_i, K_ff) manally.>

CAD:

<include media/CAD/control-box.png. say it shows the control box.>
<include control-knob.png. say it labels the rotary encoder knob+magnet+bearing around the knob+as5600.>
<include launcher.png. say it shows the launcher apparatus assembled with all the motors and card deck holder. electronics (motor drivers, etc) are under the top panel.>

Other pictures:

<include media/demo/overview.jpg. say this is the most recent version of the robot>
<include media/demo/limit-switch.jpg. say this shows the lazy susan bearing which the entire apparatus rotates on and the limit witches for homing (N.O.)>
<include media/demo/vcnt-bldc.jpg. say this shows the BLDC launch motor and the vCNT2030 proximity sensor that is used to detect a card>
<include media/demo/gear-motor-card-holder.jpg. say this shows the wormgear feeder motor and the card holder>
<include media/demo/roller-adjustment-screws.jpg. say this shows the feeder rollers taken from a printer and the screws to adjust the height of the card holder>
<include media/demo/electronics.jpg. say this shows the electronics to control the motors and supply power to everything.>
<include media/demo/coupling.jpg. say this shows the coupling that attaches the pololu motor shaft to the base of the robot.>

Future work:

<write about using brushless gimbal motor + simplefocmini + as5600 for better PID control>
<include media/demo/belt-drive-test.mov. show it shows gt2 belt drive with bldc motor and simplefocmini to demo how the new control system will work with better control.>

<write about using 2 esp32's, one on the control box and one on the robot, and they will wirelessly communicate using ESPnow to avoid running cables between the two.>