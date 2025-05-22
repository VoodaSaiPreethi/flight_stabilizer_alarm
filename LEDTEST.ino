#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // RS, E, D4, D5, D6, D7

const int buzzerPin = 9;
const int bankLed = 6;
const int mcasLed = 7;

unsigned long lastLCDUpdate = 0;
unsigned long lastSerialPrint = 0;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.print("Stabilizer Init");
  
  // Initialize MPU6050
  if (!mpu.begin()) {
    lcd.setCursor(0, 1);
    lcd.print("MPU6050 Failed");
    Serial.println("MPU6050 not found!");
    while (1);
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  // Pin Setup
  pinMode(buzzerPin, OUTPUT);
  pinMode(bankLed, OUTPUT);
  pinMode(mcasLed, OUTPUT);
  
  delay(2000);
  lcd.clear();
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  // Acceleration (in m/s^2), convert to g
  float axg = a.acceleration.x / 9.81;
  float ayg = a.acceleration.y / 9.81;
  float azg = a.acceleration.z / 9.81;
  
  // Calculate roll (bank angle) and pitch (nose angle)
  float bankAngle = atan2(ayg, azg) * 180 / PI;
  float noseAngle = atan2(-axg, sqrt(ayg * ayg + azg * azg)) * 180 / PI;
  
  bool bankAngleAlert = abs(bankAngle) > 35;  // Bank angle threshold: 35 degrees
  bool mcasActive = abs(noseAngle) > 15;      // Nose angle threshold: 15 degrees
  
  // LED + Buzzer Control
  if (bankAngleAlert && mcasActive) {
    digitalWrite(bankLed, HIGH);
    digitalWrite(mcasLed, HIGH);
    tone(buzzerPin, 1000); // Combined tone
  } else if (bankAngleAlert) {
    digitalWrite(bankLed, HIGH);
    digitalWrite(mcasLed, LOW);
    tone(buzzerPin, 1500); // Bank alert
  } else if (mcasActive) {
    digitalWrite(bankLed, LOW);
    digitalWrite(mcasLed, HIGH);
    tone(buzzerPin, 2000); // MCAS alert
  } else {
    digitalWrite(bankLed, LOW);
    digitalWrite(mcasLed, LOW);
    noTone(buzzerPin);
  }
  
  // LCD Update every 500 ms
  if (millis() - lastLCDUpdate >= 500) {
    lcd.clear();
    if (bankAngleAlert && mcasActive) {
      lcd.setCursor(0, 0);
      lcd.print("MCAS + BANK ANGLE");
    } else if (bankAngleAlert) {
      lcd.setCursor(0, 0);
      lcd.print("BANK ANGLE !!!");
    } else if (mcasActive) {
      lcd.setCursor(0, 0);
      lcd.print("MCAS ACTIVE !!!");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Stable Flight");
    }
    
    lcd.setCursor(0, 1);
    lcd.print("Bank:");
    lcd.print(bankAngle, 1);
    lcd.print((char)223);
    lcd.print(" N:");
    lcd.print(noseAngle, 1);
    
    lastLCDUpdate = millis();
  }
  
  // Serial output every 250 ms
  if (millis() - lastSerialPrint >= 250) {
    Serial.print("Bank Angle: ");
    Serial.print(bankAngle);
    Serial.print("°\tNose Angle: ");
    Serial.print(noseAngle);
    Serial.println("°");
    lastSerialPrint = millis();
  }
  
  delay(10);
}