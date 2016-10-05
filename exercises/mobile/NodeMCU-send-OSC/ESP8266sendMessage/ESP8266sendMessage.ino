/*---------------------------------------------------------------------------------------------

  Open Sound Control (OSC) library for the ESP8266

  Example for sending messages from the ESP8266 to a remote computer
  The example is sending "hello, osc." to the address "/test".

  This example code is in the public domain.

  This example code was edited by Joseph Paetz
  for use with Intro to Physical Computing (60-223) at
  Carnegie Mellon University.
  9-19-16

  Circuit: a switch connected to D0 with a pull-down resistor
--------------------------------------------------------------------------------------------- */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

#define BUTTON_PIN D0

/* YOU WILL NEED TO CHANGE THIS IF NOT IN THE LAB! */
char ssid[] = "PhysCompLab";          // your network SSID (name)
char pass[] = "";         // your network password

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP

/* YOU WILL NEED TO CHANGE THIS TO YOUR COMPUTER'S IP! */
const IPAddress outIp(127,0,0,1);        // remote IP of your computer

//this should match the port to listen on in the python sketch
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

void setup() {
    Serial.begin(115200);

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    //wait until connected
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Starting UDP");
    Udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(Udp.localPort());

    pinMode(BUTTON_PIN, INPUT);
}

int old_button;
void loop() {
    int button_state = digitalRead(D0);
    
    Serial.println("sending message...");

    /* This is the address of the message.
     * You can use this to distinguish between the
     * different values that you send. For example
     * you could have /button, /accel-x, /gyro-x, etc
     * for different sensors.
     */
    OSCMessage msg("/button");

    /* add data to the message. Any data type works. 
     * For example, msg.add("hi"); would also work
     * 
     * check out the documentation (https://github.com/CNMAT/OSC)
     * for all the other ways to use messages
     */
    msg.add(button_state);

    //send out the msg over udp
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();

    //send a message with a different address if button was pressed
    if (old_button == 0 and button_state == 1) {
      OSCMessage msg("/text");
      msg.add("Button Pressed!!");
      Udp.beginPacket(outIp, outPort);
      msg.send(Udp);
      Udp.endPacket();
      msg.empty();
    }
    old_button = button_state;
    delay(500);
}

