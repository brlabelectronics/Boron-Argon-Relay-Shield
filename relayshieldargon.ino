

#define BLYNK_TEMPLATE_ID "TMPLGYDWlg-c"
#define BLYNK_DEVICE_NAME "Relay Shield Argon"
#define BLYNK_AUTH_TOKEN "C6Z6LgRQGOnbgXIhwlMKnFSmnjDbuaHE"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// This #include statement was automatically added by the Particle IDE.
#include <blynk.h>


char auth[] = BLYNK_AUTH_TOKEN;

// Attach virtual serial terminal to Virtual Pin V?
WidgetTerminal terminal(V7);

const int relay1 = 5;
const int relay2 = 6;
const int relay3 = 7;
const int relay4 = 8;
float voltage = 0;
unsigned long lastmillis = 0;

int alldays = 0;
BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
    if(param.asInt() == 1)
    {
    // execute this code if the switch widget is now ON
    digitalWrite(relay1, HIGH);  // Set digital pin 2 HIGH
    terminal.println(F("Relay 1 has been switched on!"));
    terminal.println(F("------------------------"));
    terminal.flush();
    }
         else
        {
            // execute this code if the switch widget is now OFF
            digitalWrite(relay1, LOW);  // Set digital pin 2 LOW
            terminal.println(F("Relay 1 has been switched off!"));
            terminal.println(F("------------------------"));
            terminal.flush();
        }

}
BLYNK_WRITE(V1)
{
    if(param.asInt() == 1)
    {
    // execute this code if the switch widget is now ON
    digitalWrite(relay2, HIGH);  // Set digital pin 2 HIGH
    }
         else
        {
            // execute this code if the switch widget is now OFF
            digitalWrite(relay2, LOW);  // Set digital pin 2 LOW    
        }

}
BLYNK_WRITE(V2)
{
    if(param.asInt() == 1)
    {
    // execute this code if the switch widget is now ON
    digitalWrite(relay3, HIGH);  // Set digital pin 2 HIGH
    }
         else
        {
            // execute this code if the switch widget is now OFF
            digitalWrite(relay3, LOW);  // Set digital pin 2 LOW    
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

BLYNK_WRITE(V5)
{
    if(param.asInt() == 1)
    {
    // execute this code if the switch widget is now ON
    alldays == 1;
    terminal.clear();
    Blynk.virtualWrite(V6, 1);
    }
    else 
    {
    alldays=0;
    }
}

BLYNK_WRITE(V6) {
  if(alldays == 1)
  {
  TimeInputParam t(param);

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
    double voltage = analogRead(BATT)*0.0011224;
    String BV = String(voltage,2) + String("V");
    Particle.publish("Battery Voltage", BV, PRIVATE);
    int SoC = map(voltage, 0.00, 4.15, 0.00, 100.00);
    Blynk.virtualWrite(V4, SoC);
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
  terminal.println(F("------------------------"));
  terminal.flush();
}

void loop() {
 Blynk.run();
  timer.run();
  // update in 2 sec intervals
            if ((millis() - lastmillis) > 120000) {
                lastmillis = millis();
                batteryV();
            }

  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}