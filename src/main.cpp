// упрощенная охранная сигнализация

#include <MsTimer2.h>
#include <Button.h>
#include <Led4Digits.h>

#define DOOR_SENS_PIN 12     // датчик двери подключен к выводу 12
#define SECRET_BUTTON_PIN 11 // скрытая кнопка подключена к выводу 11
#define LED_PIN 10           // светодиод подключен к выводу 10
#define SIREN_PIN 9          // сирена подключена к выводу 9

#define TIME_LED_PERIOD 500 // время периода мигания светодиода (* 2 мс)
#define TIME_LED_ON 100     // время включенного светодиода
#define TIME_LED_ALARM 62   // время периода мигания светодиода при ТРЕВОГЕ (* 2 мс)
#define TIME_ALARM 15000    // время в режиме ТРЕВОГА (* 2 мс)

Button doorSens(DOOR_SENS_PIN, 50);         // создание объекта датчик двери, типа кнопка
Button secretButton(SECRET_BUTTON_PIN, 25); // создание объекта скрытая кнопка, типа кнопка

boolean sirenOn;             // признак включения сирены
unsigned int ledTimeCount;   // счетчик времени для светодиода
unsigned int alarmTimeCount; // счетчик времени тревоги
                             //------------------- обработчик прерывания 2 мс ---------------------
volatile byte soundOn = false;

Led4Digits disp(0, 5,  2, 3, 4, 
                7, A5, A3, A1, 
                A0, 8, A4, A2 );    

void timerInterupt()
{
  disp.regen(); // регенерация индикатора
  doorSens.filterAvarage();     // вызов метода фильтрации сигнала для датчика двери
  secretButton.filterAvarage(); // вызов метода фильтрации сигнала для скрытой кнопки
                                // блок управления сиреной
  if (sirenOn == true)
  {
    // tone(SIREN_PIN, 1000);
    if (soundOn == false)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      soundOn = true;
      Serial.println("Timer tone");
      //tone(SIREN_PIN, 1000);
    }
  }
  else if (soundOn)
  {
    Serial.println("Timer no tone");
    soundOn = false;
    //noTone(SIREN_PIN);
    digitalWrite(LED_BUILTIN, LOW);
  }

  ledTimeCount++;   // счетчик времени мигания светодиода
  alarmTimeCount++; // счетчик времени тревоги
}
void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(LED_PIN, OUTPUT);        // определяем вывод светодиода как выход
  pinMode(SIREN_PIN, OUTPUT);      // определяем вывод сирены как выход
  MsTimer2::set(2, timerInterupt); // задаем период прерывания по таймеру 2 мс
  MsTimer2::start();               // разрешаем прерывание по таймеру
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop()
{
  for (int i = 0; i < 32; i++) {
    if ( i == 0) disp.digit[0]= 1;
    else if ( i == 8)  disp.digit[1]= 1;
    else if ( i == 16) disp.digit[2]= 1;
    else if ( i == 24) disp.digit[3]= 1;
    else {
      disp.digit[0] = disp.digit[0] << 1;
      disp.digit[1] = disp.digit[1] << 1;
      disp.digit[2] = disp.digit[2] << 1;
      disp.digit[3] = disp.digit[3] << 1;      
    }
    delay(250);    
  }  
/*
//---------------------- режим ОТКЛЮЧЕНА --------------------------
guard_off:
  Serial.println("OFF");
  while (true)
  {

    digitalWrite(LED_PIN, LOW); // светодиод не горит
    sirenOn = false;            // сирена не звучит

    // если нажали кнопку, переход на режим ОХРАНА
    if (secretButton.flagClick == true)
    {
      secretButton.flagClick = false;
      goto guard_on;
    }
  }

//---------------------- режим ОХРАНА --------------------------------
guard_on:
  Serial.println("ON");

  while (true)
  {

    sirenOn = false;    // сирена не звучит
    alarmTimeCount = 0; // сброс счетчика времени тревоги

    // светодиод мигает раз в секунду
    if (ledTimeCount >= TIME_LED_PERIOD)
      ledTimeCount = 0;
    if (ledTimeCount < TIME_LED_ON)
      digitalWrite(LED_PIN, HIGH);
    else
      digitalWrite(LED_PIN, LOW);

    // если нажали кнопку, переход на режим ОТКЛЮЧЕНА
    if (secretButton.flagClick == true)
    {
      secretButton.flagClick = false;
      goto guard_off;
    }

    // если сработал датчик двери, переход на режим ТРЕВОГА
    if (doorSens.flagPress == true)
      goto alarm;
  }

//---------------------- режим ТРЕВОГА --------------------------------
alarm:
  Serial.println("ALARM");

  while (true)
  {

    sirenOn = true; // звучит сирена

    // светодиод мигает 4 раза в секунду
    if (ledTimeCount >= TIME_LED_ALARM)
    {
      ledTimeCount = 0;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }

    // если нажали кнопку, переход на режим ОТКЛЮЧЕНА
    if (secretButton.flagClick == true)
    {
      Serial.println("Cliked secret");

      secretButton.flagClick = false;
      goto guard_off;
    }

    // проверка времени тревоги ( 30 сек )
    if (alarmTimeCount >= TIME_ALARM)
      goto guard_off;
  }
  */
}
