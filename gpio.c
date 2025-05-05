#include <stdio.h>
#include <wiringPi.h>

#define NUM 0   //26pin

int main (void)
{

    wiringPiSetup () ;

    int i = 0;
    for(i = 0; i < 3; i++) {
        pinMode (i, OUTPUT);
        digitalWrite (i, LOW);  // On
    }
    for ( ;; )
    {
        digitalWrite (NUM, HIGH) ;  // On
        delay (1000) ;      // mS
        digitalWrite (NUM, LOW) ;   // Off
        delay (1000) ;
    }

    return 0;
}
