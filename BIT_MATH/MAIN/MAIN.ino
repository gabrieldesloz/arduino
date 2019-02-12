http://playground.arduino.cc/Code/BitMath#introduction

/// REFERENCIA RAPIDA ============================
   	if ((PORTE & 1) == 1){ .... // se o sinal do bit 0 da porta E for igual a 1

	y = (x >> n) & 1;    // n=0..15.  stores nth bit of x in y.  y becomes 0 or 1.
    x &= ~(1 << n);      // forces nth bit of x to be 0.  all other bits left alone.
    x &= (1<<(n+1))-1;   // leaves alone the lowest n bits of x; all higher bits set to 0.
    x |= (1 << n);       // forces nth bit of x to be 1.  all other bits left alone.
    x ^= (1 << n);       // toggles nth bit of x.  all other bits left alone.
    x = ~x;              // toggles ALL the bits in x.
/// REFERENCIA RAPIDA ============================

	

// Utilidades:
// salvar memoria armazenando valores logicos TRUE/ FALSE EM um byte
// ligando/desligando bits individuais em um registrador
// executar algumas operações aritmeticas envolvendo multiplicações e divisões


// definição de bases de numeros
// 0b -> prefixo

// 0b11 // = "3"
// B00000011  // = "3" 'legado'


// & 
// AND
// sendo int = tipo de dados com valores de 16 bits

// 0 & 0 == 0
// 0 & 1 == 0
// 1 & 0 == 0
// 1 & 1 == 1


// int a =  92;    // in binary: 0000000001011100
// int b = 101;    // in binary: 0000000001100101
// int c = a & b;  // result:    0000000001000100, or 68 in decimal.


// USO ---> MASKING --> ACESSAR APENAS ALGUM BIT DE UM VETOR, RETORNANDO COMO RESPOSTA APENAS ESTES BITS - usando bit 1, resto zero
// USO --> SE ASSEGURAR QUE UM bit ESPECIFICO SERÁ DESLIGADO - usando bit zero, resto 1
// int x = 5;       // binary: 101
// int y = x & 1;   // now y == 1
// x = 4;           // binary: 100
// y = x & 1;       // now y == 



// -------------------------------------------------
// | 
// OR

// 0 | 0 == 0
// 0 | 1 == 1
// 1 | 0 == 1
// 1 | 1 == 1

// USO --> SE ASSEGURAR QUE UM BIT ESPECIFICO SERÁ LIGADO, SEM INTERFERIR NOS DEMAIS BITS DO VETOR, usando 1
 // int a =  92;    // in binary: 0000000001011100
 // int b = 101;    // in binary: 0000000001100101
 // int c = a | b;  // result:    0000000001111101, or 125 in decimal.

 
 
// -------------------------------------------------
 // ^
 // Bitwise XOR
 
    // 0 ^ 0 == 0
    // 0 ^ 1 == 1
    // 1 ^ 0 == 1
    // 1 ^ 1 == 0
 
 // USO --> TOGGLE, SWITH, TROCAR UM BIT  se x ^ 1, ou x ^ 101 etc
 // SE TODOS OS BITS DA OPERAÇÃO SAO IGUAIS, O RESULTADO É 			ZERO
 // SE TODOS OS BITS DA OPERAÇÃO SÃO DIFERENTES, O RESULTADO É 		UM
 

// ------------------------------------------------- 
// ~ 
// NOT
 
 // USO INVERTER OS BITS
 // USO TRASNFORMA EM UM NUMERO NEGATIVO, BIT MAIS SIGNIFICATIVO DE INT É O DO SINAL
 // CODIFICAÇÃO DE NUMEROS NEGATIVOS E POSITIVOS É CHAMADA DE COMPLEMENTO DE 2
 /// IMPORTANTE ->´PARA CADA NÚMERO X, ~X É IGUAL -X-1
 
  
// int a = 103;    // binary:  0000000001100111
// int b = ~a;     // binary:  1111111110011000 = -104
 
 
 
 // ------------------------------------------------- 
// << ou >> 
// Bit Shift Operators
 // shifta ou desloca o operador da esquerda pelo numero de posições
 // CUIDAR COM O TAMANHO DO VETOR, QUANDO SE DESLOCA PARA A ESQUERDA, E COM A PERDA DOS BITS MENOS SIGNIFICATIVOS,
 // QUANDO SE DESLOCA OS BITS PARA A DIREITA
 // multilpicação ou divisão por 2, 4, 8 , 16 etc
 
 
// int a = 5;        // binary: 0000000000000101
// int b = a << 3;   // binary: 0000000000101000, or 40 in decimal
// int c = b >> 3;   // binary: 0000000000000101, or back to 5 like we started with
 
 // GERANDO POTENCIAS DE 2
 
1 <<  0  ==    1
1 <<  1  ==    2
1 <<  2  ==    4
1 <<  3  ==    8
...
1 <<  8  ==  256
1 <<  9  ==  512
1 << 10  == 1024
 
 // COPIA DO SINAL QUANDO NUMERO NEGATIVO E INT - SIGN EXTENSION
 int x = -16;     // binary: 1111111111110000
 int y = x >> 3;  // binary: 1111111111111110
 
 // UNSIGNED INT - SEM A COPIUA DO BIT MAIS SIGNIFICATIVO
 int x = -16;               // binary: 1111111111110000
 int y = unsigned(x) >> 3;  // binary: 0001111111111110


 // ------------------------------------------------- 
// operadores de assinalamento (assignment operators)


x = x + 7;    // increase x by 7
x += 7;      // increase x by 7
x = x - 7;    // decrease x by 7
x -= 7;      // decrease x by 7


int x = 1;  // binary: 0000000000000001
x <<= 3;    // binary: 0000000000001000
x |= 3;     // binary: 0000000000001011 - because 3 is 11 in binary
x &= 1;     // binary: 0000000000000001
x ^= 4;     // binary: 0000000000000101 - toggle using binary mask 100
x ^= 4;     // binary: 0000000000000001 - toggle with mask 100 again
 
x = ~x;    // toggle all bits in x and store back in x

 
#define OUT1 0 					// PINO OUTPUT DIGITAL	

const int ledPin = LED_BUILTIN;     
int ledState = 	LOW;             
unsigned long previousTimeCount = 0;        
const long interval = 10000;           
unsigned long timeCounter = 0;


void setup() {
 // pinMode(ledPin, OUTPUT);
 // Serial.begin(9600);
  
  pinMode(OUT1,  OUTPUT); 
	
	
/// PWMS E PREESCALERS	
/// alterações no preescaler dos PWMs--------------------------------------------------------
  // Serial.println(TCCR2B, BIN); // MOSTRA o valor do registrador que controla o pwm preescaler do pino 10  //  verificar diagrama esquematico
  // TCCR2B = (TCCR2B & B11111000) | B00000001; // modifica o preescaler do pwm
/// alterações no preescaler dos PWMs--------------------------------------------------------



  
}
void loop() {
  
  timeCounter = micros();
  
  if (timeCounter - previousTimeCount >= interval) {
    previousTimeCount = timeCounter;
    
    if (ledState == LOW) {
      ledState = HIGH;
	  PORTE = B00000111; 
    } else {
	  PORTE = B00000000;
      ledState = LOW;
    }
    
	
	// VELOCIDADE DE PORTA NORMAL
    // digitalWrite(ledPin, ledState);
    //digitalWrite(OUT1, HIGH); -- 73kHz
    //digitalWrite(OUT1, LOW);
	
	
	/// manipulação da porta através de registradores----------
	// PORTE = B00000101;  // 1MHz
	// delayMicroseconds(11);
	// PORTE = B00000000;
	// delayMicroseconds(11);
	///--------------------------------------------------------

	
	//-- clock / preescaler configurado pelo registrador TCCR1B
	//analogWrite(10, 0.5*255); //490 Hz
	
  }
}