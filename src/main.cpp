#include <Arduino.h>
#include <Button.h>
/*  Программа sketch_7_2 урока 7 
 *  Подключены две кнопки и светодиод
 *  Каждое нажатие кнопки 1 инвертирует состояние светодиода на плате Ардуино
 *  Каждое нажатие кнопки 2 инвертирует состояние светодиода на макетной плате */

#define LED_1_PIN 13     // номер вывода светодиода 1 равен 13
#define BUTTON_1_PIN 12  // номер вывода кнопки 1 равен 12
#define BUTTON_2_PIN 11  // номер вывода кнопки 2 равен 11
#define LED_2_PIN 10     // номер вывода светодиода 2 равен 10

boolean ledState1;         // переменная состояния светодиода 1
boolean ledState2;         // переменная состояния светодиода 2

Button button1(BUTTON_1_PIN, 15);  // создание объекта для кнопки 1
Button button2(BUTTON_2_PIN, 15);  // создание объекта для кнопки 2
   
void setup() {
    Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(LED_1_PIN, OUTPUT);           // определяем вывод светодиода 1 как выход
  pinMode(LED_2_PIN, OUTPUT);           // определяем вывод светодиода 2 как выход
}

// бесконечный цикл с периодом 2 мс
void loop() {

  button1.filterAvarage(); // вызов метода сканирования сигнала кнопки 1
  button2.scanState();  // вызов метода сканирования сигнала кнопки 2
  
  // блок управления светодиодом 1
  if ( button1.flagClick == true ) {
    // было нажатие кнопки
    Serial.println("cliked 1");
    button1.flagClick= false;         // сброс признака клика
    ledState1= ! ledState1;             // инверсия состояния светодиода 1
    digitalWrite(LED_1_PIN, ledState1);  // вывод состояния светодиода 1    
  }

  // блок управления светодиодом 2
  if ( button2.flagClick == true ) {
    // было нажатие кнопки
    Serial.println("cliked 2");
    button2.flagClick= false;         // сброс признака клика
    ledState2= ! ledState2;             // инверсия состояние светодиода 2
    digitalWrite(LED_2_PIN, ledState2);  // вывод состояния светодиода 2    
  }

  delay(2);  // задержка на 2 мс
}
