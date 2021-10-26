#include <SPI.h>
#include <WiFiNINA.h>
#include <RTCZero.h>
#include <WiFiUdp.h>
#include <EMailSender.h>
#include "Secrets.h" 


//Wifi credentials
char *ssid = SECRET_SSID; // network name
char *pass = SECRET_PASS; // network password

// account must have correct settings. Let me know if you need my credentials
char *eMailUser = "stewartvas01@gmail.com";
// has to be an app password. Let me know if you need my credentials
char *eMailPass = "wwkjbinloskvtiix";
// phone number to use for to sending text message to
char phoneRecipient[] = "3193839547@txt.att.net";   // for at&t wireless. Replace * with a real phone number
//char phoneRecipient[] = "*************@vtext.com";   // for verizon wireless

int status = WL_IDLE_STATUS;
// Time zone constant - change if necessary
const signed int UTC = -5;
WiFiClient client; // sets up the wifi client
// Create an rtc object
RTCZero rtc;
const signed int HOUR = 24;
int hours = 0;


int debugOn = 1;

// establish Wifi Connection
void connectWifi() {
  while (status != WL_CONNECTED) {
    if (debugOn) {
      Serial.print("Attempting to connect to wifi network: ");
      Serial.println(ssid);
    }
    status = WiFi.begin(ssid, pass); // this function from the WiFi library does the work
    // wait a second for connection
    Serial.println("WiFi Status: " + String(status));
    delay(750);
  }
  if (debugOn) {
    Serial.println("Connected");
    printWiFiStatus();
  }
}

// Function that prints out Wifi status
void printWiFiStatus() {

  // Print the network SSID
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

}


// Returns time in 12 hour AM/PM format
String displayTime() {
  uint8_t hours = getlocalTime();
  if ( hours == 12) {
    return displayTwoDigits(getlocalTime()) + ":" + displayTwoDigits(rtc.getMinutes()) + " PM";
  }
  if (hours > 12 && hours < 24) {
    return displayTwoDigits(getlocalTime() - 12) + ":" + displayTwoDigits(rtc.getMinutes()) + " PM";
  }
  if (hours >= 1 && hours <= 11) {
    return displayTwoDigits(getlocalTime()) + ":" + displayTwoDigits(rtc.getMinutes()) + " AM";
  }
  if (hours == 24) {
    return displayTwoDigits(12) + ":" + displayTwoDigits(rtc.getMinutes()) + " AM";
  }
}

// Function that returns accurate hours based on local time / Iowa City
int getlocalTime() {
  uint8_t hours = rtc.getHours();

  if (hours > 5 && hours <= 23) {
    hours = hours + UTC;
  }
  if (hours >= 0 && hours <= 5) {
    hours = HOUR + hours + UTC;
  }

  return hours;
}

// Function that takes care of 2 dgits output
String displayTwoDigits(int number) {
  if (number < 10) {
    return "0" + String(number);
  }
  return String(number);
}

// Function that sends text message
void sendTextMessage() {
  // sender's name
  char senderName[] = " Receiver";
  // text message to be sent
  String MESSAGE = " Critical Safety at " + displayTime();

  // Instantiate necessary objects to send message
  EMailSender emailSend(eMailUser, eMailPass, eMailUser, senderName);
  EMailSender::EMailMessage message;
  EMailSender::Response response;
  Serial.println("Sending text...");
  message.message = MESSAGE;

  response = emailSend.send(phoneRecipient, message); // send message to phone recipient

  // Print status
  Serial.println("Sending status: ");
  Serial.print(response.status);
  Serial.println(response.code);
  Serial.println(response.desc);
  Serial.println("");
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Check if the WiFi works
  if (WiFi.status() == WL_NO_SHIELD || WiFi.status() == WL_NO_MODULE) {
    // Wait until WiFi ready
    Serial.println("WiFi adapter/module not ready");
    while (true);
  }

  // Establish a WiFi connection
  connectWifi();
  
  // Start Real time clock
  rtc.begin();
  //delay(1000);
  // Variable to represent epoch
  unsigned long epoch = 0;

  // Take epoch without waiting for the serial monitor
  //while ((epoch == 0));
  // Waiting for the serial monitor to start taking epoch
  while ((epoch == 0) && (Serial)) {
    epoch = WiFi.getTime();
  }

  rtc.setEpoch(epoch);
}

void loop() {
  // Print real time every 30 seconds
  Serial.println("Current time...");
  Serial.println(displayTime());
  sendTextMessage();
  //delay(2000);
}
