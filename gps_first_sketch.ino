#include <LiquidCrystal.h>
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <string.h>

// Define our Software Serial Connection With GPS - GPS RX is 8, TX is 9
SoftwareSerial serial_connection(8, 9); 
//This is the GPS object that allows us to read in data, measure, etc
TinyGPSPlus gps;

// LCD Pins conneted to arduino - RS is register select which lets us pick from two registers to write to, the 
// one that allows us to position the cursor, or the one that allows us to write the message. 
// En is enable which lets us write to the registers
const int rs = 11, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

// Initialize our lcd variable with the above values. Later we'll write like lcd.println etc
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Declare an array of mixed types of char(string) and double(number) - used when looping through comparing distances
struct Poi {
  char place_name[14];
  double lat;
  double lng;
};

// Declare variables to be output to our LCD
char first_name[14];
char second_name[14];

// Initialize distance variables with large numbers so that logic written below works and overwrites. 
long first_distance = 100000;
long second_distance = 111111;
String first_result;
String second_result;

// Define our array of locations and names
Poi poi_list[38] = {
{"Target E.    ",32.965764,-96.6460794}
,
{"Target W.    ",32.9652453,-96.646699}
,
{"Office Depot ",32.9648976,-96.6471899}
,
{"Michaels     ",32.9661522,-96.645606}
,
{"Petsmart     ",32.9663851,-96.6453928}
,
{"Cheddars     ",32.9662152,-96.6444272}
,
{"Chicfila W   ",32.9658597,-96.6440074}
,
{"Chicfila E   ",32.9658473,-96.6438411}
,
{"Walmart Garde",32.9633077,-96.642327}
,
{"Walmart Main ",32.9629038,-96.642842}
,
{"Walmart Food ",32.9624649,-96.6433865}
,
{"Academy      ",32.9592816,-96.6467446}
,
{"Excel North  ",32.9617054,-96.7066517}
,
{"Kroger North ",32.977509,-96.6642554}
,
{"Kroger South ",32.9770995,-96.6642755}
,
{"Texas Roadhou",32.963124,-96.636627}
,
{"Fry's        ",33.0068033,-96.7045033}
,
{"Costco       ",33.0444814,-96.6997021}
,
{"Red Lobster  ",33.0444972,-96.7015958}
,
{"Movies AMC   ",32.9505165,-96.6139224}
,
{"B & N South  ",32.9532781,-96.613453}
,
{"B&N North    ",32.9537913,-96.6130802}
,
{"Macy's N     ",32.9524206,-96.6094002}
,
{"Macy's W     ",32.9523171,-96.6098562}
,
{"Macy's SW    ",32.9519119,-96.6097704}
,
{"Macy's SE    ",32.9516644,-96.6092205}
,
{"Macy's E     ",32.9517949,-96.6087833}
,
{"Old Navy     ",32.9524994,-96.6123775}
,
{"Panera W     ",32.9539106,-96.6168943}
,
{"Panera E     ",32.953861,-96.6167387}
,
{"In N' Out    ",32.9540388,-96.6174079}
,
{"E Home Depot ",32.9462332,-96.6152769}
,
{"W Home Depot ",32.946375,-96.6159314}
,
{"Best Buy     ",32.9456626,-96.6140002}
,
{"Bop Joa      ",32.9685094,-96.6048297}
,
{"Ranger Stadi ",32.7502775,-97.0836282}
,
{"Loser Pool   ",32.9861544,-96.5535352}
,
{"pool         ",32.9623074,-96.6515243}
};
int number_of_points = 38;



// Takes that mixed array struct and calculates the distance to the given point from our location
float calculate_distance(Poi point){
  long distance = gps.distanceBetween(
    gps.location.lat(),
    gps.location.lng(),
    point.lat,
    point.lng);
    return distance;
}

String convert_distance(float distance_meter){
  float distance = float(distance_meter); // converts long int to float
  char decimal_result[5];
  String distance_result;
  char result[5];
  
  if(distance > 999.99 && distance < 9999.99){
    distance = ("%.1f", distance / 1000);
    // convert to 3 digits 
    distance_result = String(distance);
    
  // If the distance is less than 100, we have to create a string starting with 0 so it overwrites the old distance, otherwise we end up with
  //  100 meters, turns into 199 meters because the 1 on the lcd is not updated and it's outputting 99. 
  }else if(distance_meter < 99.99){  
    String temp_distance_result = "0";
    distance_result = temp_distance_result +  String(distance_meter);
    distance_result = distance_result.substring(0, 3);
    
  //distance is going to be over 9.9km, meaning its 10.0 km so we change to just 10km
  }else if(distance_meter > 9999 && distance < 99999){ 
    distance_meter = distance_meter / 1000;
    distance_result = String(distance_meter);
    distance_result = distance_result.substring(0,2);
    distance_result = distance_result + "k";
  }else if(distance_meter > 99999){ 
    // distance will be over 999km so we just want to set it to 999km 
    distance_result = "FAR";
  }else if (distance_meter >= 100 && distance_meter <= 999.999){
    distance_result = String(distance_meter);
  }
  // return the 3 character thing so if it's greater than 9.9 km then it's just an integer and if it's
  // convert back into 3 digit float
  return distance_result;
}
// Takes first name and it's distance, second name and it's distance, and outputs to LCD
void output_to_lcd(){

  //convert distances back to float
  lcd.setCursor(0, 0);
  lcd.print(first_name);
  lcd.setCursor(13, 0);
  lcd.print(first_result);
  lcd.setCursor(0, 1);
  lcd.print(second_name);
  lcd.setCursor(13, 1);
  lcd.print(second_result);
}

// Our initial code that runs upon starting
void setup()
{
  Serial.begin(9600);//This opens up communications to the Serial monitor in the Arduino IDE
  serial_connection.begin(9600);//This opens up communications to the GPS
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Parkometer 2000");
  lcd.setCursor(0, 1);
  lcd.print("LOOKING 4 SAT...");
}

// Main function, checks our current position, iterates through points list measuring distance each time,
  // replaces first or second place with appropriate name, outputs to LCD
void loop()
{ 
  while(serial_connection.available())
  {
    gps.encode(serial_connection.read());
  }
  if(gps.location.isUpdated()){
  // When GPS is updated compare first place and second place to each distance in the list of POIs
  // Replace as necessary, convert to proper output based on distance 030, 2.1, 11k, and output
    for(int i=0;i<number_of_points;i++){
        // compare first position name with current position being iterated name,
          // Used later to prevent first position from taking up first and second position
        bool same_name = strcmp(first_name, poi_list[i].place_name) ;
        long distance = calculate_distance(poi_list[i]);
        if(distance < first_distance){
          strcpy(first_name, poi_list[i].place_name);
          first_distance = distance;
          first_result = convert_distance(first_distance);
        } if (distance < second_distance && distance > first_distance && same_name){
          strcpy(second_name, poi_list[i].place_name);
          second_distance = distance;
          second_result = convert_distance(distance);
        }
        output_to_lcd();
    }
  }
}