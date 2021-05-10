/*

   Code that runs on the sensor nodes located on the helmets of the workers.

   BLOCK DIAGRAM:

   1. Get Sensor Readings
   2. Check if dangerous levels have been reached and perform appropriate actions
      (e.g. beep a buzzer)
   3. Report readings back to base station along with safety status of the workers
   4. Repeat


   IMPORTANT:

   Emergency button should probably be implemented as an interrupt (if possible) to
   immediately interrupt any time-consuming running tasks.

 */


#define gasAnalog 0
#define gasDigital 8


// DS18B20 or DHT11
void getTemperature(){
        ;
}

void getHumidity() {
        ;
}

void lightsAutoOn(){
        ;
}

int getGasLevels(){

    int gasLevel = analogRead(gasAnalog);
    return gasLevel;

}

void beepBuzzer(){
        ;
}



void setup(){

        pinMode(gasAnalog, INPUT);
        pinMode(gasDigital, INPUT);

}


void loop(){

        ;
}
