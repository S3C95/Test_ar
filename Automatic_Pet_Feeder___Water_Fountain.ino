#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>                                /* Enable the use of wifi module. Make sure you downloaded and installed the ESP8266 library*/
#include <BlynkSimpleEsp8266.h>                         /* Code for talking with Blynk*/
#include <Servo.h>
#include <TimeLib.h>                                    /* Program code related to Real Time Clock (RTC). */
#include <WidgetRTC.h>                                  /* Communication code with Blynk Real Time Clock Widget */

Servo Servo1;
BlynkTimer timer;                                       /* Define parameter for Blynk Timer */
WidgetRTC rtc;                                          /* Define parameter for RTC Widget */

char auth[] = "OCjElL13fBq5Y4if8r3wu_S46Y2A5QHK";       // Put in the Auth Token for the project from Blynk. You should receive it in your email.
char ssid[] = "umiayumi";                               // Key in your wifi name. You can check with your smart phone for your wifi name
char pass[] = "uacghdan";                               // Key in your wifi password.

unsigned long startMillis;                              /* start counting time for display refresh*/
unsigned long currentMillis;                            /* current counting time for display refresh */
const unsigned long period = 500;                      // refresh every X seconds (in seconds) Default 60000 = 1 minute
long duration1;                                         // variable for the duration of sound wave travel
long duration2;
int distance1;                                          // variable for the distance measurement
int distance2;
int pos = 0;
String masa = "";
int status_pos = 1;

//pin declare
uint8_t echo1 = D0;
uint8_t echo2 = D4;
uint8_t trig1 = D2;
uint8_t trig2 = D8;
uint8_t buzzer = D1;     //D4
uint8_t led = D5;       //D5
uint8_t servoPin = 12;  //D6                                      // Declare the Servo pin
uint8_t relay_pump = D7; // GPIO13 D7

BLYNK_CONNECTED() {                                       /* When Blynk server is connected, initiate Real Time Clock function */
  rtc.begin();
}

void setup() {
  Serial.begin(9600);                                     // Serial Communication is starting with 9600 of baudrate speed
  Servo1.attach(servoPin);                                // We need to attach the servo to the used pin number
  Servo1.write(0);
  pinMode(relay_pump, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);                                 // Sets the trigPin as an OUTPUT
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);                                  // Sets the echoPin as an INPUT
  //Serial.println("Ultrasonic Sensor HC-SR04 Test");       // print some text in Serial Monitor
  //Serial.println("with Arduino UNO R3");                        /* Initiate the Serial Monitor function */
  /* Declare the Output Pin D4 (GPIO 2) as an output pin for LED Light*/
  setSyncInterval(1);                                           /* Synchronise or read time from the Blynk Server every 1 second */
  Blynk.begin(auth, ssid, pass);                                /* Initiate the Blynk server login for the specific project*/
  while (Blynk.connect() == false ) {}                          /* If the Blynk Server not yet connected to nodeMCU, keep waiting here */
  setSyncInterval(10 * 60);                                     /* After successful login, change Synchornise Time reading for every 10 minute (Do not need to always check for the time)*/
  startMillis = millis();                                       /* Start record initial time for display refresh */

}

void loop() {
  Blynk.run();                                                                    /* allow the communication between Blynk server and Node MCU*/
  timer.run();                                                                    /* allow the Blynk timer to keep counting */

  currentMillis = millis();                                                       /* Keep counting time for display refresh */
  if (currentMillis - startMillis > period) {                                     /* For every 1 second, run the set of code*/
    String currentTime = String(hour()) + ":" + minute() + ":" + second();        /* Define "currentTime" by combining hour, minute and second */
    String currentDate = String(day()) + " " + month() + " " + year();            /* Define "currentDate" by combining day, month, and year */
    masa = String(hour()) + "" + minute() + "" + second();
    Serial.print("Current time: ");                                               /* Display values on Serial Monitor */
    Serial.print(currentTime);
    Serial.print(" ");
    Serial.print(currentDate);
    Serial.print(" ");
    Serial.print(masa);
    Serial.println();

    Blynk.virtualWrite(V1, currentTime);                                          /* Send Time parameters to Virtual Pin V1 on Blynk App */
    Blynk.virtualWrite(V2, currentDate);                                          /* Send Date parameters to Virtual Pin V2 on Blynk App */

    startMillis = millis();                                                       /* Reset time for the next counting cycle */
  }
  
  if (masa == "235613" || masa == "235813" || masa == "235913" ) { //food
    Serial.println("START ROLL ");
      for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
        Servo1.write(pos); // tell servo to go to position in variable 'pos'
        delay(15); // waits 15ms for the servo to reach the position
        Serial.print("START = ");
        Serial.println(pos)
      }
      for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        Servo1.write(pos); // tell servo to go to position in variable 'pos'
        delay(15); // waits 15ms for the servo to reach the position
        Serial.print("START = ");
        Serial.println(pos);
      }
      Serial.println("END ROLL ");
  }
  measure();

  if (distance1 >= 20 ) {
    digitalWrite(relay_pump, HIGH);
  }
  else {
    digitalWrite(relay_pump, LOW);
  }

  if (distance2 >= 20 ) {
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
}

void measure() {
  digitalWrite(trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  duration1 = pulseIn(echo1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  digitalWrite(trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  duration2 = pulseIn(echo2, HIGH);
  distance2 = duration2 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  Serial.print("Distance_1: ");
  Serial.print(distance1);
  Serial.println(" cm");
  Serial.print("Distance_2: ");
  Serial.print(distance2);
  Serial.println(" cm");

  Blynk.virtualWrite(V3, distance1);
  Blynk.virtualWrite(V4, distance2);
  return;  
}
