/* 
 fc = 400
 fs = 2000
 [b,a] = butter(3,w,'high)
*/
#include <SPI.h>
#include <WiFiNINA.h>
#include <RTCZero.h>
#include <WiFiUdp.h>
#include <EMailSender.h>
#include "Secrets.h" 


// pin setup
int sensorPin = A7;

// filter setup
int n = 4;
float x[4], y[4], ynew;     // Space to hold previous samples and outputs
//float a[] = {1.0000,   -1.5906,    2.0838,   -1.5326,    0.8694,   -0.3192,   0.0821,   -0.0122,    0.0009};
//float b[] = {0.0293,   -0.2341,    0.8193,   -1.6386,    2.0483,   -1.6386, 0.8193,   -0.2341,    0.0293};
float a[] = {1.0000,   -0.5772,    0.4218,   -0.0563};
float b[] = {0.2569,   -0.7707,    0.7707,   -0.2569};

// Wifi Setup
char *ssid = SECRET_SSID; // network name
char *pass = SECRET_PASS; // network password
int status = WL_IDLE_STATUS;

// RTC Setup
// Time zone constant - change if necessary
const signed int UTC = -5;
WiFiClient client; // sets up the wifi client
// Create an rtc object
RTCZero rtc;
const signed int HOUR = 24;
int hours = 0;

// sendText Setup
// account must have correct settings. Let me know if you need my credentials
char *eMailUser = "stewartvas01@gmail.com";
// has to be an app password. Let me know if you need my credentials
char *eMailPass = "wwkjbinloskvtiix";
// phone number to use for to sending text message to
char phoneRecipient[] = "3193839547@txt.att.net";   // for at&t wireless. Replace * with a real phone number
//char phoneRecipient[] = "*************@vtext.com";   // for verizon wireless

int debugOn = 1;


// establish wifi connection
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
  pinMode(4, OUTPUT);
  pinMode(sensorPin, INPUT);
  digitalWrite(4, HIGH);
  analogReference(AR_DEFAULT);

  ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV64 |    // Divide Clock by 512.
                   ADC_CTRLB_RESSEL_10BIT;         // 10 bits resolution as default
  for (int i = 0; i < n; i++)   //  Clear x and y arrays
    x[i] = y[i] = 0.0;

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
  unsigned long t1;
  int i, count, val;
  float s;

  count = 0;
  while (1) {
    t1 = micros();

    // Apply a digital filter.
    for (i = 1; i < n; i++) {     // Shift samples
      x[n - i] = x[(n - 1) - i];
      y[n - i] = y[(n - 1) - i];
    }
    val = 1023 - analogRead(sensorPin);
    x[0] = val * (3.3 / 1023.0); // New input to filter
    ynew = 0.0;
    for (i = 0; i < n; i++)
      ynew = ynew + b[i] * x[i];
    for (i = 1; i < n; i++)
      ynew = ynew - a[i] * y[i];
    y[0] = ynew;                   // New output from filter

    s = abs(ynew);               // Use filtered signal
    Serial.println(ynew);

    //Serial.print("delay: ");
    //Serial.println(micros() - t1);
    //    Serial.println(val);
    //    delay(1000);
    //    // Now we can use the signal for its intended purpose. Below we check and turn an
    //    // LED on if the beam has been interrupted for.
    //
    if (s > 1)                 // Threshold voltage
      count++;
    else
      count--;
    if (count > 150) count = 150;
    if (count <= 0) count = 0;
    //
    if (count > 135) // normal
      digitalWrite(4, HIGH);
    else {
      // alerts 
      digitalWrite(4, LOW);
      //sendTextMessage();
    }
    //
    //    // The filter was designed assuming a 2000 Hz sampling rate. This corresponds
    //    // to a sample every 500 us.
    //
    while ((micros() - t1) < 489) // 1035 Was determined via experimentation
      ;
    //Serial.println(micros() - t1);
    //delay(100);
  }
}
