#include "LoraDriver.h"
//#define BANDWIDTH1 RH_RF95_BW_125KHZ
//#define CODINGRATE1 RH_RF95_CODING_RATE_4_8
//#define IMPLICITHEADER1 false
//
//#define SPREADINGFACTOR1 RH_RF95_SPREADING_FACTOR_128CPS
//#define TXCONTINUOUS1 false
//#define CRCON1 true
//
//#define LOWDATARATEOPTIMIZE1 false
//#define AUTOGAINCONTROLLER1 false
//
//#define FREQUENCY1 914
//
//
//
//#define BANDWIDTH2 RH_RF95_BW_125KHZ
//#define CODINGRATE2 RH_RF95_CODING_RATE_4_8
//#define IMPLICITHEADER2 false
//
//#define SPREADINGFACTOR2 RH_RF95_SPREADING_FACTOR_1024CPS
//#define TXCONTINUOUS2 false
//#define CRCON2 true
//
//#define LOWDATARATEOPTIMIZE2 false
//#define AUTOGAINCONTROLLER2 false
//
//#define FREQUENCY2 915


#define BANDWIDTH3 RH_RF95_BW_125KHZ
#define CODINGRATE3 RH_RF95_CODING_RATE_4_8
#define IMPLICITHEADER3 false

#define SPREADINGFACTOR3 RH_RF95_SPREADING_FACTOR_4096CPS
#define TXCONTINUOUS3 false
#define CRCON3 true

#define LOWDATARATEOPTIMIZE3 true
#define AUTOGAINCONTROLLER3 false

#define FREQUENCY3 916

void LoraDriver::channelModem(int channel){
	RH_RF95::ModemConfig *temp;
	temp=(RH_RF95::ModemConfig*)malloc(sizeof(*temp));
	//channel = 2; 
	// The channel is always set to 2 for now, regardless of what you ask for.
	// The surrounding code is preserved for reference in case this changes. 

	spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE);

	//if(channel==0){
	// temp->reg_1d= BANDWIDTH1 | CODINGRATE1 | IMPLICITHEADER1;
	// temp->reg_1e= SPREADINGFACTOR1 | (TXCONTINUOUS1 <<3) | (CRCON1 <<2);
	//  temp->reg_26= (LOWDATARATEOPTIMIZE1 <<3) | (AUTOGAINCONTROLLER1<<2);
	//  setFrequency(FREQUENCY1);
	//}
	//else if(channel==1){
	//  temp->reg_1d= BANDWIDTH2 | CODINGRATE2 | IMPLICITHEADER2;
	//  temp->reg_1e= SPREADINGFACTOR2 | (TXCONTINUOUS2 <<3) | (CRCON2 <<2);
	//  temp->reg_26= (LOWDATARATEOPTIMIZE2 <<3) | (AUTOGAINCONTROLLER2<<2);
	//  setFrequency(FREQUENCY2);
	//}
	//else{
	temp=(RH_RF95::ModemConfig*)malloc(sizeof(*temp));
	temp->reg_1d= BANDWIDTH3 | CODINGRATE3 | IMPLICITHEADER3;
	temp->reg_1e= SPREADINGFACTOR3 | (TXCONTINUOUS3 <<3) | (CRCON3 <<2);
	temp->reg_26= (LOWDATARATEOPTIMIZE3 <<3) | (AUTOGAINCONTROLLER3<<2);
	setFrequency(FREQUENCY3);
	//}
	setModemRegisters(temp);
	free(temp);
}
