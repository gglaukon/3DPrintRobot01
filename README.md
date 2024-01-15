# 3DPrintRobot01
Initial attempt at a self navigating robot with a 3d printed frame



01 - Overview and Materials
02 - Frame printing notes
03 - Frame Assembly
	A) Drive Plate Assembly
	B) Stearing Plate Assembly
	C) Frame Assembly
04 - Circuit Assembly

=============================
01 - Overview and Materials
=============================
  This is a design for a simple automated device that is able to path along the ground, avoiding obsticals. The frame is 3D printed, and requires a printing bed of 200mm square minimum. Additionally, the model is composed of...
    - 1 28BYJ-48 stepper motor
    - 1 ULN2003 Stepper motor driver module
    - 1 SG90 servo motor
    - 3 HC-SR04 ultrasonic range finder modules
    - 1 2x16 LCD display
    - 1 LCD1602 I2C module
    - 1 Arduino nano development board
    - 1 30 row breadboard
    - 1 9V battery
    - Glue dots
    - Electrical tape

=============================
02 - Frame printing notes
=============================
	The frame is largely composed of the two motor plates (DrivePlate03 and StearingPlate), and the BodyFrame object. When printing the body frame, print one as-is, and a second one, mirrored lengthwise. Additionally, several BodyClipDriveSide and BodyClipStearingSide objects can be printed and used to stabilize the frame. The ChasisPlatformDriveSide should be printed twice, and then supported further with a few BodyClipDriveSide objects.
	When assembling, parts can be fastened with glue dots, tape, or friction welding.

=============================
03 - Frame Assembly
=============================
	A) Drive Plate Assembly
		PRINT LIST:
		1 x DrivePlate03
		1 x MainDriveGear
		1 x DriveAxelGear
		1 x StepperMount
		2 x DrivePlateAxelMount
		2 x Wheels
		The drive plate is composed of the DrivePlate03, MainDriveGear, DriveAxelGear, DrivePlateAxelMount, StepperMount, and DriveAxel. The stepper motor can be fastened to the StepperMount with adhesive or 3mm screws. Once the motor is fastened, the mount can be slipped between the two prongs on the DrivePlate, and then the MainDriveGear can be attached to the motor spoke. Print two DrivePlateAxelMounts, and attach them to the underside of the platform. Thread the DriveAxel through one of the mounts, fit the DriveAxelGear to interlock with the MainDriveGear through the slot in the middle of the plate, and thread the DriveAxel through the DriveAxelGear and out through the other DrivePlateAxelMount. Print two wheels and fit one to either end of the DriveAxel

	B) Stearing Plate Assembly
		PRINT LIST:
		1 x StearingPlate
		1 x StearingMotorGear
		1 x StearingGear
		1 x StearingColar
		1 x StearingAxelMount
		2 x StearingAxel
		2 x Wheels
		In addition to the printed components, the cross shaped motor fitting should be used in assembly, but not fitted to the motor yet. Adhere the cross fitting to the StearingMotorGear, fit the motor into the rectangular slot on the StearingPlate (so that the motor spoke is closer to the circular hole on the plate), and then screw the StearingAxelGear to the motor. put a StearingColar on the round protrusion of the StearingGear, and then feed the rest of the protrusion through the hole on the StearingPlate. To keep the stearing gear from falling out when the device is picked up, you may want to adhere a piece of raft to the top of the protrusion. feed one axel into the cross shaped hole on the StearingGear, and fit the StearingAxelMount on the other end of the axel. Feed the other StearingAxel through the horizontal opening of the StearingAxelMount, and fit a wheel on either side of the axel

	C) Frame Assembly
		2 x BodyFrame (one nominal, one mirrored)
		1 x SensorHolder
		2 x ChasisPlatformDriveSide
		4+ x BodyClipDriveSide
		4+ x BodyClipStearingSide
		With both motor platforms assembled, take the two BodyFrame and fit them over the plates, so the drive plate is in the rear, and the body frame tapers towards the front, where the stearing plate sits. Use a few of the body clips on the front and back of the frame, and along the top, to stabilize the frame. Take the two ChasisPlatformDriveSide objects, and attach them to the frame, at the furthest extremities of the drive side of the frame (one at the furthest edge of the rear, and one as close to the taper as possible), and use a few BodyClipDriveSide objects to support it. Adhere a BodyClipStearingSide to the front of the SensorHolder, and then tape a few BodyClipDriveSide together, so that it makes solid contact with both far ends of the Sensor holder, giving support to the rear of the sensor holder when it is fitted to the front of the frame. With the sensor holder assmebled, fit the sensors into the slots of the holder

=============================
04 - Circuit Assembly
=============================
	On the breadboard, attach the positive terminal of the 9V battery to one of the power rails on the breadboard, and the negative terminal to one of the ground rails. Plug the arduino into the center of the breadboard, so the USB port is at the edge (for ease of reference, it will be assumed that the controller is plugged in to the row 30 side of the breadboard). Connect the 5V pin of the arduino to the other power rail on the breadboard, and connect the ground pin on the arduino to one of the ground rails on the breadboard, and install a jumper wire accross the two ground rails. Connect the 9V rail to the Vin pin of the arduino. 
	Connect the data pins for the right, center, and left sensors to the A0, A1, and A2 pins respectively and the power pins to the 5V rail (ground can be tied to either ground rail). Connect the signal wire for the servo to pin 9, and the power pin to the 9V rail. Connect the motor driver pins 1, 2, 3, and 4 to arduino pins 5, 4, 3, and 2 respectively, and the power pin to the 9V rail. For the LDC screen, connect the SDA pint to A1, and the SDC to A0, and the power pin to the 5V rail

