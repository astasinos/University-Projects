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







int successful_packet = 0;
int randNumber = 0;
int max_delay = 500;
int ReceivedWorkerCount = 0;
int sendDataNow = 1;
int DESTINATION_ADDRESSES[3] = {DESTINATION_ADDRESS_1, DESTINATION_ADDRESS_2, DESTINATION_ADDRESS_3};
unsigned long timenow = millis();
int EarthquakeNotice = 0;
typedef struct {

        float temperature;
        int gaslevel;
        int danger;
        int button_pressed;
        int workerid;


} worker;


void DecodeMessage(const char *v, worker *t, int from) {

        char button_pressed[2];
        char temperature[6] = "";
        char gaslevel[5] = "";
        char danger[2] = "";


        int i = 0;
        while(((*(++v) != 'T') && (button_pressed[i++] = *v)) || (i = 0));
        while(((*(++v) != 'G') && (temperature[i++] = *v)) || (i = 0));
        while(((*(++v) != 'D') && (gaslevel[i++] = *v)) || (i = 0));
        while(danger[i++] = *(++v));

        t->temperature = atof(temperature);
        t->gaslevel = atof(gaslevel);
        t->workerid = from;
        t->danger = atoi(danger);
        t->button_pressed = atoi(button_pressed);



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


        if((millis() - timenow > 20000) && sendDataNow){
          
          char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
                uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
                memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
                memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
                strcpy(data_read, "SEND_DATA");
                data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
                memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN);

                for(int i = 0; i < 3; i++) {
                        successful_packet = false;
                        while (!successful_packet)
                        {

                                if (rf22.sendtoWait(data_send, sizeof(data_send), DESTINATION_ADDRESSES[i]) != RF22_ROUTER_ERROR_NONE)
                                {
                                        Serial.println("sendtoWait failed");        // Add to which node it failed
                                        randNumber=random(200,max_delay);
                                        Serial.println(randNumber);
                                        delay(randNumber);
                                }
                                else
                                {
                                        successful_packet = true;
                                       // Serial.println("sendtoWait Succesful");
                                }
                        }
                        


                }
                sendDataNow = 0;
                
                ReceivedWorkerCount = 0;
                
          
          
          }

          // Receive

        uint8_t buf[RF22_ROUTER_MAX_MESSAGE_LEN];
        char incoming[RF22_ROUTER_MAX_MESSAGE_LEN];
        memset(buf, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
        memset(incoming, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
        uint8_t len = sizeof(buf);
        uint8_t from;


        if ((ReceivedWorkerCount < 3) && rf22.recvfromAck(buf, &len, &from))
        {
                ReceivedWorkerCount++;
                buf[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
                memcpy(incoming, buf, RF22_ROUTER_MAX_MESSAGE_LEN);
                //Serial.print("got request from : ");
                //Serial.println(from, DEC);
                //Serial.println(incoming);

                worker t;
                DecodeMessage(incoming, &t, from);         // test that you can pass a char array!
                Serial.print("\nMiner ");
                Serial.print(t.workerid);
                
                Serial.print(" has temperature ");
                Serial.print(t.temperature);
                Serial.print(" and Gas Level : ");
                Serial.print(t.gaslevel);
                Serial.print(" Danger : ");
                Serial.println(t.danger);

                if(t.danger == 1) {
                  
                    if((t.temperature > 32) && (t.gaslevel > 800)) Serial.print(" Dangerous Gas and Temperature levels detected! ");
                    if((t.temperature > 32) && (t.gaslevel < 800)) Serial.print(" Dangerous Temperature levels deteccted! ");
                    if((t.temperature < 32) && (t.gaslevel > 800)) Serial.print(" Dangerous Gas levels deteccted! ");
                    Serial.print("\nMiner ");
                    Serial.print(t.workerid);
                    Serial.println(" is in DANGER!\n");

                    
                    
                    if(t.button_pressed == 1) {

                      Serial.print("Miner ");
                      Serial.print(t.workerid);
                      Serial.println(" is pressing the emergency button!\n");
                      }

                } 
                
        }

      if ((ReceivedWorkerCount == 3) && !sendDataNow)
      { 
        timenow = millis(); sendDataNow = 1; 
      } 


        
        float sensingEarthquake  = analogRead(A0);
        
    if(sensingEarthquake > 120) {
      
        EarthquakeNotice = 1;
      }



if(EarthquakeNotice && (ReceivedWorkerCount == 3)) {

                char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
                uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
                memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
                memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
                strcpy(data_read, "EARTHQUAKE");
                data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
                memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN);
               Serial.println("\nWARNING: EARTHQUAKE! CALLING ALL MINERS!");
               

                for(int i = 0; i < 3; i++) {
                        successful_packet = false;
                        while (!successful_packet)
                        {

                                if (rf22.sendtoWait(data_send, sizeof(data_send), DESTINATION_ADDRESSES[i]) != RF22_ROUTER_ERROR_NONE)
                                {
                                        Serial.println("sendtoWait failed");    // Add to which node it failed
                                        randNumber=random(200,max_delay);
                                        Serial.println(randNumber);
                                        delay(randNumber);
                                }
                                else
                                {
                                        successful_packet = true;
                                       // Serial.println("sendtoWait Succesful");
                                }
                        }


                }

               EarthquakeNotice = 0;

        }



        

        




        

        
}