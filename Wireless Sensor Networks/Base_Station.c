#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

If someone is in danger beep a buzzer and blink a red led while also printing DANGER!
Maybe 3 Leds ? One for each worker??

*/

typedef struct {

        float temperature;
        float humidity;
        float realfeel;

} DHTReading;


void DecodeDHTReadings(char *v, DHTReading *t) {

        char temperature[5] = "";
        char humidity[5] = "";
        char realfeel[5] = "";

        int i = 0;
        while(((*(++v) != 'H') && (temperature[i++] = *v)) || (i = 0));
        while(((*(++v) != 'F') && (humidity[i++] = *v)) || (i = 0));
        while(realfeel[i++] = *(++v));

        t->temperature = atof(temperature);
        t->humidity = atof(humidity);
        t->realfeel = atof(realfeel);


}

int main()
{
        DHTReading t;
        DecodeDHTReadings("T34.5H33.99F3", &t);

        printf("%.2f\n", t.temperature);
        printf("%.2f\n", t.humidity);
        printf("%.2f", t.realfeel);
        return 0;
}
