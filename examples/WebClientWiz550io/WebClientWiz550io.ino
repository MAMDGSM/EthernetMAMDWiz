/*
 Web clientWiz550io
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino & Wiznet Ethernet shield.  
 
 Circuit:
 * Ethernet shield attached to pins 
 -10 SS/SC, 11 MOSI, 12 MISO, 13 SLK (UNO)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen
 modified 12 Aug 2013
 by Soohwan Kim
 modified 03 Apr 2014
 by Mortadha DAHMANI
 
 |-----       -----|
 |      RJ-45      |
 |    Connector    |
 |_________________|
 Wiz550io Module
 Top view
 -------------------
 J1               J2
 1- GND   |  *******             
 2- GND   |  3.3V -1
 3- MOSI  |   RDY -2
 4- MISO  |   RST -3
 5- SLK   |    NC -4
 6- SS    |   INT -5
 7- 3.3V  |   GND -6
 8- 3.3V  |  *******
 --------------------
 */

#include <SPI.h>
#include <EthernetMAMDWiz.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
#if defined(WIZ550io_WITH_MACADDRESS) // Use assigned MAC address of WIZ550io
;
#else
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
#endif  
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "www.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,2,177);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
#if defined(WIZ550io_WITH_MACADDRESS)
  if (EthernetMAMDWiz.begin() == 0) {
#else
  if (EthernetMAMDWiz.begin(mac) == 0) {
#endif  
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
#if defined(WIZ550io_WITH_MACADDRESS)
    EthernetMAMDWiz.begin(ip);
#else
    EthernetMAMDWiz.begin(mac, ip);
#endif  
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("Connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("Connected");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("Connection failed");
  }
}

void loop()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting.");
    client.stop();
    // do nothing forevermore:
    while(true);
  }
}

