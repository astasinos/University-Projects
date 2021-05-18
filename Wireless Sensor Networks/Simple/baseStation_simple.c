#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPI.h>
#include <RF22.h>
#include <RF22Router.h>


#define MY_ADDRESS      10
#define DESTINATION_ADDRESS_1 1
#define DESTINATION_ADDRESS_2 2
#define DESTINATION_ADDRESS_3 3

RF22Router rf22(MY_ADDRESS);

/*

   T35G123D1



 */

int successful_packet = 0;


int DESTINATION_ADDRESSES[3] = {DESTINATION_ADDRESS_1, DESTINATION_ADDRESS_2, DESTINATION_ADDRESS_3};
unsigned long earthquake_time;

typedef struct {

        float temperature;
        int gaslevel;
        int danger;
        int workerid;


} worker;


void DecodeMessage(const char *v, worker *t, int from) {

        char temperature[6] = "";
        char gaslevel[5] = "";
        char danger[2] = "";


        int i = 0;
        while(((*(++v) != 'G') && (temperature[i++] = *v)) || (i = 0));
        while(((*(++v) != 'D') && (gaslevel[i++] = *v)) || (i = 0));
        while(danger[i++] = *(++v));

        t->temperature = atof(temperature);
        t->gaslevel = atof(gaslevel);
        t->workerid = from;
        t->danger = atoi(danger);



}


void setup() {



        Serial.begin(9600);

        if (!rf22.init())
                Serial.println("RF22 init failed");

        if (!rf22.setFrequency(434.0))
                Serial.println("setFrequency Fail");

        rf22.setTxPower(RF22_TXPOW_20DBM);

        //rf22.setModemConfig(RF22::OOK_Rb40Bw335  );
        rf22.setModemConfig(RF22::GFSK_Rb125Fd125);

        for(int i = 0 ; i < 3 ; i ++ ) { rf22.addRouteTo(DESTINATION_ADDRESSES[i], DESTINATION_ADDRESSES[i]); }
}

void loop()
{




        uint8_t buf[RF22_ROUTER_MAX_MESSAGE_LEN];
        char incoming[RF22_ROUTER_MAX_MESSAGE_LEN];
        memset(buf, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
        memset(incoming, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
        uint8_t len = sizeof(buf);
        uint8_t from;


        if (rf22.recvfromAck(buf, &len, &from))
        {
                buf[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
                memcpy(incoming, buf, RF22_ROUTER_MAX_MESSAGE_LEN);
                Serial.print("got request from : ");
                Serial.println(from, DEC);
                Serial.println(incoming);

                worker t;
                DecodeMessage(incoming, &t, from);         // test that you can pass a char array!
                Serial.print("Miner ");
                Serial.print(t.workerid);
                Serial.print(" has temperature ");
                Serial.print(t.temperature);
                Serial.print(" and Gas Level : ");
                Serial.print(t.gaslevel);
                Serial.print(" Danger : ");
                Serial.println(t.danger);

                if(t.danger == 1) {

                    Serial.print("Dangerous ");
                    if((t.temperature > 30) && (t.gaslevel > 800)) Serial.print(" Gas and Temperature levels detected! ");
                    if((t.temperature > 30) && (t.gaslevel < 800)) Serial.print(" Temperature levels deteccted! ");
                    if((t.temperature < 30) && (t.gaslevel > 800)) Serial.print(" Gas levels deteccted! ");
                    Serial.print("Miner ");
                    Serial.print(t.workerid);
                    Serial.println(" is in DANGER!");


                }
        }

        return 0;
}
