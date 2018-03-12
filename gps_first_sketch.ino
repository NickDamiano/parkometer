#include "TinyGPS++.h"
#include "SoftwareSerial.h"

SoftwareSerial serial_connection(10, 11); //RX=pin 10, TX=pin 11
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
TinyGPSLocation loc;

struct Poi {
  char name[30];
  double lat;
  double lng;
};

Poi poi_list[13] = {
{"AK Plaza W",36.9905263,127.0847449}
,
{"AK Plaza East",36.9914775,127.0861048}
,
{"Ramen Place",36.9923795,127.0895004}
,
{"BX Center",37.0833073,127.0339894}
,
{"BX West",37.083288,127.0350462}
,
{"Osan Chilis",37.0834186,127.0365375}
,
{"Work Main Gate",36.9643656,127.0219141}
,
{"Commissary",36.9639905,127.0024359}
,
{"PX Food Court",36.9647878,127.0002204}
,
{"PX Main entrance",36.9650471,126.9993058}
,
{"One Stop",36.9484856,127.0221126}
,
{"AMC",36.9578653,127.0425832}
,
{"Braii Republic",36.9599206,127.0446378}
};
int number_of_points = 13;

float calculate_distance(Poi point){
  float distance = gps.distanceBetween(
    gps.location.lat(),
    gps.location.lng(),
    point.lat,
    point.lng);
    Serial.println("");
    Serial.print("lat is ");
    Serial.print(point.lat, 7);
    Serial.println("");
    Serial.print("longitutde is ");
    Serial.print(point.lng, 7);
    Serial.print("DISTANCE DISTANCE DISTANCE KM:");
    Serial.println(distance /1000);
    Serial.println("");
    return distance;
}

void setup()
{
  Serial.begin(9600);//This opens up communications to the Serial monitor in the Arduino IDE
  serial_connection.begin(9600);//This opens up communications to the GPS
  Serial.println("GPS Start");//Just show to the monitor that the sketch has started 

}

void loop()
{
  while(serial_connection.available())
  {
    gps.encode(serial_connection.read());
  }
  if(gps.location.isUpdated())
    //Serial Print Satellite info
    Serial.println("SATELLITE INFO FOR MY POSITION START");
    Serial.println("Satellite Count:");
    Serial.println(gps.satellites.value());
    Serial.println("Latitude:");
    Serial.println(gps.location.lat(), 6);
    Serial.println("Longitude:");
    Serial.println(gps.location.lng(), 6);
    Serial.println("SATELLITE INFO FOR MY POSITION END");
    Serial.println("");

    for(int i=0;i<number_of_points;i++){
        Serial.print("Name: ");
        Serial.println(poi_list[i].name);
        Serial.print("Distance: ");
        Serial.println(calculate_distance(poi_list[i]));
    }
}