
/*

   Code that runs on the sensor nodes located on the helmets of the workers.

   BLOCK DIAGRAM:

   1. Get Sensor Readings
   2. Check if dangerous levels have been reached and perform appropriate actions
      (e.g. beep a buzzer)
   3. Report readings back to base station along with safety status of the workers
   4. Repeat

 */


/*

   TODO:

   What about multihop routing?

   
   2. Create receiver implementation.
   3. Begin Base station
   4. Connect LEDs and LCD

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
#define buzzerPin       2
#define DHTPIN          1
#define DHTTYPE         DHT11
#define MY_ADDRESS      1
#define DESTINATION_ADDRESS 10 // 10 is Base

RF22Router rf22(MY_ADDRESS);


void lightsAutoOn(){
        // Helmet Led
        int luminoscity = analogRead(photoResistor);
        if (luminoscity < 100) {
                digitalWrite(helmetLed, LOW); // Turn Helmet Led off
        }
        else{
                digitalWrite(helmetLed, HIGH); // Turn Helmet Led on
        }
}





DHT dht = DHT(DHTPIN, DHTTYPE);
bool button_pressed = 0;
bool beepBuzzer = 0;
bool Danger = 0;
bool EarthquakeNotice = 0;

void setup(){

        Serial.begin(9600);

        if (!rf22.init())
                Serial.println("RF22 init failed");

        if (!rf22.setFrequency(434.0))
                Serial.println("setFrequency Fail");

        rf22.setTxPower(RF22_TXPOW_20DBM);

        //rf22.setModemConfig(RF22::OOK_Rb40Bw335  );
        rf22.setModemConfig(RF22::GFSK_Rb125Fd125);

        /* tells my radio card that if I want to send data to
           DESTINATION_ADDRESS then I will send them directly to
           DESTINATION_ADDRESS and not to another radio who would act as a relay
         */
        rf22.addRouteTo(DESTINATION_ADDRESS, DESTINATION_ADDRESS);
        // test if declaration in setup is valid
        int seed = analogRead(photoResistor);
        randomSeed(seed);

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
        float gasLevel = analogRead(gasAnalog);

        /* if((gaslevel > 100) || (temperature > ...) || button_pressed ||  EarthquakeNotice || .... )  {


        Danger = 1;
        tone(buzzerPin, 2000, 200);

    } else {
    Danger = 0;
}


        */

        lightsAutoOn();

        // Serial print values
        char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
        uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
        memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
        memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
        sprintf(data_read, "T%.2fH%.2fR%.2fG%.2fD%d", temperature, humidity, realfeel, gasLevel, Danger);
        data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
        memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN);

        boolean successful_packet = false;
        int max_delay = 500;

        while (!successful_packet)
        {

                if (rf22.sendtoWait(data_send, sizeof(data_send), DESTINATION_ADDRESS) != RF22_ROUTER_ERROR_NONE)
                {
                        Serial.println("sendtoWait failed");
                        randNumber=random(200,max_delay);
                        Serial.println(randNumber);
                        delay(randNumber);
                }
                else
                {
                        successful_packet = true;
                        Serial.println("sendtoWait Succesful");
                }
        }

        delay(100); // Wait a little before entering receiver mode


        uint8_t buf[RF22_ROUTER_MAX_MESSAGE_LEN];
        char incoming[RF22_ROUTER_MAX_MESSAGE_LEN];
        memset(buf, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
        memset(incoming, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
        uint8_t len = sizeof(buf);
        uint8_t from;
        int received_value = 0;



        if (rf22.recvfromAck(buf, &len, &from))
        {
                buf[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
                memcpy(incoming, buf, RF22_ROUTER_MAX_MESSAGE_LEN);
                Serial.print("got request from : ");
                Serial.println(from, DEC); //
                received_value = atoi((char*)incoming);
                Serial.println(received_value);

        }

    if(!strcmp(incoming, "EARTHQUAKE")) {

        EarthquakeNotice = 1;

    }

    if(!strcmp(incoming, "SAFE")) {

        EarthquakeNotice = 0;
    }

}
