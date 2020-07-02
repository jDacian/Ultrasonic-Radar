#include <Servo.h> 
#include <NewPing.h>

#define TRIGGER_PIN   10 
#define ECHO_PIN      11 
#define MAX_DISTANCE  50 
#define SERVO_PIN     8 
#define BAUD_RATE     115200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

unsigned int pingSpeed = 10; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;

//long duration;
int distance;
int angle;                   // current servo position 
class Sweeper
{
  Servo servo;              // the servo
  int increment;            // increment to move for each interval
  int  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
 
public: 
  Sweeper(int interval)
  {
    updateInterval = interval;
    increment = 1;
  }
  
  void Attach(int pin)
  {
    servo.attach(pin);
  }
  
  void Detach()
  {
    servo.detach();
  }
  
  void Update()
  {
    if((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
      angle += increment;
      servo.write(angle);

      if ((angle >= 180) || (angle <= 0)) // end of sweep
      {
        // reverse direction
        increment = -increment;
      }
    }
  }
};
 
Sweeper sweeper1(10);

 
void setup() 
{ 
  Serial.begin(BAUD_RATE); // Open serial monitor at 115200 baud to see ping results.
  pingTimer = millis(); // Start now.
  sweeper1.Attach(SERVO_PIN);

} 
 
 
void loop() 
{ 
  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.
    sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }
  
  sweeper1.Update();

  printData();
}

void echoCheck() 
{
  if (sonar.check_timer()) { // This is how you check to see if the ping was received.

  distance = (sonar.ping_result / US_ROUNDTRIP_CM);
  
  }
}

void printData()
{
  // Sends Data to the Radar Application
  Serial.print(angle);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");

}
