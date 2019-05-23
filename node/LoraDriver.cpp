#ifndef LoraDriverImpl
#define LoraDriverImpl
#include "LoraDriver.h"

LoraDriver::LoraDriver(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI& spi)
	:
		RH_RF95(slaveSelectPin, interruptPin, spi){
			_cad_timeout=10000;
			channel=1;


			for(int i=0;i<3;i++){
				chattempts[i]=1;
				chtimeout[i]=5000;
			}
		}

bool LoraDriver::init(){
	if(!RH_RF95::init())
		return false;

	setTxPower(TXPOWER,!PABOOST);
	channelModem(channel);
	spiWrite(RH_RF95_REG_0C_LNA, LNAGAIN | LNABOOST);
	return true;
}

uint32_t LoraDriver::getTime(){
	return millis();
}



bool LoraDriver::increaseSF(){
	return true;
	// Note: We used to use multiple channels for retries. This is depricated.
	//if(channel==3)
	//  return false;
	//channel++;
	//channelModem(channel);
	//return true;
}

void LoraDriver::testChannels(){
	uint8_t buf[20];
	uint8_t len;


	for(uint8_t i=0;i<3;i++){
		channelModem(i);
		uint8_t posRecv = 0;
		uint32_t times[9];

		Serial.print("testing channel: ");
		Serial.println(i);

		for(uint8_t j=0;j<9;j++){

			Serial.println("sending test data");
			send(channelTest , sizeof(channelTest) );
			Serial.println("Waiting for response");
			waitPacketSent();
			unsigned long timer = millis();

			while(!available() && millis()-timer<1000){
				YIELD;
			}
			if(recv(buf,&len)){
				timer = millis()-timer;

				if(testAck(buf,len,channelTest[0])){
					times[posRecv] = timer;
					posRecv++;

				}
				else{

				}

			}
			else{

			}
		}

		chattempts[i] = posRecv/3;
		unsigned long high;
		if(posRecv>0){
			high=times[0];
		}
		for(int k = 1; k<posRecv;k++){
			unsigned long temp=times[k];
			if(temp>high)
				high = temp;
		}
		chtimeout[i] = high + 500;

	}

}
// Send back an ack on receiving a packet (rudimentary)
uint8_t LoraDriver::generateAck(uint8_t *mes, uint8_t len){
	if(len<5){
		return 0;
	}
	if(mes[0]<0x8){

		if(mes[1]==waterLabel){


			return len;
		}
		else if(mes[1] == channelTest[0]){
			for(int i=1;i<sizeof(channelTest)/sizeof(*channelTest);i++){
				if(mes[i]!=channelTest[i]){
					return 0;
				}
			}
			return len;
		}
		else{
			return 0;
		}
	}
	else {
		return 0;
	}
}
// Tests the ack packet we received back to see if it's what we were expecting
bool LoraDriver::testAck(uint8_t *ack, uint8_t len, uint8_t mesType){
	if(len<5){
		return false;
	}
	if(ack[0]<0x8){


		if(ack[1] == channelTest[0]){
			for(int i=1;i<sizeof(channelTest)/sizeof(*channelTest);i++){
				if(ack[i]!=channelTest[i]){
					return false;
				}
			}
			return true;
		}
		else if(ack[1]==mesType){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}

}
#endif
