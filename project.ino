
#define SS_PIN 2  //D2
#define RST_PIN 0 //D1

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "internship-d8d7c.firebaseio.com"
#define FIREBASE_AUTH "KV3SDqf3n5g0Zd9qkE0G8GO5Q2JQy07QPqcQxhra"
#define WIFI_SSID "Redmi 5"
#define WIFI_PASSWORD "qwertyuiop"
const long utcOffsetInSeconds = 19800;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

String Get_Rfid(int);
void get_name(int);
void get_number(int);
void get_address(int);
void Store_name(String);
void Store_number(String);
void Store_address(String);
void Get_time(void);

int num_flag = 0, name_flag = 0, add_flag = 0, time_flag = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Store_name("NO_NAME");
  Store_number("NO_NUMBER");
  Store_address("NO_ADDRESS");

  Serial.println("Data are reset");
  timeClient.begin();
}
void loop()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  for (int k = 1; k <= 2; k++)
  {
    if (content.substring(1) == Get_Rfid(k)) //change UID of the card that you want to give access
    {
      get_name(k);
      get_number(k);
      get_address(k);
      
      break;
    }
  }
}

String Get_Rfid(int i)
{
  String c = "rfid";
  String res;
  c.concat(i);
  String id = c;
  id.concat("/id");
  res = Firebase.getString(id);
  Serial.println("rfid_value ");
  Serial.println(res);
  return res;
}

void get_name(int i)
{
  String c = "rfid";
  String Name;
  c.concat(i);
  String id = c;
  id.concat("/Name");
  Name = Firebase.getString(id);
  Serial.println("Name:");
  Serial.println(Name);
  Store_name(Name);
  //name_flag++;
}

void get_number(int i)
{
  String c = "rfid";
  String Number;
  c.concat(i);
  String id = c;
  id.concat("/Number");
  Number = Firebase.getString(id);
  Serial.println("Phone Number:");
  Serial.println(Number);
  Store_number(Number);
  //num_flag++;
}

void get_address(int i)
{
  String c = "rfid";
  String Address;
  c.concat(i);
  String id = c;
  id.concat("/Address");
  Address = Firebase.getString(id);
  Serial.println("Address:");
  Serial.println(Address);
  Store_address(Address);
  //add_flag++;
}

void Store_name(String Name)
{
  String logs = "logs/";
  logs.concat(name_flag);
  logs.concat("/Name");
  String place = "Unauthorized_users/";
  place.concat("/Name");
  String temp =  Firebase.getString(place);
  if (temp != Name)
  {
    Firebase.setString(place, Name);
    Firebase.setString(logs, Name);
    name_flag++;
  }
}

void Store_number(String Number)
{
  String logs = "logs/";
  logs.concat(num_flag);
  logs.concat("/Number");
  String place = "Unauthorized_users/";
  place.concat("/Number");
  String temp =  Firebase.getString(place);
  if (temp != Number)
  {
    Firebase.setString(place, Number);
    Firebase.setString(logs, Number);
    num_flag++;
  }
}

void Store_address(String Address)
{
  String logs = "logs/";
  logs.concat(add_flag);
  logs.concat("/Address");
  String temp;
  String place = "Unauthorized_users/";
  place.concat("/Address");
  temp =  Firebase.getString(place);
  if (temp != Address)
  {
    Firebase.setString(place, Address);
    Firebase.setString(logs, Address);
    add_flag++;
    Get_time();
  }
}

void Get_time(void)
{
  timeClient.update();
  String logs = "logs/";
  logs.concat(time_flag);
  logs.concat("/time");
  String time_;
  //  hours.concat("/hour");
  //  minutes.concat("/minutes");
  String place = "Unauthorized_users/";
  place.concat("/time");
  //temp =  Firebase.getString(place);
  time_ = timeClient.getHours();
  time_.concat(":");
  time_.concat(timeClient.getMinutes());
  //  Firebase.setString(place,Address);
  Firebase.setString(logs, time_);
  time_flag++;
}
