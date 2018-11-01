/*
signals morse code of 'SOS'
*/

#include <Arduino.h>

int LED = 13;

int DotPeriod = 500;

void setup() {

    init();
    
    Serial.begin(9600);

    pinMode(LED, HIGH);
}

void s() {
    Serial.println("signal s");

    int i;

    for (i = 0; i<3; i++) {
        digitalWrite(LED, HIGH);
        delay(DotPeriod);

        digitalWrite(LED, LOW);
        delay(DotPeriod);
        }
}

void o() {
    Serial.println("signal o");

    int i;

    for (i = 0; i < 3; i++) {
        digitalWrite(LED, HIGH);
        delay(3 * DotPeriod);

        digitalWrite(LED, LOW);
        delay(DotPeriod);
    }
}

int main(){
    setup();
    
    while(true){
        s();
        o();
        s();
    }
    

}