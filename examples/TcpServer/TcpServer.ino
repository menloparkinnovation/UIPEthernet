
//
// MenloParkInnovation LLC Fork:
//
// 05/21/2019
//
// - Changed addressing for local network.
// - Changed MAC addressing to be independent for TcpClient, TcpServer
// - Changed IP addressing to match TcpClient and TcpServer and debugged.
// - Changed to DHCP for TcpServer.
// - Added logging print out of assigned DHCP addresses for TcpServer.
// - Support for F32C on FPGAArduino, a 32 bit MIPS/RISC-V soft core running on FPGA's
//   https://github.com/f32c/f32c
//   http://www.nxlab.fer.hr/fpgarduino/
//

/*
 * UIPEthernet TCPServer example.
 *
 * UIPEthernet is a TCP/IP stack that can be used with a enc28j60 based
 * Ethernet-shield.
 *
 * UIPEthernet uses the fine uIP stack by Adam Dunkels <adam@sics.se>
 *
 *      -----------------
 *
 * This Hello World example sets up a server at 192.168.1.6 on port 1000.
 * Telnet here to access the service.  The uIP stack will also respond to
 * pings to test if you have successfully established a TCP connection to
 * the Arduino.
 *
 * This example was based upon uIP hello-world by Adam Dunkels <adam@sics.se>
 * Ported to the Arduino IDE by Adam Nielsen <malvineous@shikadi.net>
 * Adaption to Enc28J60 by Norbert Truchsess <norbert.truchsess@t-online.de>
 */

//
// Menlo definitions:
//
#define MACADDRESS 0x00,0x01,0x02,0x03,0x04,0x06 // TcpClient ends in 0x05

#define TCPSERVER_PORT 5000 // Server and client should agree on port

#define UARTBAUD 9600       // same as TcpClient, makes it easy with Arduino IDE default settings

// #define MACADDRESS 0x00,0x01,0x02,0x03,0x04,0x05
// #define MYIPADDR 192,168,1,6
// #define MYIPMASK 255,255,255,0
// #define MYDNS 192,168,1,1
// #define MYGW 192,168,1,1
// #define LISTENPORT 1000
// #define UARTBAUD 115200


#if defined(__MBED__)
  #include <mbed.h>
  #include "mbed/millis.h"
  #define delay(x) wait_ms(x)
  #define PROGMEM
  #include "mbed/Print.h"
#endif

#include <UIPEthernet.h>
#include "utility/logging.h"

EthernetServer server = EthernetServer(TCPSERVER_PORT);

#if defined(ARDUINO)
void setup() {
#endif  
#if defined(__MBED__)
int main() {
#endif
  #if ACTLOGLEVEL>LOG_NONE
    #if defined(ARDUINO)
      LogObject.begin(UARTBAUD);
    #endif
    #if defined(__MBED__)
      Serial LogObject(SERIAL_TX,SERIAL_RX);
      LogObject.baud(UARTBAUD);
    #endif
  #endif

  uint8_t mac[6] = {MACADDRESS};
  uint8_t myIP[4] = {MYIPADDR};
  uint8_t myMASK[4] = {MYIPMASK};
  uint8_t myDNS[4] = {MYDNS};
  uint8_t myGW[4] = {MYGW};

  //  Ethernet.begin(mac,myIP);
  // Menlo: Use DHCP  Ethernet.begin(mac,myIP,myDNS,myGW,myMASK);

  Ethernet.begin(mac); // Menlo: Use DHCP

  #if ACTLOGLEVEL>=LOG_INFO
    LogObject.uart_send_str(F("localIP: "));
    #if defined(ARDUINO)
      LogObject.println(Ethernet.localIP());
    #endif
    #if defined(__MBED__)
      LogObject.printf("%d.%d.%d.%d",Ethernet.localIP()[0],Ethernet.localIP()[1],Ethernet.localIP()[2],Ethernet.localIP()[3]);
    #endif
    LogObject.uart_send_str(F("subnetMask: "));
    #if defined(ARDUINO)
      LogObject.println(Ethernet.subnetMask());
    #endif
    #if defined(__MBED__)
      LogObject.printf("%d.%d.%d.%d",Ethernet.subnetMask()[0],Ethernet.subnetMask()[1],Ethernet.subnetMask()[2],Ethernet.subnetMask()[3]);
    #endif
    LogObject.uart_send_str(F("gatewayIP: "));
    #if defined(ARDUINO)
      LogObject.println(Ethernet.gatewayIP());
    #endif
    #if defined(__MBED__)
      LogObject.printf("%d.%d.%d.%d",Ethernet.gatewayIP()[0],Ethernet.gatewayIP()[1],Ethernet.gatewayIP()[2],Ethernet.gatewayIP()[3]);
    #endif
    LogObject.uart_send_str(F("dnsServerIP: "));
    #if defined(ARDUINO)
      LogObject.println(Ethernet.dnsServerIP());
    #endif
    #if defined(__MBED__)
      LogObject.printf("%d.%d.%d.%d",Ethernet.dnsServerIP()[0],Ethernet.dnsServerIP()[1],Ethernet.dnsServerIP()[2],Ethernet.dnsServerIP()[3]);
    #endif
  #endif

  server.begin();
#if defined(ARDUINO)
}

void loop() {
#endif  

#if defined(__MBED__)
while(true) {
#endif
  size_t size;

  if (EthernetClient client = server.available())
    {
      #if ACTLOGLEVEL>=LOG_INFO
        LogObject.uart_send_str(F("DATA from Client:'"));
      #endif
      while((size = client.available()) > 0)
        {
          uint8_t* msg = (uint8_t*)malloc(size+1);
          memset(msg, 0, size+1);
          size = client.read(msg,size);
          #if ACTLOGLEVEL>=LOG_INFO
            #if defined(ARDUINO)
              LogObject.write(msg,size);
            #endif
            #if defined(__MBED__)
              LogObject.uart_send_str(msg);
            #endif
          #endif
          free(msg);
        }
      #if ACTLOGLEVEL>=LOG_INFO
        LogObject.uart_send_strln(F("'"));
      #endif
      client.stop();
    }
}
#if defined(__MBED__)
}
#endif
