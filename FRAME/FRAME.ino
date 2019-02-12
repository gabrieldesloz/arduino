
#define PULSE  10000 			// em us
#define DELAY  20000 			// em us
#define ADC_PERIOD 10000			// FREQUENCIA AMOSTRAGEM = 1/ADC_PERIOD(us)
#define OUT1 0 					// PINO OUTPUT DIGITAL	
#define OUT2 1 					// PINO OUTPUT DIGITAL	
#define PWM_OUT_PIN  7
#define SAMPLES 20				// NUMERO DE AMOSTRAS PARA MEDIA
//#define SENSOR_PIN  A1


// INCLUDES--------------------------------------------------------------------------------------------------------

#include <LiquidCrystal_I2C.h> 		//LCD	
#include <avr/wdt.h>				//watchdog	
#include <Adafruit_INA219.h>        // ina219 lib -- sensor de corrente
// ----------------------------------------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------------------------------------
// global variables INA219
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float current_AVG = 0;
float loadvoltage = 0;
int F_CORREC = 10; //fator de correção para a corrente, modo ina.219.setCalibration_32V_2A(); 
// ----------------------------------------------------------------------------------------------------------------



// variaveis ------------------------------------------------------------------------------------------------------
int setBit_startAvg = 0;
int setBit_startAvg_OFFSET = 0;
float averageSensor = 0;
float averageSensor_m_OFFSET = 0;
int sampCounter = 0;
float average = 0.0;	
float reading = 0.0;
float sum_AVG = 0.0;
float aDC_Voltage = 0.0;					// TENSÃO ADC
float SENSOR_PIN = A6;						// PINO SENSOR
float c_SENS_VOLT_offset = 0;  			// offset inicial sensores de corrente
// ----------------------------------------------------------------------------------------------------------------



// CONSTANTES ------------------------------------------------------------------------------------------------------
const float VEXT_REF = 5.03;   // tensão externa medida com multimetro CALIBRAR
const int LCD_addr = 0x27;     // LCD i2c address
const int LCD_chars = 16;      // number of characters
const int LCD_lines = 2;       // number of lines
const int INA_addr = 0x40;     // INA219 address
// ----------------------------------------------------------------------------------------------------------------
LiquidCrystal_I2C lcd(LCD_addr, LCD_chars, LCD_lines);
// ----------------------------------------------------------------------------------------------------------------



// funções --------------------------------------------------------------------------------------------------------
void Serial_Send(void);
void waveGen(void);
void LCD_Update(void);			///LCD
// ----------------------------------------------------------------------------------------------------------------


 // set the INA219 to address 0x40
// indica o endereço do INA219 - 0x40
Adafruit_INA219 ina219(INA_addr);




// VARIAVEL DE CONTROLE DO TEMPO -----------------------------------------------------------------------------------  														
// OUTRA OPÇÃO É USAR A BIBLIOTECA TIMEMARK

unsigned long currentMicros = 0;            											// Variável de controle do tempo
unsigned long previousMicros_FSM = 0;            										// Variável de controle do tempo
unsigned long previousMicros_ADC = 0;
//-------------------------------------------------------------------------------------------------------------------


// STATE MACHINE - MAQUINA DE ESTADOS -------------------------------------------------------------------------------
enum {
  OFFSET,
  ONE,
  TWO,
  THREE
};
int state = OFFSET;
//-------------------------------------------------------------------------------------------------------------------


// CONFIGURAÇÃO / INICIALIZAÇÃO -------------------------------------------------------------------------------------
void setup() {
 
 analogReference(EXTERNAL); 										// 3V3 EXTERNO //// IMORTANTE - importante
 for(int i=0; i<20;i++) analogRead(SENSOR_PIN); 					// ler o ADC na inicialização por causa dos valroes corrompidos
 
 

 ina219.begin();													 // initialize the INA219
  // modos de calibração - verificar biblioteca
  // ina.219.setCalibration_32V_2A();
  // ina219.setCalibration_32V_1A();
  // medir corrente "raw"   getCurrent_raw()  
  // ina219.setCalibration_16V_400mA() ;
 
 
 lcd.init();														// INICIA O LCD
 lcd.clear();
 
 
  wdt_enable(WDTO_2S);												//WATCHDOG, WDTO_8S, WDTO_4S, WDTO_1S, WDTO_500MS, WDTO_250MS,
	
 
  pinMode(OUT1,  OUTPUT);  
  pinMode(OUT2,  OUTPUT);  
 
//DDRD = B11111111;  												// sets Arduino pins 0 to 7 as outputs
 
 state = OFFSET;													// INICIALIZA MAQUINA DE ESTADOS	
 
 //Serial.begin(9600); 												// NECESSARIO DESABILITAR PARA OBTER UMA FORMA DE ONDA PRECISA
 }
// --------------------------------------------------------------------------------------------------------------------


 /// ----- MAIN LOOOP ------------------------------------------------------------------------------------------------  
 void loop()
{

	
	currentMicros = micros();   								// controle do tempo de execução      
	
	
	averageSensor = averageSens();
	
	waveGen();
	
	//aDC_Voltage = averageSensor_m_OFFSET;						// INA
	//aDC_Voltage = (averageSensor_m_OFFSET/1024.00)*VEXT_REF; 	// adc atmega
	
	LCD_Update();												// MANDA INFORMAÇÃO PRO LCD
		
	wdt_reset();												// RESET WATCHDOG	
	
		
	
}
/// ----- END LOOP ------------------------------------------------------------------------------------------------ 




// calcula a media -------------------------------------------------------------------------------------------------------
float averageSens(){	
	digitalWrite(OUT2, LOW);	
	if  (currentMicros - previousMicros_ADC > ADC_PERIOD-1) 
	{
	digitalWrite(OUT2, HIGH);								
	previousMicros_ADC = currentMicros;	
	reading = analogRead(SENSOR_PIN); 				// TEAKE MEASUREMENT FROM THE ATMEGA ADC
	//reading = ina219.getCurrent_mA()/F_CORREC;			// TAKE MEASURMENT 
	
	sum_AVG = sum_AVG + reading;	
		if (sampCounter == SAMPLES-1)
		{
			sampCounter = 0;
			average = sum_AVG/SAMPLES;			
			sum_AVG = 0;
		} else
		{
		sampCounter++;
		}
	
	}
	return average;
}


// envia os valores pela serial ///////////////////////////////
// void Serial_Send(void)
// {
  // Serial.print("Average:   ");      Serial.print(averageSensor); Serial.println(" V");
  // Serial.print("Samples:   ");      Serial.println(SAMPLES);   
  // Serial.println("");
// }


//-----------------------------------------------------------------------------------------------------------------------------
// gera foram de onda
void waveGen(void)
{
  
  switch(state)
		{
		  case OFFSET:     ///  CALC OFFSET
			
			if  (currentMicros - previousMicros_FSM > PULSE-1) 
				{
					//PORTD = B00000000;  // pino 0 BAIXO
					digitalWrite(OUT1, LOW);
					previousMicros_FSM = currentMicros;				
					state = ONE;
					break;
				} 
			else
				{
					c_SENS_VOLT_offset = averageSensor	;	
					digitalWrite(OUT1, HIGH);
					//PORTD = B00000111;  // pino 0 ALTO
					break;
				}				
			break; 
			
		  case ONE: /// DELAY
		 
			if  (currentMicros - previousMicros_FSM > DELAY-1) 
				{					
					previousMicros_FSM = currentMicros;
					state = TWO;
					break;
				} 
			else
				{					
					break;
				}	
				
			break; 
	  
			case TWO: ///  AVERAGE MINUS OFFSET
		 
			if  (currentMicros - previousMicros_FSM > PULSE-1) 
				{
					
					previousMicros_FSM = currentMicros;
					state = THREE;
					digitalWrite(OUT1, LOW);
					//PORTD = B00000000;  // pino 0 BAIXO
					break;
				} 
			else
				{	
					//PORTD = B00000111;  // pino 0 ALTO
					digitalWrite(OUT1, HIGH);
					averageSensor_m_OFFSET = averageSensor - c_SENS_VOLT_offset;					
					break;
					
				}	
				
			break; 
			
			
			
			case THREE: ///  DELAY
		 
			if  (currentMicros - previousMicros_FSM > DELAY-1) 
				{
					
					previousMicros_FSM = currentMicros;
					state = OFFSET;
					break;
				} 
			else
				{					
					break;
					
				}	
				
			break; 
	  
	  
	  
		  default:
			state = OFFSET;
		}	
  
}


// atualiza o LCD com os valores /////////////////////////////////////
void LCD_Update(void)
{
  
    // Print a message to the LCD.
  lcd.backlight(); 
  // set the cursor and write some text
  lcd.setCursor(0,0);
  lcd.print("T(V)   C(A)    "); 
  
  lcd.setCursor(0,1);
  lcd.print(averageSensor);  //lcd.print("V   ");
 
  lcd.setCursor(7,1);
  lcd.print(averageSensor); //lcd.print("mA   ");
}




/* 
void convert_Values(void)
{
  float square_ratio = reading / 1023.0;     	 	//Get percent of maximum value (1023)
  square_ratio = pow(square_ratio, 2.0);      		//Square to make response more obvious
}



void outputPinValues(void)
{
  analogWrite(LED_BUILTIN, 255.0 * square_ratio);   //Adjust LED brightness relatively  
  analogWrite(PWM_OUT_PIN, 255.0 * square_ratio);   //VISUALIZAÇÃO NO OSCILOSCOPIO 
} */

