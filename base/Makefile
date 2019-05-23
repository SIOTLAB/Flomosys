# Makefile
# Sample for RH_RF95 (client and server) on Raspberry Pi
# Caution: requires bcm2835 library to be already installed
# http://www.airspayce.com/mikem/bcm2835/

CC            = g++
CFLAGS        = -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY -D__BASEFILE__=\"$*\"
LIBS          = -l bcm2835
RADIOHEADBASE = ../lib/RadioHead
CRYPTOBASE    = ../lib/arduinolibs-master/libraries/Crypto
DRIVERBASE		= ../node
INCLUDE       = -I$(RADIOHEADBASE)

default: rf95_server

RasPi.o: $(RADIOHEADBASE)/RHutil/RasPi.cpp
				$(CC) $(CFLAGS) -c $(RADIOHEADBASE)/RHutil/RasPi.cpp $(INCLUDE)

rf95_server.o: rf95_server.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

LoraDriver.o: $(DRIVERBASE)/LoraDriver.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

modem.o: $(DRIVERBASE)/modem.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<


RH_RF95.o: $(RADIOHEADBASE)/RH_RF95.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

RHDatagram.o: $(RADIOHEADBASE)/RHDatagram.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

RHHardwareSPI.o: $(RADIOHEADBASE)/RHHardwareSPI.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

RHSPIDriver.o: $(RADIOHEADBASE)/RHSPIDriver.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

RHGenericDriver.o: $(RADIOHEADBASE)/RHGenericDriver.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

RHGenericSPI.o: $(RADIOHEADBASE)/RHGenericSPI.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

rf95_server: rf95_server.o LoraDriver.o RH_RF95.o RasPi.o RHHardwareSPI.o RHGenericDriver.o RHGenericSPI.o RHSPIDriver.o modem.o 
				$(CC) $^ $(LIBS) -o rf95_server

clean:
				rm -rf *.o rf95_server
