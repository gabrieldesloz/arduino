//#define LEDPIN 11         //LED brightness (PWM) writing
#define LIGHTSENSORPIN A6 //Ambient light sensor reading 
#define PWM_OUT_PIN 7	//PWM PIN OSCILOSCOPIO
#define SERIAL_RATE  500 // em ms


/// VARIAVEL DE CONTROLE DO TEMPO
long previousMillis = 0;            // Variável de controle do tempo


void setup() {
	
  // INICIALIZA PINOS	
  pinMode(LIGHTSENSORPIN,  INPUT);  
  //pinMode(LEDPIN, OUTPUT);  
  pinMode(LED_BUILTIN, OUTPUT);	
  
  // INICIALIZA SERIAL
  Serial.begin(9600);
}





void loop() {

 unsigned long currentMillis = millis();   			// controle do tempo de execução 
  
  float reading = analogRead(LIGHTSENSORPIN); 		//Read light level
  float square_ratio = reading / 1023.0;     	 	//Get percent of maximum value (1023)
  square_ratio = pow(square_ratio, 2.0);      		//Square to make response more obvious

  //analogWrite(LEDPIN, 255.0 * square_ratio);  	//Adjust LED brightness relatively
  analogWrite(LED_BUILTIN, 255.0 * square_ratio);   //Adjust LED brightness relatively  
  analogWrite(PWM_OUT_PIN, 255.0 * square_ratio);   //VISUALIZAÇÃO NO OSCILOSCOPIO 
  
    

  
  if (currentMillis - previousMillis > SERIAL_RATE)
  
  {
	
	previousMillis = currentMillis;	
	Serial.println("Float RAW: ");  Serial.println(reading);                    		//Display reading in serial monitor
	Serial.println(square_ratio);                    		
	Serial.println(255.0*square_ratio);
  } 
    

}
