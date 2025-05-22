COMPONENTS REQUIRED
MPU6050
16x2 LCD (with LiquidCrystal library)
 Active Buzzer
Two LEDs (Bank Angle & MCAS alerts)
Jumper Wires

Component	Arduino Uno Pin	Description
LCD RS	12	LCD Register Select
LCD E	11	LCD Enable
LCD D4	5	LCD Data Line 4
LCD D5	4	LCD Data Line 5
LCD D6	3	LCD Data Line 6
LCD D7	2	LCD Data Line 7
LCD VSS	GND	Ground
LCD VDD	5V	Power Supply
LCD VO	Potentiometer Mid	Contrast control (middle of 10k pot)
LCD RW	GND	Set to Write Mode
LCD A (LED+)	5V via 220Ω	LCD Backlight Anode (+)
LCD K (LED−)	GND	LCD Backlight Cathode (−)
________________________________________
MPU6050 I2C Connections
MPU6050 Pin	Arduino Uno Pin	Description
VCC	5V	Power supply
GND	GND	Ground
SDA	A4	I2C Data
SCL	A5	I2C Clock
________________________________________
Buzzer and LEDs
Component	Arduino Uno Pin	Notes
Buzzer +	9	Tone output
Buzzer −	GND	
Bank LED	6	Indicates bank angle alert
MCAS LED	7	Indicates nose angle alert
LED GND (both)	GND	Use current-limiting resistors (~220Ω)
________________________________________
 LCD Contrast Control (Optional, but Recommended)
•	Connect a 10k potentiometer:
o	One side to GND
o	One side to 5V
o	Middle pin (wiper) to VO on LCD

