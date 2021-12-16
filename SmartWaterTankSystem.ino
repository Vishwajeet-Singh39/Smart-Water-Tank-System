#include <LiquidCrystal.h>

#define trigger 8
#define echo 9
#define motor 10

SoftwareSerial sgsm(0, 1);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

float time, distance;
int motor_status = 0, t = -1;
String sms;

void setup(){

    lcd.begin(16, 2);

    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Initialising");
    lcd.setCursor(6,1);
    lcd.print("GSM");
    sgsm.begin(9600);
    delay(3000);

    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("GSM");
    lcd.setCursor(2,1);
    lcd.print("Initialised");
    delay(3000);
    lcd.clear();

    sgsm.print("\r");
    sgsm.print("AT+CMGF=1\r");
    delay(2000);
    sgsm.print("AT+CMGR=1\r");

    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(motor, OUTPUT);

    lcd.print("  Water Level  ");
    lcd.setCursor(0, 1);
    lcd.print("   Indicator ");
    delay(500);
}

void loop(){

    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);

    time = pulseIn(echo, HIGH);
    distance = time * 340 / 20000;

    if (t != 100 - distance){

        lcd.clear();
        lcd.print("Water Tank at ");
        lcd.print(t);
        t = distance;
        delay(2000);
    }
    if (distance < 15 && motor_status == 1){
        
        digitalWrite(motor, LOW);

        lcd.clear();
        lcd.print("Water Tank Full ");
        lcd.setCursor(0, 1);
        lcd.print("Motor Turned OFF");
        delay(3000);

        motor_status = 0;
    }
    else if (distance > 80 && motor_status == 0){

        digitalWrite(motor, HIGH);

        lcd.clear();
        lcd.print("LOW Water Level");
        lcd.setCursor(0, 1);
        lcd.print("Motor Turned ON");
        delay(2000);

        motor_status = 1;
    }
    t = 100 - distance;
    if(sgsm.available()){
        sms=sgsm.readString();
    }
    if(sms=="STOP"){
        motor_status=-1;
        digitalWrite(motor, LOW);

        sgsm.print("AT+CMGS=\"+917055807310\"\r");
        sgsm.print("System is Offline");
        sgsm.print("AT+CMGR=1\r");
        sms=NULL;
    }
    else if(sms="START"){
        motor_status=0;
        
        sgsm.print("AT+CMGS=\"+917055807310\"\r");
        sgsm.print("System is Online");
        sgsm.print("AT+CMGR=1\r");
        sms=NULL;
    }
}