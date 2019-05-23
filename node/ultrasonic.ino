#define PW_PIN  4 //Pulse width pin from ultrasonic sensor
#define VCC_ULTRASOUND_PIN 7  //VCC from ultrasonic sensor
#define RX_PIN 3 //Not sure what this pin does but it is used for the ultrasonic sensor

#define WATER_DIST_LABEL 1 //label that declares the type of data being transmitted

#define ULTRASONIC_RETRIES 10 //maximum number of times we will try to read the sensor
#define ULTRASONIC_AVG_COUNT 3  //the number of good readings we want to get before we send the data


//object to help converting from a float to unsigned bytes
union Float {
	float    m_float;
	uint8_t  m_bytes[sizeof(float)];
};

float obj;

uint8_t readUltrasonic(uint8_t* buf, uint8_t len){
#ifdef SIMDATA
	float tempData = 24.5;
	Float dummyData;
	dummyData.m_float = tempData;

	len = 1;

	buf[len]=WATER_DIST_LABEL;
	len++;
	buf[len]= dummyData.m_bytes[0];
	len++;
	buf[len]= dummyData.m_bytes[1];
	len++;
	buf[len]= dummyData.m_bytes[2];
	len++;
	buf[len]= dummyData.m_bytes[3];
	len++;
	return len;
#endif
	obj=0;
	float cm=0; 
	uint8_t goodreads = 0; 
	uint8_t numreads = 0;
	Float data; 
	data.m_float = 0.0;
	float total =0.0;
	int newSub =0;
	ultrasonic();

	for(int newCount =0;newCount < 7;newCount++){
		ultrasonic();

		cm=obj;
		if(cm==0){
			newSub++; 
		}
		else{ 
			total+= obj;
		}

	}

	data.m_float =total/(7-newSub);


	buf[len]=WATER_DIST_LABEL;
	len++;
	buf[len]=data.m_bytes[0];
	len++;
	buf[len]=data.m_bytes[1];
	len++;
	buf[len]=data.m_bytes[2];
	len++;
	buf[len]=data.m_bytes[3];
	len++;

	return len;
}


long ultrasonic()
{ 
	long cm = ( (long)pulseIn(PW_PIN,HIGH) ) /58;  //converts bit count to cm


#ifdef VERBOS_MODE
	Serial.print("Distance (cm): "); 
	Serial.println(cm);  
	delay(200);
#endif     

	obj=cm;
	return cm;
}

//sets up the ultrasonic pins and variables
void setupUltrasonic(){
	pinMode(PW_PIN,INPUT);
	pinMode(VCC_ULTRASOUND_PIN, OUTPUT);
	pinMode(RX_PIN, OUTPUT);
#ifdef VERBOS_MODE
	Serial.println("ultrasonic setup");
#endif
}

void wakeUltrasonic(){
	digitalWrite(LED_ULTRASONIC_PIN,HIGH);
	digitalWrite(VCC_ULTRASOUND_PIN, HIGH);
	delay(100);
}

void sleepUltrasonic(){
	digitalWrite(VCC_ULTRASOUND_PIN,LOW);
	digitalWrite(LED_ULTRASONIC_PIN,LOW);
}
