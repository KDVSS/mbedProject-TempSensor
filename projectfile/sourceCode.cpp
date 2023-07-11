/*
Name : Dhana Venkata Siva Sai Kondapalli
Project: By Using temperature sensor on the LCD, Max and Min values after certain range temp cross threshold level
all led's and speaker get activates and then press center joystick switch to contiune reading the temperature after the temp threshold is reached.
Teacher:  Irida Shallari
Lab Teacher: Hiba Alqaysi
*/

#include "mbed.h"    // include the mbed header file as part of this program
#include "LM75B.h"   // include LM75B library to this program
#include "C12832.h"  // include C12832 library to this program

C12832 lcd(p5, p7, p6, p8, p11);  // initialize lcd
I2C tempsensor(p28,p27);          // create an I2C on pin 28 and 27
PwmOut spkr(p26);                 // initialize PwmOut hardware for p26 to generate an audio signal

DigitalIn fire(p14);              // initialize center joystick pin
DigitalOut led1(LED1);            // initialze LED1 pin
DigitalOut led2(LED2);            // initialze LED2 pin
DigitalOut led3(LED3);            // initialze LED3 pin
DigitalOut led4(LED4);            // initialze LED4 pin

int main()                                   // main program starts
{
  float temp, avgTemp, minTemp, maxTemp;     // variables for temperature
  float roomTemp = 28;                     // temperature threshold value
  const int slaveAddr = 0x90;                // define address of the slave
  char temp_read[2];                         // array for the temperature values
  char config_t[3];                          // create config array to save config data
  config_t[0] = 0x01;                        // set pointer register to "config register"
  config_t[1] = 0x60;                        // config data byte1
  config_t[2] = 0xA0;                        // config data byte2
  tempsensor.write(slaveAddr, config_t, 3);  // write on the bus
  config_t[0] = 0x00;                        // set pointer register to "data register"
  tempsensor.write(slaveAddr, config_t, 1);  // send to pointer read temp value

  while (1)                                  // while loop conditions starts
  {
    maxTemp = minTemp = 0.0;                 // initialize maxTemp and minTemp to '0'
    for(int i=1; i<21; i++)                  // this loop for temperature reading
    {
      lcd.locate(0,0);                       // set location for the temperature value in the top left corner
      temp=tempsensor.read(slaveAddr, temp_read, 2); // reading the temperature and assinging back to temp
      temp = 0.125 * (((temp_read[0] << 8) + temp_read[1]) >> 5);
      lcd.printf("Temp: %.2fC\n", temp); // print the temperature value in LCD screen
      if(temp>maxTemp)                   // if temprature value is greater than the maxTemp value,
      {
        maxTemp=temp;                    // then allocate the temperature to maxTemp parameter
      }
      else
      {
        minTemp=temp;                    // else allocate the temperature to minTemp parameter
      }
      lcd.locate(0,10);                  // set location x=0, y=10 for maxTemp value
      lcd.printf("Max: %.2f\n",maxTemp); // print the maxTemp on lcd screen
      lcd.locate(0,20);                  // set location x=0, y=20 for minTemp value
      lcd.printf("Min: %.2f\n",minTemp); // print the minTemp on lcd screen
      wait(5);                           // wait 5 seconds
      avgTemp = (maxTemp + minTemp)/2;
      lcd.locate(65,13);                 // set location x=65, y=13 for avgTemp value
      lcd.printf("Avg-t: %.2fC",avgTemp);
      if(avgTemp>roomTemp)
      {
        for (int i=0.0; i<26.0; i+=2)
        {
            spkr.period(1.0/969.0);  // generates a tone by setting the freequency using PWM harware
            led1 = (!led1);  // toggle LED1
            led2 = (!led2);  // toggle LED1
            led3 = (!led3);  // toggle LED1
            led4 = (!led4);  // toggle LED1
            spkr.write(i/50.0);  // vary the duty cycle
            wait(0.5);  // wait 5 ms
            spkr.period(1.0/800.0);  // generates different tone by changing the freequency using PWM harware
            wait(0.5);    // wait 5 ms
        }
        spkr.write(0.0);  // set the duty cycle to 0% to stop the speaker
        led1 = led2 = led3 = led4 = 0;
        while(!fire) {}   // halts the temp reading until fire (center joystick switch) is pressed

        lcd.cls(); // clear LCD screen
      }// end of if condition
    }// end of for loop
    lcd.cls();    // clear LCD screen
    wait(3);      // wait 3 seconds
  }// end of while loop
}// end of main
