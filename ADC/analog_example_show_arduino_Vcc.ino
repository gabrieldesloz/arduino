

int valor_Sensor;
int pinoSensor = A7;  						//define o pino para o sensor
int ledPin=13; 								//define o pino para o LED, depende placa
long valor_Vcc = 0L;
float valor_Vcc_float = 0.0;



void setup() {

analogReference(DEFAULT); //5V



pinMode( ledPin, OUTPUT );  									// inicializa pino led como output //  pinMode(LED_BUILTIN, OUTPUT);
 
for(int i=0; i<8;i++) analogRead(pinoSensor); 					// ler o ADC na inicialização por causa dos valroes corrompidos

Serial.begin(9600);  // inicia comunicação serial  


}


void loop() {

valor_Sensor = analogRead(pinoSensor);							// valor sem conversao
valor_Vcc = readVcc();											// função que retorna valor Vcc
valor_Vcc_float = (float) readVcc()/1000; 						// mostrando na tela a tesnão no formato float,com correção (/1000)

//Serial.println(valor_Sensor);									// valor sem conversao
//Serial.println(valor_Vcc);										// valor VCc
Serial.println(valor_Vcc_float);										// valor VCc



analogWrite(ledPin, analogRead(pinoSensor)/4); 					//	Envia o Valor de Leitura para o Pino do LED

delay(10); 													    //	Tempo curto para acionamento do LED

}




// como exemplo em **provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
     ADMUX = _BV(MUX5) | _BV(MUX0) ;
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
