#include "TinyGPS++.h"
#include "SoftwareSerial.h"

SoftwareSerial serial_connection(10, 11); //RX=pin 10, TX=pin 11
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
TinyGPSLocation loc;
const double EIFFEL_TOWER_LAT = 36.96070;
const double EIFFEL_TOWER_LNG = 127.05692;
struct Poi {
  char name[30];
  float lat;
  float lon;
};

Poi poi_list[13] = {
{"AK Plaza W",127.0847449,36.9905263}
,
{"AK Plaza East",127.0861048,36.9905263}
,
{"Ramen Place",127.0895004,36.9905263}
,
{"BX Center",127.0339894,36.9905263}
,
{"BX West",127.0350462,36.9905263}
,
{"Osan Chilis",127.0365375,36.9905263}
,
{"Work Main Gate",127.0219141,36.9905263}
,
{"Commissary",127.0024359,36.9905263}
,
{"PX Food Court",127.0002204,36.9905263}
,
{"PX Main entrance",126.9993058,36.9905263}
,
{"One Stop",127.0221126,36.9905263}
,
{"AMC",127.0425832,36.9905263}
,
{"Braii Republic",127.0446378,36.9905263}
};
int number_of_points = 13;




void setup()
{
  Serial.begin(9600);//This opens up communications to the Serial monitor in the Arduino IDE
  serial_connection.begin(9600);//This opens up communications to the GPS
  Serial.println("GPS Start");//Just show to the monitor that the sketch has started

  //test for iterating through struct
      for(int i=0;i<number_of_points;i++){
        Serial.println(poi_list[i].name);
    }
    
  }

void loop()
{
  while(serial_connection.available())//While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if(gps.location.isUpdated())//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Serial Print Satellite info
    Serial.println("Satellite Count:");
    Serial.println(gps.satellites.value());
    Serial.println("Latitude:");
    Serial.println(gps.location.lat(), 6);
    Serial.println("Longitude:");
    Serial.println(gps.location.lng(), 6);
    Serial.println("Speed MPH:");
    Serial.println(gps.speed.mph());
    Serial.println("Altitude Feet:");
    Serial.println(gps.altitude.feet());
    Serial.println("");

    

    double distance =
      gps.distanceBetween(
        gps.location.lat(),
        gps.location.lng(),
        EIFFEL_TOWER_LAT,
        EIFFEL_TOWER_LNG);
      double courseTo =
        gps.courseTo(
          gps.location.lat(),
          gps.location.lng(),
          EIFFEL_TOWER_LAT,
          EIFFEL_TOWER_LNG);
    Serial.print("Distance (km) to Eiffel Tower: ");
    Serial.println(distance);
    Serial.print("Course to Eiffel Tower: ");
    Serial.println(courseTo);
    Serial.print("Human directions: ");
    Serial.println(gps.cardinal(courseTo));
  }
}


