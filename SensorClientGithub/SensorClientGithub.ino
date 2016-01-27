#include <SPI.h>
#include <Ethernet.h>

// sensor setting
int cid = 7;  // farm area number
int aid = 4;  // sensor group number
int snum = 5; // How many sensors?
int sid[] = {26, 27, 28, 29, 30}; // each number of sensor data 
char stype[] = {'1', '3', '4', '5', '6'}; // sensor data type

// network setting
static byte mac[] = { 0xDE, 0xAD, 0xB1, 0xEF, 0xFE, 0xEF }; 
IPAddress server(10, 10, 0, 1);  //server所在的IP
IPAddress ip(10, 10, 0, 1);
IPAddress subnet(255, 0, 0, 0);
IPAddress gateway(10, 10, 0, 254);
EthernetClient client;

// Httprequest GET format 
String strGet = "GET /ToDB.php?";
String strGet2 = "data=";
String strGet3 = "&type=";
String strGet4 = "&oid=";
String strHttp=" HTTP/1.1";

void setup() {
  Serial.begin(9600);

  // connect by DHCP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  IPAddress localAddr = Ethernet.localIP();

  // print your IP
  Serial.print("My IP address: ");
  Serial.println(localAddr);

  // request your IP to database
  String MyIpAddress;
  MyIpAddress = String(localAddr[0]) + "." + String(localAddr[1]) + "." + String(localAddr[2]) + "." + String(localAddr[3]);

  // connect server first time, send your setting 
  if (client.connect(server, 88)) {
    // Httprequest GET format 
    String allsid = "&sids=";
    String allstype = "&types=";
    for(int i = 0; i < snum; i++)
    {
        allsid = allsid + String(sid[i]);
        allstype = allstype + String(stype[i]);
    }
    
    String strSetup = "GET /ToDB.php?ip=" + MyIpAddress + "&aid=" + aid + "&snum=" + snum + allsid + allstype + "&cid=" + cid + "&setup=1" + strHttp;
    Serial.println(strSetup);
    client.println(strSetup);
    client.println("Host: 10.10.0.1");
    client.println("Connection: close");
    client.println();
    
    client.stop();
  }
  else {
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }

  
}

void loop()
{
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

   // write program about reading your sersors data and call httpRequest() to send your data to database
   // ...
   // ...

   // set a delay time for your sensors
    delay(1800000);
}

void httpRequest(String data, char type, int tempsid) {
  // Httprequest GET format
  // data value is your sensors data
  // type value is setting your sensors data type

  // -- That is my default setting.
  // 1: Temperature (using DHT11 
  // 3: Humidity (using DHT11
  // 4: Illuminance (using BH1750
  // 5: Soil Humidity (using SHT11
  // 6: Soil Temperature (using SHT11
  
  if (client.connect(server, 88)) {
    String strHttpGet = strGet + strGet2 + data + strGet3 + type + strGet4 + String(tempsid) + "&setup=0" + strHttp;
    Serial.println("connecting...");
    Serial.println(strHttpGet);
    client.println(strHttpGet);
    client.println("Host: 10.10.0.1");
    client.println("Connection: close");
    client.println();
    client.stop();
  }
  else {
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}

