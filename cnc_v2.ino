#include <EEPROM.h>
#define xdirpin 11
#define xsteppin 10
#define ydirpin 5
#define ysteppin 4
#define zdirpin 7
#define zsteppin 8
#define xenable 6
#define yenable 3
#define zenable 2
#define m0 9
#define sfet 13

int slength;
int arraysize;
int feedrate;
int dongu;
int gdeger;
int mod;
long x2;
long y2;
float x = 0;
float y = 0;
float z = 0;
float xcurrent = 0.0000;
float ycurrent = 0.0000;
float zcurrent = 0.0000;
float xkonum = 0;
float ykonum = 0;
float zkonum = 0;
String degisken;
String a;
bool state=false;
void setup() {
  Serial.begin(115200);
  pinMode(xenable, OUTPUT);
  pinMode(yenable, OUTPUT);
  pinMode(zenable, OUTPUT);
  pinMode(xdirpin, OUTPUT);
  pinMode(xsteppin, OUTPUT);
  pinMode(ydirpin, OUTPUT);
  pinMode(ysteppin, OUTPUT);
  pinMode(zdirpin, OUTPUT);
  pinMode(zsteppin, OUTPUT);
  pinMode(m0, OUTPUT);
  pinMode(sfet, OUTPUT);
  digitalWrite(m0, HIGH);
  digitalWrite(xenable, HIGH);
  digitalWrite(yenable, HIGH);
  digitalWrite(zenable, HIGH);
  Serial.setTimeout(5);

  EEPROM.get(10, xkonum);
  EEPROM.get(15, ykonum);
  EEPROM.get(20, zkonum);
}

void loop() {

state=false;
  while (!Serial.available())  {
  }
  a = Serial.readString();
  degisken = a.substring(0, 1);

  if (degisken == "G")  {      
  

    for (int u = 1; u < a.length(); u++) {
      if (a.substring(u, u + 1) == " ") {
        gdeger = a.substring(1, u).toInt();
        if (gdeger == 0) {
          state=true;
          feedrate = 20;
          u++;

        }
        else if (gdeger == 1)  {
          feedrate = 250;
          u++;
        state=true;

        }
        else if (gdeger == 4) {
          float pdeger = a.substring(4).toFloat();
          state=true;
          delay(pdeger * 1000);
          Serial.println("D");
          break;
        }
      }
      if (a.substring(u, u + 1) == "Z") {
        float zdeger = a.substring(5).toFloat();
        zcurrent = zdeger;
        state=true;

        if (zkonum > zcurrent) {
          digitalWrite(zdirpin, HIGH);
          z = ((zkonum - zcurrent) * 800);
        }
        else if (zkonum < zcurrent) {
          digitalWrite(zdirpin, LOW);
          z = ((zcurrent - zkonum) * 800);
        }
        long z2 = z;
        digitalWrite(zenable, LOW);
        for (long i = 0; i < z2; i++) {
          digitalWrite(zsteppin, HIGH);
          delayMicroseconds(feedrate);
          digitalWrite(zsteppin, LOW);
          delayMicroseconds(feedrate);
        }
        zkonum = zcurrent;
        EEPROM.put(20, zkonum);
           Serial.println("D");

        digitalWrite(zenable, HIGH);
        break;
      }

      if (a.substring(u, u + 1) == "Y") {
        
        xcurrent = a.substring(5, u).toFloat();
        ycurrent = a.substring(u + 1).toFloat();
        x = 0;
        y = 0;
        if (xkonum < xcurrent) {
          digitalWrite(xdirpin, HIGH);
          x = ((xcurrent - xkonum) * 800);
        }
        if (xkonum > xcurrent) {
          digitalWrite(xdirpin, LOW);
          x = ((xkonum - xcurrent) * 800);
        }
        if (ykonum < ycurrent)  {
          digitalWrite(ydirpin, HIGH);
          y = ((ycurrent - ykonum) * 800);
        }
        if (ykonum > ycurrent) {
          digitalWrite(ydirpin, LOW);
          y = ((ykonum - ycurrent) * 800);
        }
        x2 = x;
        y2  = y;
        while (y2 && x2 > 0) {
          digitalWrite(yenable, LOW);
          digitalWrite(xenable, LOW);
          digitalWrite(ysteppin, HIGH);
          digitalWrite(xsteppin, HIGH);
          delayMicroseconds(feedrate);
          digitalWrite(ysteppin, LOW);
          digitalWrite(xsteppin, LOW);
          delayMicroseconds(feedrate);
          x2--;
          y2--;
        }
        digitalWrite(yenable, HIGH);
        digitalWrite(xenable, HIGH);

        while (x2 > 0) {
          digitalWrite(xenable, LOW);
          digitalWrite(xsteppin, HIGH);
          delayMicroseconds(feedrate);
          digitalWrite(xsteppin, LOW);
          delayMicroseconds(feedrate);
          x2--;
        }
        digitalWrite(xenable, HIGH);
        while (y2 > 0) {
          digitalWrite(yenable, LOW);
          digitalWrite(ysteppin, HIGH);
          delayMicroseconds(feedrate);
          digitalWrite(ysteppin, LOW);
          delayMicroseconds(feedrate);
          y2--;
        }
        xkonum = xcurrent;
        ykonum = ycurrent;
        EEPROM.put(10, xkonum);
        EEPROM.put(15, ykonum);
    Serial.println("D");

        digitalWrite(yenable, HIGH);
        break;
      }
    }
  }
  else if (degisken == "M")  {
            state=true;

    int mdeger = a.substring(1).toInt();
    if (mdeger == 03) {
      for (int i2 = 15; i2 < 256; i2 += 16)  {
        analogWrite(sfet, i2);
        delay(50);
      }
    }
    else if (mdeger == 05) {
      digitalWrite(sfet, LOW);
      digitalWrite(xenable, HIGH);
      digitalWrite(yenable, HIGH);
      digitalWrite(zenable, HIGH);
    }
    Serial.println("D");
  }
  else if (degisken == "R") {
    state = true;
    if (EEPROM.get(10, xkonum) != 0) {
      Serial.print("In reset: ");
      Serial.println(xkonum);

      long  a = xkonum * 800;
      Serial.println(a);
      digitalWrite(xdirpin, LOW);
      digitalWrite(xenable, LOW);
      for (long i = 0; i < a; i++) {
        digitalWrite(xsteppin, HIGH);
        delayMicroseconds(20);
        digitalWrite(xsteppin, LOW);
        delayMicroseconds(20);
      }
      digitalWrite(xenable, HIGH);
      xkonum = 0;
      EEPROM.put(10, xkonum);
    }
    if ( EEPROM.get(15, ykonum) != 0) {
      long a = ykonum * 800;
      digitalWrite(ydirpin, LOW);
      digitalWrite(yenable, LOW);
      for (long i = 0; i < a; i++) {
        digitalWrite(ysteppin, HIGH);
        delayMicroseconds(20);
        digitalWrite(ysteppin, LOW);
        delayMicroseconds(20);
      }
      digitalWrite(yenable, HIGH);
      ykonum = 0;
      EEPROM.put(15, ykonum);
    }
    if ( EEPROM.get(20, zkonum) < 0) {
      long a = -zkonum * 800;
      digitalWrite(zdirpin, LOW);
      digitalWrite(zenable, LOW);
      for (long i = 0; i < a; i++) {
        digitalWrite(zsteppin, HIGH);
        delayMicroseconds(20);
        digitalWrite(zsteppin, LOW);
        delayMicroseconds(20);
      }
      digitalWrite(zenable, HIGH);
      zkonum = 0;
      EEPROM.put(20, zkonum);
    }
    if (EEPROM.get(20, zkonum) > 0) {
      long a2 = zkonum * 800;
      digitalWrite(zdirpin, HIGH);
      digitalWrite(zenable, LOW);
      for (long i = 0; i < a2; i++) {
        digitalWrite(zsteppin, HIGH);
        delayMicroseconds(20);
        digitalWrite(zsteppin, LOW);
        delayMicroseconds(20);
      }
      digitalWrite(zenable, HIGH);
      zkonum = 0;
      EEPROM.put(20, zkonum);
    }
    Serial.println("D");
  }
   if (state == false)
    Serial.println("D");

}


