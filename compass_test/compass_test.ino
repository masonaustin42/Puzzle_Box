#include <SPI.h>               // include SPI bus library
#include <WS2812Serial.h>      // include library to use serial port on Teensy for neopixels
#define USE_WS2812SERIAL
#include <FastLED.h>           // include FastLED Library
#include <Adafruit_LIS3MDL.h>  // include Adafruit Magnetometer library
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>


// Set Compass pin
Adafruit_LIS3MDL lis3mdl;
const int LIS3MDL_PIN = 37;

// const float offsetX = 0.09457378;
// const float offsetY = 21.76815338;
// const float offsetZ = 31.36425958;

const float offsetX = 0.0;
const float offsetY = 0.0;
const float offsetZ = 0.0;
const float scaleX = 1.0;
const float scaleY = 1.0;
const float scaleZ = 1.0;

const float declinationAngle = 15.13;

// Instantiate LED Rings
const int NUM_LEDS = 93;
const int LED_PIN = 1;

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  lis3mdl.begin_SPI(LIS3MDL_PIN);
  lis3mdl.setPerformanceMode(LIS3MDL_HIGHMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_40_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);

  FastLED.addLeds<WS2812SERIAL, LED_PIN, BRG>(leds, NUM_LEDS);
  FastLED.setBrightness(10);

}

void loop() {
  // put your main code here, to run repeatedly:

  // read compass data
  lis3mdl.read();
  sensors_event_t event;
  lis3mdl.getEvent(&event);

  Serial.print("Raw:");
  Serial.print(event.magnetic.x);
  Serial.print(",");
  Serial.print(event.magnetic.y);
  Serial.print(",");
  Serial.println(event.magnetic.z);

  float calibratedX = (event.magnetic.x - offsetX) * scaleX;
  float calibratedY = (event.magnetic.y - offsetY) * scaleY;
  float calibratedZ = (event.magnetic.z - offsetZ) * scaleZ;

  float curr_heading = atan2(calibratedY, calibratedX) * 180 / M_PI;
  curr_heading += declinationAngle;

  if (curr_heading < 0) {
    curr_heading += 360;
  }
  Serial.print("Heading: ");
  Serial.println(curr_heading);

  // int selectedLED = ((int)round(curr_heading / 11.25) )% 32;
  int selectedLED = map(curr_heading, 0, 360, 0, 31);
  FastLED.clear();
  leds[selectedLED] = CRGB::Red;
  FastLED.show();
}


// Function to convert degrees to radians
double degreesToRadians(double degrees) {
  return degrees * (PI / 180.0);
}

// Function to calculate the bearing from current location to target location
double calculateBearing(double lat1, double lon1, double lat2, double lon2, float curr_heading) {
  // Convert latitude and longitude from degrees to radians
  lat1 = degreesToRadians(lat1);
  lon1 = degreesToRadians(lon1);
  lat2 = degreesToRadians(lat2);
  lon2 = degreesToRadians(lon2);
  
  // Calculate the difference in longitudes
  double deltaLon = lon2 - lon1;

  // Calculate the components of the bearing
  double y = sin(deltaLon) * cos(lat2);
  double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(deltaLon);
  
  // Calculate the initial bearing in radians
  double initialBearing = atan2(y, x);

  // Convert the bearing from radians to degrees
  double bearing = initialBearing * (180.0 / PI);

  // adjust for current heading
  bearing -= curr_heading;

  // Normalize the bearing to a 0-360 range
  bearing = fmod((bearing + 360.0), 360.0);

  return bearing;
}

void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

    Serial.print (int(val));  //prints the int part
    Serial.print("."); // print the decimal point
    unsigned int frac;
    if(val >= 0)
      frac = (val - int(val)) * precision;
    else
       frac = (int(val)- val ) * precision;
    int frac1 = frac;
    while( frac1 /= 10 )
        precision /= 10;
    precision /= 10;
    while(  precision /= 10)
        Serial.print("0");

    Serial.println(frac,DEC) ;
}
