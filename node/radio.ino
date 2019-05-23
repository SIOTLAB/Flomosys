#define ENABLE_RADIO_PIN 6
#define VCC_RADIO_PIN 8

LoraDriver radio; //controls the radio
//trys to send the message on each channel until it gets an ack back
bool sendReliably(const uint8_t *data, uint8_t len){

	//temporary buffer to receive acks
	uint8_t buf[20];


	for(int ch=0;ch<3;ch++){

		//send data
		radio.send(data,len);
		radio.waitPacketSent();
		//listen until receive ack or timeout expires
		if(!radio.waitAvailableTimeout(radio.chtimeout[i])){
			//no message received. try again
#ifdef VERBOSE_MODE
			Serial.println("no ack recvied");
#endif
			continue; 
		}
		len=8;
		//read ack into buffer
		if(radio.recv(buf,&len)){

#ifdef VERBOSE_MODE
			Serial.print("got reply: ");
			for(int i=0;i<len;i++){
				Serial.print((char)buf[i]);
			}
			Serial.println("");
			delay(50);
#endif
			//check if message recevied is right ack
			if(radio.testAck(buf,len,radio.waterLabel)){//good ack, return
				return true;
			}

			//not an ack for us, go on to next test message
			continue;
		}
		else
		{
#ifdef VERBOSE_MODE
			Serial.println("recv failed");
#endif
			//received packet, but couldn't read it properly


		}

	}

	//all attempts to send have failed
#ifdef VERBOSE_MODE
	Serial.println("all attempts have failed");
#endif
	return false;
}

void setupRadio(){
	//set up radio pins and turn on radio
	pinMode(ENABLE_RADIO_PIN,OUTPUT);
	pinMode(VCC_RADIO_PIN,OUTPUT);
	wakeRadio();

	//ensure radio is properly configured
#ifdef VERBOSE_MODE
	Serial.println("testing channels");
	radio.testChannels();
#endif

	radio.channelModem(2);

#ifdef VERBOSE_MODE
	radio.printRadioConfig();
	radio.printChannelConfig();
#endif

	sleepRadio();
}

void wakeRadio(){
	digitalWrite(LED_RADIO_PIN,HIGH);
	digitalWrite(VCC_RADIO_PIN, HIGH);
	digitalWrite(ENABLE_RADIO_PIN, HIGH);
	delay(50);
	if (!radio.init())
#ifdef VERBOSE_MODE
		Serial.println("init failed");
#endif
	;
}

void sleepRadio(){
	radio.waitPacketSent();
	radio.spiWrite(RH_RF95_REG_01_OP_MODE,RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE);
	digitalWrite(ENABLE_RADIO_PIN,LOW);
	digitalWrite(VCC_RADIO_PIN,LOW);
	digitalWrite(LED_RADIO_PIN,LOW);
}  

