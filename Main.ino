// v1.0
// Left and right are in line with the direction of the head.

#define MOTION_PIN              2

#define MICRO_SWITCH_LEFT_PIN   3
#define MICRO_SWITCH_RIGHT_PIN  4

#define AC_MOTOR_LEFT_PIN       8 // Anti-Clockwise
#define AC_MOTOR_RIGHT_PIN      9 // Clockwise

char state; // r=right, l=left, c=center
char targetState;

bool isMoving = false;
unsigned char moveCount = 0; // 0 to 255

unsigned int centerMoveTime = 5000; // ms

bool microSwitchesActive = false;

void setup() {
  pinMode(MOTION_PIN, INPUT);
  pinMode(MICRO_SWITCH_LEFT_PIN, INPUT);
  pinMode(MICRO_SWITCH_RIGHT_PIN, INPUT);

  pinMode(AC_MOTOR_LEFT_PIN, OUTPUT);
  pinMode(AC_MOTOR_RIGHT_PIN, OUTPUT);

  digitalWrite(AC_MOTOR_LEFT_PIN, HIGH);
  digitalWrite(AC_MOTOR_RIGHT_PIN, HIGH);

  if(getMicroSwitchesState() != 'o')
    microSwitchesActive = true;

  // Test
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  // Test
  if(motionDetect())
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);

  if(getMicroSwitchesState() != 'o' && !microSwitchesActive){ // MicroSwitches is on
    moveFinish();
    microSwitchesActive = true;
  }
  else if(getMicroSwitchesState() == 'o')
    microSwitchesActive = false;

  if(motionDetect() && moveAllow())
	  move();
}

bool moveAllow() {
  bool result = true;
  // TODO: check count of move
  return !isMoving && result;
}

bool motionDetect() {
  return digitalRead(MOTION_PIN) == HIGH;
}

void move(){
  isMoving = true;
  moveCount++;
  
  if(state == NULL){
    char microSwitchesSatate = getMicroSwitchesState();
    if(microSwitchesSatate == 'o')
    {
      if(randBool())
        targetState = 'l';
      else 
        targetState = 'r';      
    }
    else{
      state = microSwitchesSatate;
      targetState = getRandomState(state);
    }
  }
  else
	  targetState = getRandomState(state);
  
  // Test
  Serial.println(targetState);

  switch (targetState) {
	case 'c':
	  if(state == 'r')
		acMotorMove('l');
	  else if(state == 'l')
		acMotorMove('r');

	  delay(centerMoveTime);

	  moveFinish();
	  
	  break;

	default:
	  acMotorMove(targetState); // For left and right
  }
}

void acMotorMove(char move){ // r=right, l=left, o=off
  switch (move) {
	case 'r':
	  digitalWrite(AC_MOTOR_RIGHT_PIN, LOW);
	  digitalWrite(AC_MOTOR_LEFT_PIN, HIGH); // just for case
	  break;
	case 'l':
	  digitalWrite(AC_MOTOR_LEFT_PIN, LOW);
	  digitalWrite(AC_MOTOR_RIGHT_PIN, HIGH); // just for case
	  break;
	case 'o':
	  digitalWrite(AC_MOTOR_LEFT_PIN, HIGH);
	  digitalWrite(AC_MOTOR_RIGHT_PIN, HIGH);
	  break;
  }
}

char getMicroSwitchesState(){ 
  if(digitalRead(MICRO_SWITCH_LEFT_PIN) == HIGH)
	  return 'l';
  if(digitalRead(MICRO_SWITCH_RIGHT_PIN) == HIGH)
	  return 'r';
  else
    return 'o';
}

void moveFinish(){
  //Test
  Serial.println("moveFinish()");

  acMotorMove('o');
  state = targetState;
  delay(2000);
  isMoving = false;
}

char getRandomState(char stateParam){
  switch (stateParam) {
	case 'r':
	  if(randBool())
		return 'l';
	  else 
		return 'c';
	  break;
	case 'l':
	  if(randBool())
		return 'r';
	  else 
		return 'c';
	  break;
	case 'c':
	  if(randBool())
		return 'l';
	  else 
		return 'r';
	  break;
  }  
  
}

int randNumber(int min, int max){
  return (min + (rand() % max)); // not return max self
}

bool randBool(){
  return randNumber(0, 2)%2 == 0;
}