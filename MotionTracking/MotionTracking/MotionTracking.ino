#include "Wire.h"
#include <SeeedOLED.h>

// I2Cdev and MPU9250 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU9250.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU9250 accelgyro;
I2Cdev   I2C_M;

uint8_t buffer_m[6];

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

float Axyz[3]; 

int Current_steps=0;

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();

  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)
  Serial.begin(38400);

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
	Serial.println("Testing device connections...");
	Serial.println(accelgyro.testConnection() ? "MPU9250 connection successful" : "MPU9250 connection failed");

  // Display
  SeeedOled.init();
  
	delay(1000);

}

void loop() 
{   	
	getAccel_Data(); 
  Serial.println("z: "+String(Axyz[2]));
  //LED Level UP; Condition +-1.5g 
  if(abs(int(Axyz[2]*10))>15){
    Current_steps+=1;
    SeeedOled.clearDisplay();
    SeeedOled.putString("Step: ");
    SeeedOled.putNumber(Current_steps);
    Serial.println("Move");
    delay(25);
  }
  
  delay(100);
}

void getAccel_Data(void)
{
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  Axyz[0] = (double) ax / 16384;//16384  LSB/g
  Axyz[1] = (double) ay / 16384;
  Axyz[2] = (double) az / 16384; 
}
