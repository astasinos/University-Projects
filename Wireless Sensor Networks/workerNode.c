
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

 #include <SPI.h>
 #include <RF22.h>
 #include <RF22Router.h>
 #include <DHT.h>
 #include <DHT_U.h>

#define gasAnalog       A0
#define photoResistor   A1
#define helmetLed       9
#define gasDigital      8
#define DHTPIN        1
#define DHTTYPE         DHT11

// NOTE: Sensor Readings do not have to be seperate functions
// These are just to remind us what sensor readings we must implement.



void lightsAutoOn(){
    // Helmet Led
    int luminoscity = analogRead(photoResistor);
    if (luminoscity < 100) {
            digitalWrite(helmetLed, LOW);  // Turn Helmet Led off
    }
    else{
            digitalWrite(helmetLed, HIGH); // Turn Helmet Led on
    }
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

        // Helmet Led
        pinMode(helmetLed, OUTPUT);
        pinMode(photoResistor, INPUT);


}


void loop(){

        // Get temperature, humidity and realfeel values
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();
        float realfeel = dht.computeHeatIndex(temperature, humidity, false);

        // Get gas level
        int gasLevel = analogRead(gasAnalog);


        lightsAutoOn();




}
