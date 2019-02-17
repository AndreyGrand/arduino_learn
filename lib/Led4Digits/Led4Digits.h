/*
Led4Digits.h - библиотека управления 4х разрядным семисегментным светодиодным индикатором

Подробно описана в Уроке 20.
http://mypractic.ru/uroki-programmirovaniya-arduino-navigaciya-po-urokam

Библиотека разработана Калининым Эдуардом
mypractic.ru 
*/

// проверка, что библиотека еще не подключена
#ifndef Led4Digits_h // если библиотека Led4Digits не подключена
#define Led4Digits_h // тогда подключаем ее

#include "Arduino.h"

class Led4Digits {
  public:
    byte digit[4];  // коды управления сегментами разрядов
    void regen(); // регенерация, метод должен вызываться регулярно
    void tetradToSegCod(byte dig, byte tetrad);  // преобразования тетрады в коды сегментов 
    boolean print(unsigned int value, byte digitNum, byte blank); // вывод целого числа 

    // конструктор
    // typeLed - тип управления, определяет активные состояния сигналов управления
    // 0 - выбор разряда -_-, выбор сегмента -_-
    // 1 - выбор разряда _-_, выбор сегмента -_-
    // 2 - выбор разряда -_-, выбор сегмента _-_
    // 3 - выбор разряда _-_, выбор сегмента _-_
    // digitPin0 ... digitPin3 - выводы выбора разрядов, = 255 - разряд отключен
    // segPinA ... segPinH - выводы выбора сегментов          
    Led4Digits (byte typeLed, byte digitPin0,  byte digitPin1, byte digitPin2, byte digitPin3, 
                byte segPinA, byte segPinB, byte segPinC, byte segPinD, 
                byte segPinE, byte segPinF, byte segPinG, byte segPinH );    
  private:
    byte _digitNumber;  // номер активного разряда      
    byte _typeLed; // тип управления
    byte _digitPin[4];  // выводы выбора разрядов
    byte _segPin[8];  // выводы выбора сегментов  
    const byte _segCod[16] = { // коды символов
      B00111111,  // 0
      B00000110,  // 1
      B01011011,  // 2
      B01001111,  // 3
      B01100110,  // 4
      B01101101,  // 5
      B01111101,  // 6
      B00000111,  // 7
      B01111111,  // 8
      B01101111,  // 9
      B01110111,  // A
      B01111100,  // b
      B00111001,  // C
      B01011110,  // d
      B01111001,  // E
      B01110001,  // F 
    };    
} ;

#endif