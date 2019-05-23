#include <SPI.h>       
#include "LoraDriver.h"  
#include <RHEncryptedDriver.h>
#include <LowPower.h> 

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 8


#define LED_POWER_PIN A0  //seems to actually go to led labeled A2
#define LED_ULTRASONIC_PIN A2 //seems to actually go to LED labeled A0
#define LED_RADIO_PIN A1 

#define MINSLEEP 60000 // 1 minute = 60000 ms. shortened for test purposes
#define LONGSLEEP 900000 // 15 minutes = 900000 ms. shortened for test purposes

//#define SIMDATA
//#define VERBOSE_MODE

uint8_t buf[20]; 
uint8_t len; 


void setup() {
#ifdef VERBOS_MODE
	Serial.begin(9600);
	while(!Serial);
#endif

	setupLEDs();
	setupUltrasonic();
	setupRadio();
}

void loop() {
	len=0;
	buf[len]=CLIENT_ADDRESS;
	len++;


	wakeUltrasonic();
	ultrasonic();
	delay(100);
	len = readUltrasonic(buf,len);

	sleepUltrasonic();
	wakeRadio();
	int count=0;
	while(!sendReliably(buf,len)){ 
		count++;
		sleepRadio();
		sleep(MINSLEEP);
		// if we can't send a message after 4 cycles, stop trying
		if(count>4){
			break;
		}
		wakeRadio();
	}
	sleepRadio();
	sleep(LONGSLEEP);
}

//set up led debugging pins
void setupLEDs(){
	pinMode(LED_POWER_PIN, OUTPUT);
	digitalWrite(LED_POWER_PIN, HIGH);
	pinMode(LED_ULTRASONIC_PIN, OUTPUT);
	digitalWrite(LED_ULTRASONIC_PIN,LOW);
	pinMode(LED_RADIO_PIN,OUTPUT);
	digitalWrite(LED_RADIO_PIN,LOW);
}


//makes the whole system sleep for the designated number of milliseconds
void sleep(uint32_t timeToSleep){
	//radio.sleep();
#ifdef VERBOSE_MODE
	Serial.println("sleeping");
	delay(100);
#endif

	//double check that radio and ultrasonic sensor are sleeping
	sleepRadio();
	sleepUltrasonic();

	while(timeToSleep>0){
		LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
		if(timeToSleep<8000){ //check to ensure we don't have unsigned integer underflow
			timeToSleep = 0;
		}
		else{
			timeToSleep-=8000;
		}
	}

#ifdef VERBOSE_MODE
	Serial.println("waking up");
#endif
}
