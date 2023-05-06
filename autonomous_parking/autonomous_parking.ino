// Otonom Park Eden Araba
// Developed by Furkan Hanilçi 
 
//motorların tanımlamaları

#include <AFMotor.h>
AF_DCMotor sol_on_motor(4);
AF_DCMotor sag_on_motor(3);
AF_DCMotor sol_arka_motor(1);
AF_DCMotor sag_arka_motor(2);
#include <Ultrasonic.h>
 
Ultrasonic ultrasonic_arka(40,41),ultrasonic_sol_arka(38,39),ultrasonic_sol_on(36,37),ultrasonic_on(34,35);
// ultrasomik sensörlerin tanımlamamalrı
 
#define sol 0 //sol yön komutu
#define sag 1 //sağ yön komutu
#define ileri 2 //ileri yön komutu
#define geri 3 //geri yön komutu
#define minimum_limit 15 //Arabanın genişliği (cm)
#define minimum_limit1 28 //arabanın uzunluğu (cm)
 
byte park_durumu = 0;
int sinyalpin = 21;
volatile int val;
 
int sayac = 0;
int guncel_durum = 0;
int onceki_durum = 0;
 
void say(int saydir)
{
for (int i = 0 ; i <= saydir; i+1)
{
val = digitalRead(sinyalpin);
if (val == LOW) {
 
guncel_durum = 0;
}
else {
 
guncel_durum = 1;
}
 
if(guncel_durum != onceki_durum)
{
if(guncel_durum == 1)
{
sayac = sayac + 1;
Serial.println(sayac);
i = i+1;
}
else
{
i = i ;
}
 
onceki_durum = guncel_durum;
 
}
if (i == saydir)
{
 
sol_on_motor.run(RELEASE);
sag_on_motor.run(RELEASE);
sol_arka_motor.run(RELEASE);
sag_arka_motor.run(RELEASE);
 
}
 
}
 
}
 
void motor_pin_setup()
{
 
sol_on_motor.run(RELEASE);
sag_on_motor.run(RELEASE);
sol_arka_motor.run(RELEASE);
sag_arka_motor.run(RELEASE);
}
 
// Hareket fonksiyonları

void robot_hareket(byte motor, byte spd)
{
if (motor == ileri)
{
sol_on_motor.setSpeed(spd);
sag_on_motor.setSpeed(spd);
sol_arka_motor.setSpeed(spd);
sag_arka_motor.setSpeed(spd);
sol_on_motor.run(FORWARD);
sag_on_motor.run(FORWARD);
sol_arka_motor.run(FORWARD);
sag_arka_motor.run(FORWARD);
 
}
if (motor == geri)
{
sol_on_motor.setSpeed(spd);
sag_on_motor.setSpeed(spd);
sol_arka_motor.setSpeed(spd);
sag_arka_motor.setSpeed(spd);
sol_on_motor.run(BACKWARD);
sag_on_motor.run(BACKWARD);
sol_arka_motor.run(BACKWARD);
sag_arka_motor.run(BACKWARD);
 
}
if (motor == sol)
{
sol_on_motor.setSpeed(spd);
sag_on_motor.setSpeed(spd);
sol_arka_motor.setSpeed(spd);
sag_arka_motor.setSpeed(spd);
sol_on_motor.run(BACKWARD);
sag_on_motor.run(FORWARD);
sol_arka_motor.run(BACKWARD);
sag_arka_motor.run(FORWARD);
 
}
 
if (motor == sag)
{
sol_on_motor.setSpeed(spd);
sag_on_motor.setSpeed(spd);
sol_arka_motor.setSpeed(spd);
sag_arka_motor.setSpeed(spd);
sol_on_motor.run(FORWARD);
sag_on_motor.run(BACKWARD);
sol_arka_motor.run(FORWARD);
sag_arka_motor.run(BACKWARD);
 
}
 
}
 
void robot_dur()
{
sol_on_motor.run(RELEASE);
sag_on_motor.run(RELEASE);
sol_arka_motor.run(RELEASE);
sag_arka_motor.run(RELEASE);
}
 
// Park yeri arama

bool park_yeri_kontrol()
{
 
long on_sensor = ultrasonic_on.Ranging(CM);
long sag_sensor = ultrasonic_sol_on.Ranging(CM);
long sag_arka_sensor =ultrasonic_sol_arka.Ranging(CM);
 
if( (sag_sensor <= minimum_limit)&&(sag_arka_sensor <= minimum_limit)&&(park_durumu == 0))
{
robot_hareket(ileri, 100);
park_durumu = 1; Serial.println(park_durumu);
}
 
if((sag_sensor > minimum_limit)&&(sag_sensor < minimum_limit1)&&(sag_arka_sensor > minimum_limit)&&(sag_arka_sensor < minimum_limit1)&&(park_durumu == 1))
{
robot_hareket(ileri, 100);
park_durumu = 2;Serial.println(park_durumu);
}
 
if((sag_sensor >= minimum_limit1)&&(sag_arka_sensor >= minimum_limit1)&&(park_durumu == 1))
{
// Dik Park Etme Kararı

robot_dur() ;
delay(500);
park_durumu = 10;Serial.println(park_durumu);
}
 
if((sag_sensor <= minimum_limit)&&(sag_arka_sensor <= minimum_limit)&&(park_durumu == 2))
{
//Paralel Park Etme Kararı
 
park_durumu = 3; Serial.println(park_durumu);
}
 
return park_durumu;
}
 
void park_bul()
{
park_yeri_kontrol();
if(park_durumu == 3 )
{
robot_dur();Serial.println(park_durumu);
delay(400);
park_durumu = 4;
}
if(park_durumu == 4 )
{
 
robot_hareket(geri,120);
say(18);
robot_dur();Serial.println(park_durumu);
delay(500);
robot_hareket(Sag,180);
say(9);
robot_dur();
delay(500);
park_durumu = 5;
}
if(park_durumu == 5)
{
 
robot_hareket(geri,120);
long arka_sensor = ultrasonic_arka.Ranging(CM);Serial.println(arka_sensor);
 
if(arka_sensor>0 && arka_sensor <= 13)
{
robot_dur();
delay(400);
park_durumu = 6;
}
return arka_sensor;
}
 
if(park_durumu == 6)
{
robot_hareket(sol,150);
long sag_sensor = ultrasonic_sol_on.Ranging(CM); Serial.println(sag_Sensor);
long sag_arka_sensor = ultrasonic_sol_arka.Ranging(CM); Serial.println(sag_arka_Sensor);
 
if(sag_sensor == sag_arka_sensor)
{
robot_dur();
park_durumu = 7;
}
 
return sag_sensor,sag_arka_sensor;
}
if(park_durumu == 7)
{
long on_sensor = ultrasonic_on.Ranging(CM);
 
if(on_sensor<=6)
{
robot_dur();
park_durumu = 8;
}
else
{
robot_hareket(ileri,100);
}
return on_sensor;
}
if (park_durumu ==10)
{
 
robot_hareket(Sol,180);
say(40);
robot_dur();
delay(500);
park_durumu = 7;
 
}
 
}
 
void setup()
{
Serial.begin(9600);
attachInterrupt(5, say, CHANGE);
pinMode (sinyalpin, INPUT) ;
 
motor_pin_setup();
}
 
void loop()
{
Park_bul();
}
