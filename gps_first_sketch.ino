#include <LiquidCrystal.h>
#include "TinyGPS++.h"
#include "SoftwareSerial.h"

SoftwareSerial serial_connection(8, 9); //RX=pin 8, TX=pin 9
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
TinyGPSLocation loc;
const int rs = 11, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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


Poi poi_list[11] = {
{"TGT E Entrance",32.965764,-96.6460794}
,
{"TGT W. Entrance",32.9652453,-96.646699}
,
{"Office Depot",32.9648976,-96.6471899}
,
{"Michaels",32.9661522,-96.645606}
,
{"Petsmart",32.9663851,-96.6453928}
,
{"Cheddars",32.9662152,-96.6444272}
,
{"Chicfila W",32.9658597,-96.6440074}
,
{"Chicfila E",32.9658473,-96.6438411}
,
{"Walmart Garden",32.9633077,-96.642327}
,
{"Walmart Main",32.9629038,-96.642842}
,
{"Walmart Food",32.9624649,-96.6433865}
};
int number_of_points = 11;


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
  Serial.print("beginning");
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("hello world");
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