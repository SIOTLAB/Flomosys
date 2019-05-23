#include <bcm2835.h> 
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <iostream> 
#include <fstream> 

#include "../node/LoraDriver.h"

#define RASPI

#include "../lib/RadioHead/RHutil/RasPiBoards.h"

#define RF_FREQUENCY  915.00
#define RF_NODE_ID    1

LoraDriver radio(RF_CS_PIN); 

volatile sig_atomic_t force_exit = false;

void sig_handler(int sig)
{
  printf("\n%s Break received, exiting!\n", __BASEFILE__);
  force_exit=true;
}


//used to convert ultrasonic data
union Float {
    float    m_float;
    uint8_t  m_bytes[sizeof(float)];
};
Float data;
void print_custom_buffer(uint8_t buff[], int len)
{
 int i;
 Float reading;
 for(i=0;i<4;++i)
 {
	reading.m_bytes[i]=buff[i+2];
 }
 printf("Client id: %d\n",buff[0]);
 printf("OpCode: %d\n",buff[1]);
 printf("Float received: %f\n",reading.m_float);
}
//Main Function
int main (int argc, const char* argv[] )
{
  unsigned long led_blink = 0;



  signal(SIGINT, sig_handler);
  printf( "%s\n", __BASEFILE__);

  if (!bcm2835_init()) {
    fprintf( stderr, "%s bcm2835_init() Failed\n\n", __BASEFILE__ );
    return 1;
  }

  printf( "RF95 CS=GPIO%d", RF_CS_PIN);

#ifdef RF_LED_PIN
  pinMode(RF_LED_PIN, OUTPUT);
  digitalWrite(RF_LED_PIN, HIGH );
#endif

#ifdef RF_IRQ_PIN
  printf( ", IRQ=GPIO%d", RF_IRQ_PIN );
  // IRQ Pin input/pull down
  pinMode(RF_IRQ_PIN, INPUT);
  bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);
  // Now we can enable Rising edge detection
  bcm2835_gpio_ren(RF_IRQ_PIN);
#endif

#ifdef RF_RST_PIN
  printf( ", RST=GPIO%d", RF_RST_PIN );
  // Pulse a reset on module
  pinMode(RF_RST_PIN, OUTPUT);
  digitalWrite(RF_RST_PIN, LOW );
  bcm2835_delay(150);
  digitalWrite(RF_RST_PIN, HIGH );
  bcm2835_delay(100);
#endif

#ifdef RF_LED_PIN
  printf( ", LED=GPIO%d", RF_LED_PIN );
  digitalWrite(RF_LED_PIN, LOW );
#endif

  if (!radio.init()) {
    fprintf( stderr, "\nRF95 module init failed, Please verify wiring/module\n" );
  } else {

    radio.channelModem(2);
    radio.setThisAddress(RF_NODE_ID);
    radio.setHeaderFrom(RF_NODE_ID);
    radio.setPromiscuous(true);
    radio.printRadioConfig();
    radio.setModeRx();
    std::ofstream myfile ("packetLog.txt");
    printf( "Listening packet...\n" );
    while (!force_exit) {

#ifdef RF_IRQ_PIN
      // We have a IRQ pin ,pool it instead reading
      // Modules IRQ registers from SPI in each loop

      // Rising edge fired ?
      if (bcm2835_gpio_eds(RF_IRQ_PIN)) {
        // Now clear the eds flag by setting it to 1
        bcm2835_gpio_set_eds(RF_IRQ_PIN);
        //printf("Packet Received, Rising event detect for pin GPIO%d\n", RF_IRQ_PIN);
#endif

        if (radio.available()) {
#ifdef RF_LED_PIN
          led_blink = millis();
          digitalWrite(RF_LED_PIN, HIGH);
#endif
          // Should be a message for us now
          uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
          uint8_t len  = sizeof(buf);
          uint8_t from = radio.headerFrom();
          uint8_t to   = radio.headerTo();
          uint8_t id   = radio.headerId();
          uint8_t flags= radio.headerFlags();;
          int8_t rssi  = radio.lastRssi();

#ifdef NO_ENCRYPT
        if(radio.recv(buf,&len)){
		printf("Len has been set to: %d\n",len);
            //printf("Packet[%02d] #%d => #%d %ddB: ", len, from, to, rssi);
            //printbuffer(buf, len);

            //TODO: check to see if byte 0 is client address and byte 1 is a water level label  

            Float waterLevel;
            waterLevel.m_bytes[0]=buf[2];
            waterLevel.m_bytes[1]=buf[3];
            waterLevel.m_bytes[2]=buf[4];
            waterLevel.m_bytes[3]=buf[5];
            printf("waterlevel m_float: %f\n",waterLevel.m_float);
            radio.printRecv(buf,len);

            if (myfile.is_open())//TODO: package data in json format for node
            {
              std::string data="Packet[";
              data+=len;
              data+="] #";
              data+=from;
              data+=" => #";
              data+=to;
              data+=" ";
              data+=rssi;
              data+="dB";
              myfile <<data;
              //myfile << "This is another line.\n";

            }
            else printf("Unable to open file");

            print_custom_buffer(buf, len);
            //generate then send an ack
            len = radio.generateAck(buf,len);

            //printf("Sending %02d bytes to node #%d => ", len, RF_GATEWAY_ID );
            printf("\n" );
              radio.send(buf,len);
            radio.waitPacketSent();

          }
          else {
            //Serial.print("receive failed");
            printf("receive failed");
          }
          printf("\n");
        }

#ifdef RF_IRQ_PIN
      }
#endif

#ifdef RF_LED_PIN
      // Led blink timer expiration ?
      if (led_blink && millis()-led_blink>200) {
        led_blink = 0;
        digitalWrite(RF_LED_PIN, LOW);
      }
#endif
      // Let OS doing other tasks
      // For timed critical appliation you can reduce or delete
      // this delay, but this will charge CPU usage, take care and monitor
      bcm2835_delay(5);
    }
    myfile.close();
  }

#ifdef RF_LED_PIN
  digitalWrite(RF_LED_PIN, LOW );
#endif
  printf( "\n%s Ending\n", __BASEFILE__ );
  bcm2835_close();
  return 0;
}
