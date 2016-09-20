#include <Servo.h>
#include <NewPing.h>


#define TRIG_PIN   3
#define ECHO_PIN   4
#define MAX_DIST  50
#define VEL       10

//Continuous Servos: self.write defines ang. velocity
Servo RodaE;
Servo RodaD;
//Micro Servos: self.write defines the angle
Servo BaseSonar;
Servo RodaF;

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DIST);

void setup()
{
    RodaE.attach(9);
    RodaD.attach(10);
    BaseSonar.attach(11);
    RodaF.attach(12);
}

void loop()
{

    int uS = sonar.ping();
    int DS = uS / US_ROUNDTRIP_CM;
    if (DS > 5 || DS == 0)
    {
      //Walk foward indefinitedly
      RodaE.write(180 - VEL);
      RodaD.write(VEL);
    }
    else
    {
      //Stop the robot
      RodaE.write(90);
      RodaD.write(90);

      int BestAngle = 0;
      int BestDist = 0;
      //Linear search in robot FOV (0 - 180)
      for (int i = 0; i <= 180; i++)
      {
        //Set 'i' as the angle of the sonar servo
        BaseSonar.write(i);
        //Check the distance
        int uS = sonar.ping();
        int DS = uS / US_ROUNDTRIP_CM;
        delay(10);
        //If the distance DS is greater than the previous best distance...
        if (DS > BestDist)
        {
          //then DS is the new best distance
          BestDist = DS;
          //and 'i' is the new best angle
          BestAngle = i;
        }
      }
      //Set the sonar servo to the default position
      BaseSonar.write(90);
      //Move robot backwards
      RodaE.write(0);
      RodaD.write(180);
      // Wait 1s to finish the movement
      delay(1000);
      
      //If the best angle found is in the Right
      if (BestAngle > 90)
      {
        //Turn the directional wheel pi/4 to the Right
        RodaF.write(45);
        //Left wheel foward
        RodaE.write(180);
        //Right wheel stops
        RodaD.write(90);
      }
      //If the best angle found is in the Left
      else if (BestAngle <= 90)
      {
        //Turn the directional wheel pi/4 to the Left
        RodaF.write(180 - 45);
        //Left wheel stops
        RodaE.write(90);
        //Right wheel foward
        RodaD.write(0);
      }
      //TODO: Time is related to the angle...
      delay(1000);
      //Turn the directional wheel fowards
      RodaF.write(90);
    }
  }
}
