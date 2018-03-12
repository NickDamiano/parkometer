#include "TinyGPS++.h"
#include "SoftwareSerial.h"

SoftwareSerial serial_connection(10, 11); //RX=pin 10, TX=pin 11
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
TinyGPSLocation loc;
void output_to_lcd();

struct Poi {
  char place_name[30];
  double lat;
  double lng;
};

char first_name[30];
char second_name[30];
long first_distance = 100000;
long second_distance = 111111;


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
  long distance = gps.distanceBetween(
    gps.location.lat(),
    gps.location.lng(),
    point.lat,
    point.lng);
    return distance;
}

void output_to_lcd(char frst_name[30], long frst_distance,char scond_name[30],  long scond_distance){
  Serial.println("LCD stuff here");
  Serial.print(frst_name);
  Serial.println(frst_distance);
  Serial.print(scond_name);
  Serial.println(scond_distance);
  Serial.println("END OF LCD STUFF");
}

void setup()
{
  Serial.begin(9600);//This opens up communications to the Serial monitor in the Arduino IDE
  serial_connection.begin(9600);//This opens up communications to the GPS
}

void loop()
{ 
  while(serial_connection.available())
  {
    gps.encode(serial_connection.read());
  }
  if(gps.location.isUpdated()){
  
    for(int i=0;i<number_of_points;i++){
        int distance = calculate_distance(poi_list[i]);
        if(distance < first_distance){
          strcpy(first_name, poi_list[i].place_name);
          first_distance = distance;
        } else if (distance < second_distance){
          strcpy(second_name, poi_list[i].place_name);
          second_distance = distance;
        }
        output_to_lcd(first_name, first_distance, second_name, second_distance);
    }
  }
}
