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

 #include <DHT.h>
 #include <DHT_U.h>

#define gasAnalog   0
#define gasDigital  8
#define DHT11PIN    1
#define DHTTYPE DHT11

// DS18B20 or DHT11
void getTemperature(){

        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();
        float realfeel = dht.computeHeatIndex(temperature, humidity, false);




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

DHT dht = DHT(DHTPIN, DHTTYPE);


void setup(){

        Serial.begin(9600);

        pinMode(gasAnalog, INPUT);
        pinMode(gasDigital, INPUT);

        dht.begin();


}


void loop(){

        ;
}
