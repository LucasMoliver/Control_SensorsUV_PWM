#include "BluetoothSerial.h"
#include "ML8511.h"
// DEFINIÇÕES
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

int UVOUT[6] = {15, 0, 16, 5, 19, 22};
int REF_3V3[6] = {2, 4, 17, 18, 21, 23};

//Pinout: Primeiro Sensor UV

//UVOUT[0] = 15;
//REF_3V3[0] = 2;

//Pinout: Segundo Sensor UV

//UVOUT[1] = 0;
//REF_3V3[1] = 4;

//Pinout: Terceiro Sensor UV

//UVOUT[2] = 16;
//REF_3V3[2] = 17;

//Pinout: Quarto Sensor UV

//UVOUT[3] = 5;
//REF_3V3[3] = 18; 

//Pinout: Quinto Sensor UV

//UVOUT[4] = 19;
//REF_3V3[4] = 21;

//Pinout: Sexto Sensor UV

//UVOUT[5] = 22;
//REF_3V3[5] = 23;

//Pinout: LedUV

uint8_t LedUV = 5;

//Pinout: PWM

uint8_t Up_PWM = 14;
uint8_t Down_PWM = 27;

//Pinout: Contador

uint8_t Select_sensor = 12;

//Variaveis: sensor

uint8_t count = 0;
int uvLevel;
int refLevel;


//Intensidade PWM (0 a 255)

uint16_t Value_Intensity_PWM = 0;

//Antidebounce

unsigned long lastDebounceTime = 0; // última vez que o botão foi pressionado
unsigned long debounceDelay = 70;   // O intervalo, igual ao delay do código anterior
int countClicks = 0; // Contador

float UV;

ML8511 light_0(UVOUT[0], REF_3V3[0]);
ML8511 light_1(UVOUT[1], REF_3V3[1]);
ML8511 light_2(UVOUT[2], REF_3V3[2]);
ML8511 light_3(UVOUT[3], REF_3V3[3]);
ML8511 light_4(UVOUT[4], REF_3V3[4]);
ML8511 light_5(UVOUT[5], REF_3V3[5]);



void setup()

{

  pinMode(LedUV, OUTPUT);

  pinMode(UVOUT[0], INPUT);
  pinMode(UVOUT[1], INPUT);
  pinMode(UVOUT[2], INPUT);
  pinMode(UVOUT[3], INPUT);
  pinMode(UVOUT[4], INPUT);
  pinMode(UVOUT[5], INPUT);

  pinMode(REF_3V3[0], INPUT);
  pinMode(REF_3V3[1], INPUT);
  pinMode(REF_3V3[2], INPUT);
  pinMode(REF_3V3[3], INPUT);
  pinMode(REF_3V3[4], INPUT);
  pinMode(REF_3V3[5], INPUT);

  pinMode(Select_sensor, INPUT_PULLUP);

  Serial.begin(57600);

  Serial.println("The device started, now you can pair it with bluetooth!"); // Testa a conexão bluetooth

  light_0.setVoltsPerStep(3.3, 4095);
  light_1.setVoltsPerStep(3.3, 4095);
  light_2.setVoltsPerStep(3.3, 4095);
  light_3.setVoltsPerStep(3.3, 4095);
  light_4.setVoltsPerStep(3.3, 4095);
  light_5.setVoltsPerStep(3.3, 4095);

}

void loop()
{

  SetCount(count);

  SetLevel(count);

  analogWrite(LedUV, Value_Intensity_PWM);
  
  CheckVal(Up_PWM, Down_PWM);

  switch (count) {

    case '0':
    UV = light_0.getUV();
    break;

    case '1':
    UV = light_1.getUV();
    break;

    case '2':
    UV = light_2.getUV();
    break;

    case '3':
    UV = light_3.getUV();
    break;

    case '4':
    UV = light_4.getUV();
    break;

    case '5':
    UV = light_5.getUV();
    break;

    default:
    Serial.println("Erro!");
    break;

  }

  Serial.print(UV, 4);
  Serial.println(" mW cm^2");

  Serial.println();
  
  delay(50);

}

void CheckVal (uint8_t Up_PWM, uint8_t Down_PWM) {

  if (digitalRead(Up_PWM) == 1 && Value_Intensity_PWM < 4070) {
     if ((millis() - lastDebounceTime) > debounceDelay) {
       countClicks++;
     }
       lastDebounceTime = millis();

    if (countClicks >= 10) {
      Value_Intensity_PWM = Value_Intensity_PWM + 200;
  } 

  }

  if (digitalRead(Down_PWM) == 1 && Value_Intensity_PWM > 200) {
     if ((millis() - lastDebounceTime) > debounceDelay) {
       countClicks++;
     }
       lastDebounceTime = millis();

    if (countClicks >= 10) {
      Value_Intensity_PWM = Value_Intensity_PWM - 200;
  } 

  }

  if(Value_Intensity_PWM > 4095) {

   Value_Intensity_PWM = 0;

  }

}

void SetLevel (uint8_t x) {

  uvLevel = analogRead(UVOUT[x]);
  refLevel = analogRead(REF_3V3[x]);

}

void SetCount (uint8_t count) {

if (digitalRead(Select_sensor) == 1) {
     if ((millis() - lastDebounceTime) > debounceDelay) {
       countClicks++;
     }
       lastDebounceTime = millis();

    if (countClicks >= 10) {
      count = count + 1;
  } 

  }

if (count > 5) {

  count = 0;

}

}

