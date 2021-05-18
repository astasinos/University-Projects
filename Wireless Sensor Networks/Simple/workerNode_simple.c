#include <SPI.h>
#include <RF22.h>
#include <RF22Router.h>
#include <OneWire.h>
#include <DallasTemperature.h>






#define gasAnalog       A1
#define photoResistor   A0
#define helmetLed       5
#define buzzerPin       4
#define buttonPin       6
#define ONE_WIRE_BUS    3           // FOR DS18B20
#define MY_ADDRESS      1           // NODE1
#define DESTINATION_ADDRESS 10      // 10 is Base

RF22Router rf22(MY_ADDRESS);


void lightsAutoOn(){
        // Helmet Led
        int luminoscity = analogRead(photoResistor);
        if (luminoscity < 100) {
                digitalWrite(helmetLed, HIGH); // Turn Helmet Led off
        }
        else{
                digitalWrite(helmetLed, LOW); // Turn Helmet Led on
        }
}



OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);



int button_pressed = 0;
unsigned long timenow;
int Danger = 0;
int EarthquakeNotice = 0;
int randNumber = 0;

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
        pinMode(buttonPin, INPUT);

        sensors.begin();

        // Helmet Led
        pinMode(helmetLed, OUTPUT);
        pinMode(photoResistor, INPUT);
        pinMode(buzzerPin, OUTPUT);



}


void loop(){

        // Get temperature, humidity and realfeel values
        sensors.requestTemperatures();
        float temperature_val =  sensors.getTempCByIndex(0);
        char temperature[6];
        dtostrf(temperature_val, 4, 2, temperature);

        // Get gas level
        int gasLevel = analogRead(gasAnalog);

        button_pressed = digitalRead(buttonPin);
        if((gasLevel > 800) || (temperature_val > 32.0 ) || button_pressed ||  EarthquakeNotice )  {

                tone(buzzerPin, 2000, 500);
                Danger = 1;


        } else {
                Danger = 0;
        }



/*
        Serial.print("Temperature : ");
        Serial.print(temperature);
        Serial.print("   Gas Level: ");
        Serial.print(gasLevel);
        Serial.print(" Light value : ");
        Serial.print(analogRead(photoResistor));
        Serial.print("  Button : ");
        Serial.println(button_pressed);

*/
        lightsAutoOn();

        // Serial print values
        char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
        uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
        memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
        memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
        sprintf(data_read, "T%sG%dD%d", temperature, gasLevel, Danger);
        data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
        memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN);

        int successful_packet = false;
        int max_delay = 500;

        if(millis() - timenow > 5000) {
            timenow = millis();
        while (!successful_packet)
        {


                if (rf22.sendtoWait(data_send, sizeof(data_send), DESTINATION_ADDRESS) != RF22_ROUTER_ERROR_NONE)   // strlen is better here
                {
                        Serial.print("sendtoWait failed");
                        randNumber=random(200,max_delay);
                        serial.print(" : I will wait for ");
                        Serial.println(randNumber);
                        delay(randNumber);
                }
                else
                {
                        successful_packet = true;

                        Serial.print("sendtoWait Succesful");
                        Serial.print("I sent the message : ");
                        Serial.println(data_read);
                }


        }
    }






}
