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
    terminal.clear();
    terminal.println(F("Relay 4 has been switched on!"));
    terminal.flush();
    }
         else
        {
            // execute this code if the switch widget is now OFF
            digitalWrite(relay4, LOW);  // Set digital pin 2 LOW 
            terminal.println(F("Relay 4 has been switched off!"));
            terminal.flush();     
        }

}
BLYNK_WRITE(V6)
{
    if(param.asInt() == 1)
    {
    alldays = 1;
    weekdays = 0;
    weekend = 0;
    custom = 0;
    Blynk.syncVirtual(V10);
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

  // Process start time

  if (t.hasStartTime())
  {
    String StartTime = String("Start Time:") + String(t.getStartHour()) + ":" + String(t.getStartMinute());
    terminal.println(StartTime);
  }
  else if (t.isStartSunrise())
  {
    terminal.println("Start at sunrise");
  }
  else if (t.isStartSunset())
  {
    terminal.println("Start at sunset");
  }
  else
  {
    // Do nothing
  }

  // Process stop time

  if (t.hasStopTime())
  {
    String StopTime = String("Stop Time:") + String(t.getStopHour()) + ":" + String(t.getStopMinute());
    terminal.println(StopTime);
  }
  else if (t.isStopSunrise())
  {
    terminal.println("Stop at sunrise");
  }
  else if (t.isStopSunset())
  {
    terminal.println("Stop at sunset");
  }
  else
  {
    // Do nothing: no stop time was set
  }

  // Process timezone
  // Timezone is already added to start/stop time

  terminal.println(String("Time zone: ") + t.getTZ());

  // Get timezone offset (in seconds)
  terminal.println(String("Time zone offset: ") + t.getTZ_Offset());

  // Process weekdays (1. Mon, 2. Tue, 3. Wed, ...)

  for (int i = 1; i <= 7; i++) {
    if (t.isWeekdaySelected(i)) {
      String DaySelected = String(i) + String("is selected.");
      terminal.println(DaySelected);
    }
  }
  terminal.println("---------------------");
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