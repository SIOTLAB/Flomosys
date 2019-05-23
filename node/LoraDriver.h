#ifndef LoraDriver_h
#define LoraDriver_h 

#include <RH_RF95.h>


#define LNABOOST RH_RF95_LNA_BOOST_HF
#define LNAGAIN RH_RF95_LNA_GAIN_G1
#define TXPOWER 23
#define PABOOST true

class LoraDriver : public RH_RF95{
	public:
		uint8_t channel;
		uint8_t chattempts[3];
		uint16_t chtimeout[3];
		const uint8_t waterLabel = 0x1;
		const uint8_t channelTest[5] = {0,20,10,5,2};
		LoraDriver(uint8_t slaveSelectPin = SS, uint8_t interruptPin = 2, RHGenericSPI& spi = hardware_spi);
		virtual bool init();
		void printRadioConfig();
		void printRecv(uint8_t *data, uint8_t len);
		void printSent(const uint8_t *data, uint8_t len);
		void channelModem(int channel);
		uint32_t getTime();
		bool increaseSF();
		void testChannels();
		void printChannelConfig();
		uint8_t generateAck(uint8_t *mes, uint8_t len);
		bool testAck(uint8_t *ack, uint8_t len, uint8_t mesType);
};
#endif
