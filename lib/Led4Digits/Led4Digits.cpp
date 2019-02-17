/*
Led4Digits.h - библиотека управления 4х разрядным семисегментным светодиодным индикатором

Подробно описана в Уроке 20.
http://mypractic.ru/uroki-programmirovaniya-arduino-navigaciya-po-urokam

Библиотека разработана Калининым Эдуардом
mypractic.ru 
*/

#include "Arduino.h"
#include "Led4Digits.h"

//---------------------------- конструктор -----------------------------------
Led4Digits::Led4Digits (byte typeLed, byte digitPin0,  byte digitPin1, byte digitPin2, byte digitPin3, 
                        byte segPinA, byte segPinB, byte segPinC, byte segPinD, 
                        byte segPinE, byte segPinF, byte segPinG, byte segPinH ) {

  // загрузка массивов выводов
  _digitPin[0]=digitPin0; _digitPin[1]=digitPin1; _digitPin[2]=digitPin2; _digitPin[3]=digitPin3;  
  _segPin[0]=segPinA; _segPin[1]=segPinB; _segPin[2]=segPinC; _segPin[3]=segPinD; 
  _segPin[4]=segPinE; _segPin[5]=segPinF; _segPin[6]=segPinG; _segPin[7]=segPinH;  

  _typeLed=typeLed; // тип управления

  // инициализация выводов выбора разрядов
  for (int i = 0; i < 4; i++) {     
    if ( _digitPin[i] != 255 ) {    // если вывод не отключен
      pinMode(_digitPin[i], OUTPUT);
      // устанавливаем вывод в неактивное состояние
      if ( (_typeLed & 1) == 0 ) digitalWrite(_digitPin[i], HIGH);
      else digitalWrite(_digitPin[i], LOW);      
    }    
  }
    
  // инициализация выводов выбора сегментов
  for (int i = 0; i < 8; i++) {     
    pinMode(_segPin[i], OUTPUT);
    // устанавливаем вывод в неактивное состояние
    if ( (_typeLed & 2) == 0 ) digitalWrite(_segPin[i], HIGH);
    else digitalWrite(_segPin[i], LOW);          
  }                            
}

//---------------------------- регенерация индикаторов ----------------------------
// метод должен вызываться регулярно
void Led4Digits::regen() {

  // выключение активного разряда
  if ( (_typeLed & 1) == 0 ) digitalWrite(_digitPin[_digitNumber], HIGH);
  else digitalWrite(_digitPin[_digitNumber], LOW);      

  // новый разряд
  while (true) {
    _digitNumber++;   if (_digitNumber > 3)  _digitNumber= 0;
    if ( _digitPin[_digitNumber] != 255 ) break;
    _digitNumber++;   if (_digitNumber > 3)  _digitNumber= 0;
    if ( _digitPin[_digitNumber] != 255 ) break;
    _digitNumber++;   if (_digitNumber > 3)  _digitNumber= 0;
    if ( _digitPin[_digitNumber] != 255 ) break;    
    _digitNumber++;   if (_digitNumber > 3)  _digitNumber= 0;
    if ( _digitPin[_digitNumber] != 255 ) break;    
  }

  // состояние сегментов для нового разряда
  byte  d;
  // учет активного состояния сегментов
  if ( (_typeLed & 2) == 0 ) d= (digit[_digitNumber]) ^ 0xff;  
  else d= digit[_digitNumber];

  // перегрузка состояния сегментов
  for (byte i = 0; i < 8; i++) {     
  if ( (d & 1) == 0 ) digitalWrite(_segPin[i], LOW);
  else digitalWrite(_segPin[i], HIGH);
  d = d >> 1;    
  }

  // включение нового разряда
  if ( (_typeLed & 1) == 0 ) digitalWrite(_digitPin[_digitNumber], LOW);
  else digitalWrite(_digitPin[_digitNumber], HIGH);          
}

//-------------------------- преобразование тетрады в коды сегментов -------------------------
// аргументы: dig - номер разряда (0 ... 3)
//            tetrad - число для отображения
// в результате коды сегментов оказываются в элементе массива digit[]
void Led4Digits::tetradToSegCod(byte dig, byte tetrad) {
  digit[dig]= (digit[dig] | _segCod[tetrad & 0x0f]) & ((_segCod[tetrad & 0x0f]) | 0x80)  ;   
}


//--------------------------- вывод целого числа ---------------------------------------
// value - число
// digitNum - число разрядов
// blank != 0 - гашение незначащих разрядов 
// возвращает false - ошибка переполнения
boolean Led4Digits::print(unsigned int value, byte digitNum, byte blank) {

  // проверка ошибки переполнения
  if ( (value > 9999) && (digitNum == 4) ) {
    digit[3] = (digit[3] & 0x80) | 0x40; 
    digit[2] = (digit[2] & 0x80) | 0x40; 
    digit[1] = (digit[1] & 0x80) | 0x40; 
    digit[0] = (digit[0] & 0x80) | 0x40;     
    return(false);
  }
  if ( (value > 999) && (digitNum == 3) ) {
    digit[2] = (digit[2] & 0x80) | 0x40; 
    digit[1] = (digit[1] & 0x80) | 0x40; 
    digit[0] = (digit[0] & 0x80) | 0x40;     
    return(false);
  }
  if ( (value > 99) && (digitNum == 2) ) {
    digit[1] = (digit[1] & 0x80) | 0x40; 
    digit[0] = (digit[0] & 0x80) | 0x40;     
    return(false);
  }
  
  // перевод числа в двоично-десятичный код
  byte d1=0;
  byte d2=0;
  byte d3=0;

  // тысячи
  while(true) {    
    if ( value < 1000 ) break;
    value -= 1000;
    d3++;
  } 
  
  // сотни
  while(true) {    
    if ( value < 100 ) break;
    value -= 100;
    d2++;
  } 
  
  // десятки
  while(true) {    
    if ( value < 10 ) break;
    value -= 10;
    d1++;
  } 
  
  // перегрузка и гашение незначащих разрядов
  if ( digitNum > 3 ) {
  if ( (d3 == 0) && (blank != 0) ) digit[3] &= 0x80; // гашение
  else tetradToSegCod(3, d3);      
  }
  if ( digitNum > 2 ) {
  if ( (d3 == 0) && (d2 == 0) && (blank != 0) ) digit[2] &= 0x80; // гашение
  else tetradToSegCod(2, d2);      
  }
  if ( digitNum > 1 ) {
  if ( (d3 == 0) && (d2 == 0) && (d1 == 0) && (blank != 0) ) digit[1] &= 0x80; // гашение
  else tetradToSegCod(1, d1);      
  }
  tetradToSegCod(0, value);
      
  return(true);
}