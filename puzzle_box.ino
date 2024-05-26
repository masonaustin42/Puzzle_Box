#include <SPI.h>               // include SPI bus library
#include <MFRC522.h>           // include RFID reader library
#include <FastLED.h>           // include FastLED Library
#include <Adafruit_LIS3MDL.h>  // include Adafruit Magnetometer library
#include <Adafruit_Sensor.h>
#include <TinyGPSPlus.h>       // include GPS Data parsing library
#include <DFPlayerMini_Fast.h> // include mp3 player library
#include <Wire.h>
#include <math.h>


// GLOBALS

// Define GPS Points
const double Points[][2] = {{ 47.422176222346536, -122.54161606046057 }, // OBM
                            { 47.5424255792486, -122.6373545951069 },    // Merc
                            { 47.487875749473154, -122.61306548845718}}; // random road north of home
const int numPoints = 3;


const int GPS_Baud = 9600;
// Instantiate TinyGPS+ object
TinyGPSPlus gps;

// Set Compass pin
Adafruit_LIS3MDL lis3mdl;
const int LIS3MDL_PIN = 37;


// Instantiate LED Rings
const int NUM_LEDS = 93;
// const int leds[] = {32, 24, 16, 12, 8, 1};
const int LED_PIN = 2;

CRGB leds[NUM_LEDS];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial7.begin(GPS_Baud);

  lis3mdl.begin_SPI(LIS3MDL_PIN);
  lis3mdl.setPerformanceMode(LIS3MDL_HIGHMODE);

  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(10);

}

void loop() {
  // put your main code here, to run repeatedly:

  // read GPS data
  while (Serial7.available() > 0) {
    gps.encode(Serial7.read());
  }
  // if received valid location data from GPS
  if (gps.location.isValid()) {
    // check distance between Objectives and self
    double distances[numPoints];
    
    for (int i = 0; i < numPoints; i++) {
      distances[i] = TinyGPSPlus::distanceBetween(
      gps.location.lat(), // current lat/lng
      gps.location.lng(),
      Points[i][0],       // objective lat/lng
      Points[i][1]
    ) / 1000.0;
    }

    // read compass data
    lis3mdl.read();
    sensors_event_t event;
    lis3mdl.getEvent(&event);
    float curr_heading = atan2(event.magnetic.y, event.magnetic.x) * 180 / M_PI;
    // float curr_heading = 0.0;
    Serial.print("Heading: ");
    Serial.println(curr_heading);

    // Calculate bearings of each Objective

    double bearings[numPoints];

    for (int i = 0; i < numPoints; i++) {
      bearings[i] = calculateBearing(gps.location.lat(), gps.location.lng(), Points[i][0], Points[i][1], curr_heading);
    }

    // draw the map
    drawMap(distances, bearings);
    delay(300);
  }

}

// Function to draw the map based off of:
// - Distance between self and each Objective
// - Bearing to each Objective
// - Current facing direction based off of Magnetometer
void drawMap(double distances[], double bearings[]) {
  const int Rings[][2] = {{0, 31},
                          {32, 55},
                          {56, 71},
                          {72, 83},
                          {84, 91}};
  const int RingSizes[] = {32, 24, 16, 12, 8, 1};
  const int Ring6 = 92;

  FastLED.clear();
  int huee = 0;
  for (int point = 0; point < numPoints; point++){
    Serial.print("Distance: ");
    printDouble(distances[point], 1000);
    Serial.print("Bearing: ");
    printDouble(bearings[point], 1000);

    int selectedRing;
    if (distances[point] > 2 && distances[point] <= 4) {
      selectedRing = 4;
    } else if (distances[point] > 4 && distances[point] <= 6) {
      selectedRing = 3;
    } else if (distances[point] > 6 && distances[point] <= 8) {
      selectedRing = 2;
    } else if (distances[point] > 8 && distances[point] <= 10) {
      selectedRing = 1;
    } else if (distances[point] > 10) {
      selectedRing = 0;
    }

    int offset = 0;
    for (int i = 0; i < selectedRing; i++) {
      offset += RingSizes[i];
    }

    int pointLed = ((bearings[point] / 360) * RingSizes[selectedRing]) + offset;
    leds[pointLed].setHue(huee);
    huee += 100;
  }
  leds[92].setHue(200);
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
  bearing += curr_heading;

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
