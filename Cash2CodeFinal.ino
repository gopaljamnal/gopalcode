
#include <UnoWiFiDevEd.h>
#include <Wire.h>
#include <stdio.h>


// we assigned weight as pressure = 5, motion = 2, Noise = 1 and Light = 2

//weights
double pw = .5;
double  mw = .2;
double nw = .1;
double lw = .2;
int PSW;
int MSW;
int NSW;
int LSW;

// state
int state;

//pressure
int fsrpin = A3;
int fsrval;
int pval;
//timer
int t=1;

//Noise variables
int noisepin = A0;
int noiseval = 0;
int nv = 0;

//PIR variables
int pirpin = 4;
int pirval = 0;

//ultrasonic variables
int echopin = 8;
int trigpin = 9;
int duration = 0;
int distance = 0;
int USS;
//light variables
int lightpin = A1;
int lightval = 0;
int LS;
// holding all sensor values
int allval[4] = {0,0,0,0};
int sum = 0;
//fuzzy score
int fuzzy_score = 0;
//buffer  
char buffer[100];
void setup() {
// put your setup code here, to run once:
Wifi.begin();
//Serial.begin(9600);
pinMode(trigpin, OUTPUT);
pinMode(echopin, INPUT);
pinMode(pirpin, INPUT);
pinMode(lightpin, INPUT);
pinMode(noisepin,INPUT);
pinMode(fsrpin, INPUT);
}
// put your main code here, to run repeatedly:
void loop() {
//pressure
fsrval = analogRead(fsrpin);
if(fsrval > 966)
{ pval = 1;
PSW = pval*pw*10;
}
else if(fsrval <967)
{ pval = 0;
PSW = pval*pw*10;
}
//light  
lightval = analogRead(lightpin);
if (lightval >400)
{LS = 1;
LSW = LS*lw*10;
}
else if(lightval <400 )
{LS = 0;
LSW = LS*lw*10;
}
//ULTRA SONIC
digitalWrite(trigpin, LOW);
delay(2);
digitalWrite(trigpin, HIGH);
delay(10);
digitalWrite(trigpin, LOW);
duration = pulseIn(echopin,HIGH);
distance = duration*0.034/2;
if(distance < 100)
{ USS = 1;}
else
{USS = 0;}
//Motion
pirval = digitalRead(pirpin);
if(pirval == 1)
{ MSW = pirval*mw*10;
}
else if (pirval == 0)
{
  MSW = pirval*mw*10;
}

//Noise
noiseval = analogRead(noisepin);
if (noiseval > 320)
{nv=1;
NSW = nv*nw*10;
}
else if (noiseval <320)
{nv=0 ;
NSW = nv*nw*10;
}
for(int i = 0; i < 4 ; i++)
{ 
//define a array to hold all values
allval[0]=PSW;
allval[1]=MSW;
allval[2]=NSW;
//allval[2]=USS;
allval[3]=LSW;

//sum total array value
//sum +=   allval[i];
 sum = sum + allval[i];


//i++;
}
if(sum >= 6)
{
  // sleeping
  state = 1;
}
else if(sum == 5)
{
  //reading
  state = 2;  
}

else if(sum <5)
{
  //working
  state = 3;
}
 int k = sprintf(buffer, "%d %d %d %d %d %d %d", t, PSW, MSW, NSW, LSW, sum, state);
for (int L = 0; L<k;L++)
Wifi.print(buffer[L]);
Wifi.println();
//Serial.print(buffer[L]);
//Serial.println();

delay(10000);
t++;
sum = 0;
}
