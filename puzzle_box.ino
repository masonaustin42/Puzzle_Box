// #include <SPI.h>               // include SPI bus library
// #include <MFRC522.h>           // include RFID reader library
#include <WS2812Serial.h>      // include library to use serial port on Teensy for neopixels
#define USE_WS2812SERIAL
#include <FastLED.h>           // include FastLED Library
// #include <Adafruit_LIS3MDL.h>  // include Adafruit Magnetometer library
// #include <Adafruit_Sensor.h>
#include <TinyGPSPlus.h>       // include GPS Data parsing library
// #include <DFPlayerMini_Fast.h> // include mp3 player library
#include <Wire.h>
#include <math.h>


// GLOBALS

// Define GPS Points
const double Points[][2] = {
                            // { 47.542046203062654, -122.54614416764574},  // hal's parents
                            // { 47.46740713249172, -122.61465463015585},   // home
                            { 47.5424255792486, -122.6373545951069 },    // Merc
                            { 47.53358012994527, -122.60760145751593}, // Taco Bell
                            { 47.5166091859565, -122.62829766716061}, // Walmart
                            { 47.50396635151055, -122.65078295913231}, // SC
                            // { 47.55570665517761, -122.54354641428509},   //manchester
                            // { 47.56710558284195, -122.64955548857306},   //insight
                            // { 47.66473591230466, -122.69514208676632},   //trappers
                            // { 47.476690187351664, -122.5491264948875},   //banner forest
                            // { 47.422176222346536, -122.54161606046057 }, // OBM
                            // { 47.43409098724879, -122.54497605607479},   // Mom & Dad's
                            }; 
const int numPoints = 4;
int PointHues[] = {192, 64, 96, 160, 32, 210, 0};
// uint8_t hue = 0;
int objective = 0;
int timeAtObjective = 0;

const int GPS_Baud = 9600;
// Instantiate TinyGPS+ object
TinyGPSPlus gps;

bool GPSConnected = false;

double bearing = 0;
double distance = 100.0;

// Instantiate DFPlayer
// DFPlayerMini_Fast MP3;
// const uint8_t searchingTrack = 0;
// const uint8_t foundTrack = 1;
// const uint8_t objectiveTrack = 2;


// Instantiate LED Rings
const int NUM_LEDS = 93;
const int LED_PIN = 1;

CRGB leds[NUM_LEDS];

const int Rings[][2] = {{0, 31},
                          {32, 55},
                          {56, 71},
                          {72, 83},
                          {84, 91}};
const int RingSizes[] = {32, 24, 16, 12, 8, 1};
const int Ring6 = 92;

// declare function prototypes
void ledPoints();
void ledPulsing();
void ledSearching();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial2.begin(9600);
  // MP3.begin(Serial2);
  // MP3.volume(30);

  Serial7.begin(GPS_Baud);

  FastLED.addLeds<WS2812SERIAL, LED_PIN, BRG>(leds, NUM_LEDS);
  FastLED.setBrightness(10);

}

void loop() {
  // read GPS data
  while (Serial7.available() > 0) {
    gps.encode(Serial7.read());
  }
  // if received valid location data from GPS
  if (gps.location.isValid()) {
    // check distance between Objective and self
      distance = TinyGPSPlus::distanceBetween(
      gps.location.lat(),    // current lat/lng
      gps.location.lng(),
      Points[objective][0],  // objective lat/lng
      Points[objective][1]
    ) / 1000.0;
  }

  // Calculate bearings of each Objective
  bearing = calculateBearing(gps.location.lat(), gps.location.lng(), Points[objective][0], Points[objective][1]);


  // draw the map
  drawMap(distance, bearing);

  // update global hue value 
  // EVERY_N_MILLISECONDS(10) {
  //   hue++;
  // }

  // check how long we've been at currently selected obj to select next one after 300 seconds
  EVERY_N_SECONDS(1) {
    Serial.print("Time at current Objective: ");
    Serial.println(timeAtObjective);
    if (timeAtObjective > 0 && timeAtObjective < 180) {
      timeAtObjective += 1;
    } else if (timeAtObjective >= 180) {
      timeAtObjective = 0;
      objective += 1;
    }
  }

}

// Function to draw the map based off of:
// - Distance between self and current objective
// - Bearing to objective
void drawMap(double distance, double bearing) {
  displayGPSInfo();

  Serial.print("Objective Status: ");
  if (! gps.location.isValid()) {
    Serial.println("GPS Searching...");
    ledSearching(false);
    // if (! MP3.isPlaying()) {
    //   MP3.play(searchingTrack);
    // }
  } else if (distance < 0.1) {
    Serial.println("Close to point");
    ledPulsing();
    // set timeAtObjective to 1 once, only when arriving at objective
    Serial.println(timeAtObjective);
    if (timeAtObjective < 1 || timeAtObjective > 300) {
      timeAtObjective = 1;
      // MP3.play(objectiveTrack);
      };
  } else {
    Serial.println("Displaying points...");
    ledPoints();
    Serial.print("Bearing: ");
    Serial.println(bearing);
    Serial.print("Distance: ");
    Serial.println(distance);
    // if (! GPSConnected) {
    //   MP3.play(foundTrack);
    //   GPSConnected = true;
    // }
  }
  
  FastLED.show();
}

// Function to convert degrees to radians
double degreesToRadians(double degrees) {
  return degrees * (PI / 180.0);
}

// Function to calculate the bearing from current location to target location
double calculateBearing(double lat1, double lon1, double lat2, double lon2) {
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

  // Normalize the bearing to a 0-360 range
  bearing = fmod((bearing + 360.0), 360.0);

  return bearing;
}

