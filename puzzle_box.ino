#include <WS2812Serial.h>      // include library to use serial port on Teensy for neopixels
#define USE_WS2812SERIAL
#include <FastLED.h>           // include FastLED Library
#include <TinyGPSPlus.h>       // include GPS Data parsing library
#include <Wire.h>
#include <math.h>

// GLOBALS

// Uncomment to enable serial print statements
#define DEBUG

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
uint8_t hue = 0;
int objective = 0;
int timeAtObjective = 0;

const int GPS_Baud = 9600;
// Instantiate TinyGPS+ object
TinyGPSPlus gps;

bool GPSConnected = false;

double bearing = 0;
double distance = 100.0;

// Instantiate DFPlayer trigger
// DFPlayerMini_Fast MP3;
const uint8_t MP3_PIN_1 = 2;
const uint8_t MP3_PIN_2 = 3;
unsigned long MP3_1_Millis = 0;
unsigned long MP3_2_Millis = 0;

unsigned long currentMillis = 0;


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
void debugPrint();
void debugPrintln();

void setup() {
  // put your setup code here, to run once:
  #ifdef DEBUG
  Serial.begin(9600);
  #endif

  pinMode(MP3_PIN_1, OUTPUT);
  pinMode(MP3_PIN_2, OUTPUT);

  Serial7.begin(GPS_Baud);

  FastLED.addLeds<WS2812SERIAL, LED_PIN, BRG>(leds, NUM_LEDS);
  FastLED.setBrightness(10);

}

void loop() {
  // update currentMillis value
  currentMillis = millis();

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
  EVERY_N_MILLISECONDS(10) {
    hue++;
  }

  // check how long we've been at currently selected obj to select next one after 300 seconds
  EVERY_N_SECONDS(1) {
    // debugPrint("Time at current Objective: ");
    debugPrint("Time at current Objective: ");
    // debugPrintln(timeAtObjective);
    debugPrintln(timeAtObjective);
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
  #ifdef DEBUG
  displayGPSInfo();
  #endif
  
  debugPrint("Objective Status: ");
  if (! gps.location.isValid()) {
    debugPrintln("GPS Searching...");
    ledSearching(false);
  } else if (distance < 0.1) {
    debugPrintln("Close to point");
    ledPulsing();
    // set timeAtObjective to 1 once, only when arriving at objective
    debugPrintln(timeAtObjective);
    if (timeAtObjective < 1 || timeAtObjective > 300) {
      timeAtObjective = 1;
      // play objective found mp3
      digitalWrite(MP3_PIN_2, LOW);
      };
  } else {
    debugPrintln("Displaying points...");
    ledPoints();
    debugPrint("Bearing: ");
    debugPrintln(bearing);
    debugPrint("Distance: ");
    debugPrintln(distance);
    // play next point mp3
      digitalWrite(MP3_PIN_1, LOW);
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

void updateMP3Pins(uint8_t pin) {
  // Checks to set MP3 pins back to high if they have been low for 200 milliseconds
  // Check pin #1 first
  if (MP3_PIN_1 == LOW) {
    // if it has been 200 millis since mp3 #1 was set high
    if (currentMillis - MP3_1_Millis >= 200) {
      digitalWrite(MP3_PIN_1, HIGH);
    }
  } else if (MP3_PIN_1 == HIGH) {
    // update mp3 #1 value to current time
    MP3_1_Millis = currentMillis;
  }

  // Same check for pin #2
  if (MP3_PIN_2 == LOW) {
    if (currentMillis - MP3_2_Millis >= 200) {
      digitalWrite(MP3_PIN_2, HIGH);
    }
  } else if (MP3_PIN_2 == HIGH) {
    MP3_2_Millis = currentMillis;
  }
}
