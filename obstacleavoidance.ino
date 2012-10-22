boolean debug = false;

// setup ultrasonic distance sensor
const int pingPin = 2;
unsigned int duration, distance;
int sensorWait = 10;

// setup servo under sensor
#include <Servo.h> 
Servo sensingServo;
int pos = 1300;
int sensingServoMin = 800;
int sensingServoMax = 1700;
int sensingServoResolution = 25;
boolean sweepLeft = true;

// setup array for storing directions and distances
// array length = ((sensingServoMax - sensingServoMin) / sensingServoResolution) + 1;
int directionsAndDistances[37][2];
int directionsAndDistancesLength = 37;
int i;

// setup arrray for choosing direction
int obstacleFreeDirections[37][3];
int minimumDistance = 48;
int waypointDirection = 1400;

void readSensor()
{
  // move servo into position
  sensingServo.writeMicroseconds(pos);
  
  // read ultrasonic sensor
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  distance = duration / 74 / 2; 

  // write position and distance to array
  directionsAndDistances[i][0] = pos;
  directionsAndDistances[i][1] = distance;  
}

int chooseDirection()
{
  if(debug)
  {
    Serial.println("Directions and distances");
  
    for (int i = 0; i < directionsAndDistancesLength; i++)
    {
    Serial.print(directionsAndDistances[i][0]);
    Serial.print(",");
    Serial.print(directionsAndDistances[i][1]);
    Serial.print("\n");      
    }
  }  
  
  // filter out directions with obstacles
  for(int i,j = 0; i < directionsAndDistancesLength; i++)
  {
    if (directionsAndDistances[i][1] >= minimumDistance)
    { 
    obstacleFreeDirections[j][0] = directionsAndDistances[i][0];
    obstacleFreeDirections[j][1] = directionsAndDistances[i][1];
    }
    else
    {
      obstacleFreeDirections[j][0] = 0;
      obstacleFreeDirections[j][1] = 0;     
    }
    obstacleFreeDirections[j][2] = 0;    
    j++;
  }
  
  if(debug)
  {
    Serial.println("Obstacle free directions");
    
    for (int i = 0; i < directionsAndDistancesLength; i++)
    {
      Serial.print(obstacleFreeDirections[i][0]);
      Serial.print(",");
      Serial.print(obstacleFreeDirections[i][1]);
      Serial.print(",");
      Serial.print(obstacleFreeDirections[i][2]);    
      Serial.print("\n");               
    }    
  }
 
  // calculate the discrepancy between the waypoint direction and each direction
  for(int i = 0; i < directionsAndDistancesLength; i++)
  {
    if(obstacleFreeDirections[i][0] != 0)
    {
    obstacleFreeDirections[i][2] = abs(waypointDirection - obstacleFreeDirections[i][0]);
    }
  }
  
  if(debug)
  {
    Serial.println("Discrepancies from way point");
  
    for (int i = 0; i < directionsAndDistancesLength; i++)
    {
    Serial.print(obstacleFreeDirections[i][0]);
    Serial.print(",");
    Serial.print(obstacleFreeDirections[i][1]);
    Serial.print(",");    
    Serial.print(obstacleFreeDirections[i][2]);    
    Serial.print("\n");
    }
  }
  
  // choose the direction closest to the waypoint direction with no obstacles

  // find the first obstacle free direction
  int solutionDirectionReference = 0; 
  while(obstacleFreeDirections[solutionDirectionReference][0] == 0)
  {
    solutionDirectionReference++;
  }
  
  // find the obstacle free direction closest to the waypoint direction
  for(int i = 0; i < directionsAndDistancesLength; i++)
  {
    if(obstacleFreeDirections[i][0] != 0 && obstacleFreeDirections[i][2] < obstacleFreeDirections[solutionDirectionReference][2])
    {
      solutionDirectionReference = i;
    }
  }

  if(obstacleFreeDirections[solutionDirectionReference][0] > 15000)
  {
    return 0;
  }
  else
  {
    return obstacleFreeDirections[solutionDirectionReference][0];
  }
}

void setup() 
{
  Serial.begin(115200);
  sensingServo.attach(5);  
}

void loop() { 
  if (sweepLeft)
  {
    // sweep left
    i = 0;
    for(pos = sensingServoMin; pos <= sensingServoMax; pos += sensingServoResolution)
    {
      readSensor();
      i++;
      delay(sensorWait);
    }
    sweepLeft = false;
  }
  
  else
  {
    // sweep right
    i = directionsAndDistancesLength - 1;

    for(pos = sensingServoMax; pos >= sensingServoMin; pos -= sensingServoResolution)
    {
      readSensor();
      i--;
      delay(sensorWait);
    }
    sweepLeft = true;
  }
  
  int chosenDirection = chooseDirection();
//  Serial.print("Direction chosen:  ");
  Serial.print(chosenDirection);
  Serial.print("\n");    
}