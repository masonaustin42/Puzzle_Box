#include <SPI.h>               // include SPI bus library
#include <MFRC522.h>           // include RFID reader library
#include <FastLED.h>           // include FastLED Library
#include <Adafruit_LIS3MDL.h>  // include Adafruit Magnetometer library
#include <TinyGPSPlus.h>       // include GPS Data parsing library
#include <DFPlayerMini_Fast.h> // include mp3 player library
#include <Wire.h>
#include <math.h>


// GLOBALS
// Instantiate TinyGPS+ object
TinyGPSPlus gps;
// Define Objective Coords
const double Point1[] = { 47.422176222346536, -122.54161606046057 } // OBM
const double Point2[] = { 47.5424255792486 , -122.6373545951069 }   // Merc

const Points[] = { Point1, Point2 }
const int numPoints = 2


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  // if received valid location data from GPS
  if (gps.location.isValid()) {
    // check distance between Objectives and self
    double distances[]
    
    for (i = 0; i < numPoints; i++) {
      distances[i] = TinyGPSPlus.distanceBetween(
      gps.location.lat(), // current lat/lng
      gps.location.lng(),
      Points[i][0],       // objective lat/lng
      Points[i][1]
    ) / 1000.0
    }

    // Calculate bearings of each Objective

    double bearings[]

    for (i = 0; i < numPoints; i++) {
      bearings[i] = calculateBearing(gps.location.lat(), gps.location.lng(), Points[i][0], Points[i][1])
    }

    // draw the map
    drawMap(distances, bearings)
  }

}

// Function to draw the map based off of:
// - Distance between self and each Objective
// - Bearing to each Objective
// - Current facing direction based off of Magnetometer
void drawMap(double distances[], double bearings[]) {

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
