/* Run with a host C compiler; no physical UART or public broker needed. */
#include "ESPLib.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static uint32_t tick;
static unsigned char response[1024],packet[255];
static size_t read_at,response_len,packet_len;
static int timeout_mode,reject_broker;
uint32_t HAL_GetTick(void) { return tick++; }
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *u,uint8_t *data,uint16_t len,uint32_t timeout)
{
 (void)u;tick+=timeout;
 if (timeout_mode || read_at+len>response_len) return HAL_TIMEOUT;
 memcpy(data,response+read_at,len);read_at+=len;return HAL_OK;
}
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *u,uint8_t *data,uint16_t len,uint32_t timeout)
{
 (void)u;(void)timeout;
 read_at=0;
 if (len>=2 && data[0]=='A' && data[1]=='T') {
  const char *s=len>10 && !memcmp(data,"AT+CIPSEND",10) ? "\r\n>" : "\r\nOK\r\n";
  response_len=strlen(s);memcpy(response,s,response_len);
 } else {
  assert(len<=sizeof(packet));memcpy(packet,data,len);packet_len=len;
  const unsigned char ack[]={'S','E','N','D',' ','O','K','\r','\n','+','I','P','D',',','4',':',0x20,2,0,0};
  response_len=sizeof(ack);memcpy(response,ack,response_len);
  if (reject_broker) response[response_len-1]=5;
 }
 return HAL_OK;
}
int main(void)
{
 UART_HandleTypeDef uart={0};
 assert(ESP8266_Init(StationMode,&uart)==funcOk);
 MQTT_InitTypeDef_t config={"test-node",NULL,NULL,60,CleanSession};
 assert(MQTT_connectBroker(&config)==funcOk);
 const unsigned char expected[]={0x10,21,0,4,'M','Q','T','T',4,2,0,60,0,9,'t','e','s','t','-','n','o','d','e'};
 assert(packet_len==sizeof(expected) && !memcmp(packet,expected,sizeof(expected)));
 reject_broker=1;assert(MQTT_connectBroker(&config)==funcErr);reject_broker=0;
 assert(MQTT_publishTopic("sensor/a","{}",QoS0)==funcOk);
 const unsigned char pub[]={0x30,12,0,8,'s','e','n','s','o','r','/','a','{','}'};
 assert(packet_len==sizeof(pub) && !memcmp(packet,pub,sizeof(pub)));
 char payload[241];memset(payload,'a',sizeof(payload)-1);payload[240]=0;
 assert(MQTT_publishTopic("sensor/a",payload,QoS0)==funcOk);
 assert(packet[1]==250 && packet[2]==1 && packet_len==253);
 assert(MQTT_publishTopic("long/topic",payload,QoS0)==funcErr);
 assert(MQTT_publishTopic("sensor/#","{}",QoS0)==funcErr);
 assert(MQTT_publishTopic("sensor/a","{}",2)==funcErr);
 assert(ESP8266_wifiConnect("bad\"ssid","x")==funcErr);
 assert(MQTT_connectBroker(NULL)==funcErr);
 timeout_mode=1;uint32_t start=tick;
 assert(MQTT_publishTopic("a","{}",QoS0)==funcErr);
 assert(tick-start<3100);
 puts("PASS: CONNECT bytes, CONNACK rejection, QoS0 bytes, multibyte Remaining Length, input bounds, finite timeout");
}
