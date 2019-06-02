//#include <SoftwareSerial.h>
#include <Servo.h>

#define LASER 8
#define RED 4
#define WHITE 6
#define GREEN 10

//SoftwareSerial mySerial(2, 3);
Servo myservo;

String str;
const int len = 25;
int commaIndex, secCommaIndex, thrCommaIndex, dist;
float x, y, z, minValue, maxValue, oscl;
int i = 0, j = 0;
bool isP = true, isN = true;
float xarrP[len] = {0.0};
float xarrN[len] = {0.0};
int pos = 165;
float prevx, prevy, xvel, yvel, proj;
long int timeP, prevTime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LASER, OUTPUT);
  digitalWrite(LASER, HIGH);
  pinMode(RED, OUTPUT);
  pinMode(RED + 1, OUTPUT);
  pinMode(WHITE, OUTPUT);
  pinMode(WHITE + 1, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(GREEN + 1, OUTPUT);
  //mySerial.begin(9600);
  myservo.attach(9);
  myservo.write(pos);
}

void loop() {
  if (Serial.available() > 0)
  {
    str = Serial.readStringUntil('\n');
    commaIndex = str.indexOf(',');
    secCommaIndex = str.indexOf(',', commaIndex + 1);
    thrCommaIndex = str.indexOf(',', secCommaIndex + 1);

    x = str.substring(0, commaIndex).toFloat();
    y = str.substring(commaIndex + 1, secCommaIndex).toFloat();
    z = str.substring(secCommaIndex + 1, thrCommaIndex).toFloat();
    dist = str.substring(thrCommaIndex + 1).toInt();

    Serial.println(x);
    timeP = millis();
    if (timeP != timeP) {
      xvel = abs(x - prevx) / abs(timeP - timeP);
      yvel = abs(y - prevy) / abs(timeP - prevTime);
    }

    if (x >= 0 and isN) {
      minValue = xarrN[0];
      for (j = 0; j < len; j++) {
        Serial.print(xarrN[j]);
        if (xarrN[j] < minValue) {
          minValue = xarrN[j];
        }
        xarrN[j] = 0.0;
        Serial.print(',');
      }
      Serial.println("");
      Serial.print("minValue : ");
      Serial.println(minValue);
      isP = true;
      isN = false;
      i = 0;
    }
    if (x < 0 and isP) {
      maxValue = xarrP[0];
      for (j = 0; j < len; j++) {
        Serial.print(xarrP[j]);
        if (xarrP[j] > maxValue) {
          maxValue = xarrP[j];
        }
        xarrP[j] = 0.0;
        Serial.print(',');
      }
      Serial.println("");
      Serial.print("maxValue : ");
      Serial.println(maxValue);
      isP = false;
      isN = true;
      i = 0;
    }

    if (isP and x < 400) {
      xarrP[i] = x;
      i = i + 1;
    }
    if (isN and x > -400) {
      xarrN[i] = x;
      i = i + 1;
    }

    proj = (sqrt(0.196*dist + sq(yvel)) - yvel)/9.8;
    oscl = maxValue + abs(minValue);
    if (oscl > 300) {
      myservo.write(pos - 70);
      digitalWrite(RED, HIGH);
      digitalWrite(RED + 1, LOW);
      digitalWrite(WHITE, LOW);
      digitalWrite(WHITE + 1, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(GREEN + 1, LOW);
    }
    if (oscl > 200 and oscl < 300) {
      myservo.write(pos - 58);
      digitalWrite(RED, LOW);
      digitalWrite(RED + 1, LOW);
      digitalWrite(WHITE, HIGH);
      digitalWrite(WHITE + 1, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(GREEN + 1, LOW);

    }
    if (oscl > 75 and oscl < 200) {
      myservo.write(pos - 45);
      digitalWrite(RED, LOW);
      digitalWrite(RED + 1, LOW);
      digitalWrite(WHITE, LOW);
      digitalWrite(WHITE + 1, LOW);
      digitalWrite(GREEN, HIGH);
      digitalWrite(GREEN + 1, LOW);
    }
    if (oscl < 75) {
      myservo.write(pos);
      digitalWrite(RED, LOW);
      digitalWrite(RED + 1, LOW);
      digitalWrite(WHITE, LOW);
      digitalWrite(WHITE + 1, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(GREEN + 1, LOW);
    }

    if (i >= len)
      i = 0;

    prevx = x;
    prevy = y;
    prevTime = timeP;
  }
}
