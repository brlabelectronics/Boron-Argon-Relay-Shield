/*-------------------------------------------------------------------------------
Argon Relay Time Scheduler
This is the code for the Particle Argon app that communicates and runs 
the Boron-Argon Relay Shield using the Argon.
Company; BRLAB ELECTRONICS
Website: www.brlabelectronics.com
Written By: Bryan Restine
Email: bryan.restine@brlabelectronics.com

Relay 1 Manual ON/OFF BLYNK(V1)
Relay 2 Manual ON/OFF BLYNK(V2)
Relay 3 Manual ON/OFF BLYNK(V3)
Relay 4 Manual ON/OFF BLYNK(V4)
Battery SoC Gauge BLYNK(V4)
Terminal BLYNK(V5)
ALL DAYS Button ON/OFF BLYNK(V6)
WEEKDAYS Button ON/OFF BLYNK(V7)
WEEKEND Button ON/OFF BLYNK(V8)
CUSTOM Button ON/OFF BLYNK(V9)
ALL DAYS Scheduler BLYNK(V10)
WEEKDAYS Scheduler BLYNK(V11)
WEEKEND Scheduler BLYNK(V12)
CUSTOM Scheduler BLYNK(V13)
---------------------------------------------------------------------------------*/
#define BLYNK_TEMPLATE_ID "TMPLGYDWlg-c"
#define BLYNK_DEVICE_NAME "Relay Shield Argon"
#define BLYNK_AUTH_TOKEN "C6Z6LgRQGOnbgXIhwlMKnFSmnjDbuaHE"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// This #include statement was automatically added by the Particle IDE.
#include <Blynk.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Attach virtual serial terminal to Virtual Pin V5
WidgetTerminal terminal(V5);

bool isFirstConnect = true;
// constant integer assignment
const int relay1 = 5;
const int relay2 = 6;
const int relay3 = 7;
const int relay4 = 8;

// integer variable assignments
int alldays;
int weekdays;
int weekend;
int custom;

// float assignemnts
float voltage = 0;

// Timing variables
unsigned long batterylastmillis = 0;
unsigned long currentTimelastmillis = 0;
unsigned long currentDaylastmillis = 0;
long startsecondswd;
long stopsecondswd;
long nowseconds;

BlynkTimer timer;

BLYNK_CONNECTED() {
if (isFirstConnect) {
  Blynk.syncAll();
  Blynk.notify("TIMER STARTING!!!!");
isFirstConnect = false;
}
}
// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
    if(param.asInt() == 1)
    {
    // execute this code if the switch widget is now ON
    digitalWrite(relay1, HIGH);  // Set digital pin 5 HIGH
    }
         else
        {
            // execute this code if the switch widget is now OFF
            digitalWrite(relay1, LOW);  // Set digital pin 5 LOW
        }

}
BLYNK_WRITE(V1)
{
    if(param.asInt() == 1)
    {
    // execute this code if the switch widget is now ON
    digitalWrite(relay2, HIGH);  // Set digital pin 6 HIGH
    }
         else
        {
            // execute this code if the switch widget is now OFF
            digitalWrite(relay2, LOW);  // Set digital pin 6 LOW   
        }

}
BLYNK_WRITE(V2)
{
    if(param.asInt() == 1)
    {
    // execute this code if the switch widget is now ON
    digitalWrite(relay3, HIGH);  // Set digital pin 7 HIGH
    }
         else
        {
            // execute this code if the switch widget is now OFF
            digitalWrite(relay3, LOW);  // Set digital pin 7 LOW 
        }

}
BLYNK_WRITE(V3)
{
    if(param.asInt() == 1)
    {
    // execute this code if the switch widget is now ON
    digitalWrite(relay4, HIGH);  // Set digital pin 2 HIGH
    }
         else
        {
          // execute this code if the switch widget is now OFF
          digitalWrite(relay4, LOW);  // Set digital pin 2 LOW     
        }

}
BLYNK_WRITE(V6)// AllDAYS Schedule Selected
{
    if(param.asInt() == 1)
    {
    alldays = 1;
    weekdays = 0;
    weekend = 0;
    custom = 0;
    Blynk.syncVirtual(V10);
    terminal.clear();
    terminal.println("ALL DAYS SCHEDULE IS ACTIVE");
    terminal.flush();
    }
         else
        {
        alldays = 0; 
        }

}


BLYNK_WRITE(V10) { // alldays schedule
  TimeInputParam t(param);
  
  if(alldays == 1)
  {
    terminal.clear();
    terminal.println("ALL DAYS SCHEDULE HAS BEEN ACTIVATIED AT CURRENT TIME");
    currentTime();
    terminal.flush();
    currentDay();
    int dayadjustment = -1;
    int currentweekday = Time.weekday();
    if(currentweekday == 1)
    {
      dayadjustment = 6; // need for getting Sunday particle Day 1 is sunday and blynk is day 7
    }
    if(t.isWeekdaySelected(Time.weekday() + dayadjustment))
    { //Time library starts week on Sunday, Blynk on Monday
      terminal.println("ALL DAYS ACTIVE today");
      terminal.flush();
    if (t.hasStartTime()) // Process start time
    {
      String startTime = String("START TIME: ") + String(t.getStartHour()) + ":" + String(t.getStartMinute());
      terminal.println(startTime);
      terminal.flush();
    }
    if (t.hasStopTime()) // Process stop time
    {
      String stopTime = String("STOP TIME: ") + String(t.getStopHour()) + ":" + String(t.getStopMinute());
      terminal.println(stopTime);
      terminal.flush();
    }
    // Display timezone details, for information purposes only 
    terminal.println(String("Time zone: ") + t.getTZ()); // Timezone is already added to start/stop time 
    //  terminal.println(String("Time zone offset: ") + t.getTZ_Offset()); // Get timezone offset (in seconds)
    terminal.flush();
  
     for (int i = 1; i <= 7; i++) 
     {  // Process weekdays (1. Mon, 2. Tue, 3. Wed, ...)
        if (t.isWeekdaySelected(i)) {
        String selectedday = String("SELECTED DAY: ") + String(i);
        terminal.println(selectedday);
        terminal.flush();
        }
      } 
        nowseconds = ((Time.hour() * 3600) + (Time.minute() * 60) + Time.second());
        startsecondswd = (t.getStartHour() * 3600) + (t.getStartMinute() * 60);
        //Serial.println(startsecondswd);  // used for debugging
        terminal.print("Now Seconds: ");
        terminal.println(nowseconds);
        terminal.print("Start Seconds: ");
        terminal.println(startsecondswd);
      if(nowseconds >= startsecondswd)
      {    
        terminal.print("ALL DAYS STARTED AT");
         String currentTime = String("Current Time: ") + String(Time.hourFormat12()) + ":" + String(Time.minute());
         terminal.println(currentTime); // current time in hours and minutes
        terminal.flush();
        if(nowseconds <= startsecondswd + 90)
        {    // 90s on 60s timer ensures 1 trigger command is sent
          // put code here to run relay
        }      
      }
      else
      {
        terminal.println("ALL DAY DEVICE NOT STARTED TODAY");
        terminal.flush();
      }
      stopsecondswd = (t.getStopHour() * 3600) + (t.getStopMinute() * 60);
      if(nowseconds >= stopsecondswd)
      {
        //digitalWrite(TestLED, LOW); // set LED OFF
        //Blynk.virtualWrite(V2, 0);
        terminal.print("All DAY STOPPED at");
        terminal.println(t.getStopHour() + ":" + t.getStopMinute());
        terminal.flush();
        if(nowseconds <= stopsecondswd + 90)
        {   // 90s on 60s timer ensures 1 trigger command is sent
            // code here to switch the relay OFF
        }              
      }
      else
      {
        if(nowseconds >= startsecondswd)
        {  
          //digitalWrite(TestLED, HIGH); // set LED ON  TEST!!!!!
          //Blynk.virtualWrite(V2, 1);
          terminal.println("All day is ON");
          terminal.flush();
 
        }           
      }
    }
    else
    {
      terminal.println("All day INACTIVE today");
      terminal.flush();
      // nothing to do today, check again in 30 SECONDS time    
    }
    terminal.println();
  }
}

void batteryV(){ 
  // custom battery function
    double voltage = analogRead(BATT)*0.0011224;
    String BV = String(voltage,2) + String("V");
    Particle.publish("Battery Voltage", BV, PRIVATE);
    int SoC = map(voltage, 3.20, 4.14, 0.00, 100.00);
    Blynk.virtualWrite(V4, SoC);
}

void currentTime(){ 
  // function to get current local time for debugging 
  int timezoneoffset = -6;
  Time.zone(timezoneoffset);
  String currentTime = String("Current Time: ") + String(Time.hourFormat12()) + ":" + String(Time.minute());
  terminal.print(currentTime); // current time in hours and minutes
  terminal.flush();
  int isAM = Time.isAM();
  if(isAM == 1){
    terminal.println("AM");
    terminal.flush();
  }
  else{
    terminal.println("PM");
    terminal.flush();
  }
  if(alldays == 1)
  {
  }
}
void currentDay(){ 
  // function to get current local day for debugging
  int currentweekday = Time.weekday();
  switch (currentweekday)
  {
    case 1:
    terminal.println("Sunday");
    terminal.flush();
    break;
    case 2:
    terminal.println("Monday");
    terminal.flush();
    break;
   case 3: 
    terminal.println("Tuesday");
    terminal.flush();
    break;
    case 4:
    terminal.println("Wednesday");
    terminal.flush();
    break;
    case 5:
    terminal.println("Thursday");
    terminal.flush();
    break;
    case 6:
    terminal.println("Friday");
    terminal.flush();
    break;
    case 7:
    terminal.println("Saturday");
    terminal.flush();
    break;
  }
}

void activetoday(){        // check if schedule should run today
  if(Time.year() != 1970)
  {
   if (alldays == 1) 
   { 
     Blynk.syncVirtual(V10); // sync timeinput widget  
   }
   
  }
}
void setup() {
 // Debug console
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);  
  pinMode(relay4, OUTPUT);
  delay(5000); // Allow board to settle
  Blynk.begin(auth);
  
  // Clear the terminal content
  terminal.clear();

  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device has Booted"));
  currentTime();
  currentDay();
  terminal.flush();

  timer.setInterval(10000L, activetoday);  // check every 10 SECONDS if schedule should run today 
}

void loop() {
  Blynk.run();
  timer.run();
  // update in 120 sec intervals
  if ((millis() - batterylastmillis) > 120000) 
  {
      batterylastmillis = millis();
      batteryV();
  }
  // update in 60 sec intervals
  if ((millis() - currentTimelastmillis) > 60000) 
  {
      currentTimelastmillis = millis();
      currentTime();
  }
  // update in 30 min intervals
  if ((millis() - currentDaylastmillis) > 1800000) 
  {
      currentDaylastmillis = millis();
      currentDay();
  }
  
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}