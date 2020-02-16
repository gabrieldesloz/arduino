

// time count variables =============
int timeCounter = 0;
int previousTimeCount = 0;
int interval = 50;  
int WAIT_ON = 25;
int WAIT_OFF = 25;
// ==================================


void setup()  {

// initialize digital pin LED_BUILTIN as an output.
pinMode(LED_BUILTIN, OUTPUT);


  
}
void loop() {
  
  
  // every loop
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(WAIT_ON);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(WAIT_OFF);                       // wait for a second
  
  
  // time control 
  timeCounter = micros();   // ou timeCounter = millis();


  if (timeCounter - previousTimeCount >= interval) {
    previousTimeCount = timeCounter; 	
  }
  
  
}

