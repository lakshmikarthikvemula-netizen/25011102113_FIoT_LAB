/* Bounded polling transport; MQTT interface adapted from Can Guveren's ESPLib.
 * No interrupt receiver shares this UART. Only MQTT 3.1.1 clean-session,
 * anonymous CONNECT and QoS 0 PUBLISH are implemented. No delivery guarantee.
 */
#include "ESPLib.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
static UART_HandleTypeDef *esp_uart;
static funcState_t wait_bytes(const uint8_t *expected, size_t length, uint32_t timeout)
{
 uint32_t start=HAL_GetTick();
 size_t matched=0;
 uint8_t byte;
 while ((uint32_t)(HAL_GetTick()-start)<timeout) {
  if (HAL_UART_Receive(esp_uart,&byte,1,10)!=HAL_OK) continue;
  if (byte==expected[matched]) ++matched;
  else matched=(byte==expected[0]) ? 1U : 0U;
  if (matched==length) return funcOk;
 }
 return funcErr;
}
static funcState_t command(const char *cmd, const char *expected, uint32_t timeout)
{
 if (!esp_uart || HAL_UART_Transmit(esp_uart,(uint8_t *)cmd,(uint16_t)strlen(cmd),1000)!=HAL_OK) return funcErr;
 return wait_bytes((const uint8_t *)expected,strlen(expected),timeout);
}
static int safe_argument(const char *s)
{
 return s && !strpbrk(s,"\"\\\r\n");
}
funcState_t ESP8266_Init(espMode_t mode, UART_HandleTypeDef *uart)
{
 char cmd[24];
 if (!uart || mode<StationMode || mode>StationAP) return funcErr;
 esp_uart=uart;
 if (command("AT\r\n","OK\r\n",2000)!=funcOk || command("ATE0\r\n","OK\r\n",2000)!=funcOk) return funcErr;
 snprintf(cmd,sizeof(cmd),"AT+CWMODE=%u\r\n",(unsigned)mode);
 if (command(cmd,"OK\r\n",2000)!=funcOk) return funcErr;
 return command("AT+CIPMUX=0\r\n","OK\r\n",2000);
}
funcState_t ESP8266_wifiConnect(const char *ssid, const char *password)
{
 char cmd[160];
 if (!safe_argument(ssid)||!safe_argument(password)) return funcErr;
 int n=snprintf(cmd,sizeof(cmd),"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);
 if (n<0 || (size_t)n>=sizeof(cmd)) return funcErr;
 return command(cmd,"OK\r\n",25000);
}
funcState_t ESP8266_portConnect(const char *type, const char *host, const char *port)
{
 char cmd[160];
 if (!type || strcmp(type,"TCP") || !safe_argument(host) || !port || !*port || strspn(port,"0123456789")!=strlen(port)) return funcErr;
 int n=snprintf(cmd,sizeof(cmd),"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",host,port);
 if (n<0 || (size_t)n>=sizeof(cmd)) return funcErr;
 return command(cmd,"OK\r\n",15000);
}
static funcState_t send_packet(const uint8_t *packet, size_t length)
{
 char cmd[32];
 if (!esp_uart || length>255) return funcErr;
 snprintf(cmd,sizeof(cmd),"AT+CIPSEND=%u\r\n",(unsigned)length);
 if (command(cmd,">",3000)!=funcOk) return funcErr;
 if (HAL_UART_Transmit(esp_uart,(uint8_t *)packet,(uint16_t)length,2000)!=HAL_OK) return funcErr;
 return wait_bytes((const uint8_t *)"SEND OK\r\n",9,5000);
}
/* Encodes Remaining Length using the variable-length MQTT format. */
static size_t header(uint8_t *packet, uint8_t type, size_t remaining)
{
 size_t n=0;
 packet[n++]=type;
 do { uint8_t digit=remaining%128; remaining/=128; packet[n++]=digit|(remaining ? 0x80 : 0); } while (remaining);
 return n;
}
funcState_t MQTT_connectBroker(const MQTT_InitTypeDef_t *config)
{
 uint8_t packet[255];
 if (!config || !config->clientID || !*config->clientID || config->username || config->password || config->connectFlag!=CleanSession) return funcErr;
 size_t len=strlen(config->clientID);
 if (len>220) return funcErr;
 size_t n=header(packet,0x10,12+len);
 const uint8_t protocol[]={0,4,'M','Q','T','T',4,CleanSession};
 memcpy(packet+n,protocol,sizeof(protocol));n+=sizeof(protocol);
 packet[n++]=(uint8_t)(config->keepAlive>>8);packet[n++]=(uint8_t)config->keepAlive;
 packet[n++]=(uint8_t)(len>>8);packet[n++]=(uint8_t)len;
 memcpy(packet+n,config->clientID,len);n+=len;
 if (send_packet(packet,n)!=funcOk) return funcErr;
 /* Default ESP-AT active receive framing: +IPD,<length>:<MQTT bytes>.
  * Consume one complete frame and require an exact successful CONNACK.
  */
 if (wait_bytes((const uint8_t *)"+IPD,4:",7,5000)!=funcOk) return funcErr;
 uint8_t ack[4];
 if (HAL_UART_Receive(esp_uart,ack,4,2000)!=HAL_OK) return funcErr;
 const uint8_t success[]={0x20,2,0,0};
 return memcmp(ack,success,4)==0 ? funcOk : funcErr;
}
funcState_t MQTT_publishTopic(const char *topic, const char *message, uint8_t qos)
{
 uint8_t packet[255];
 if (!topic || !*topic || !message || qos!=QoS0 || strpbrk(topic,"+#")) return funcErr;
 size_t t=strlen(topic),m=strlen(message);
 if (t>240 || m>240 || t+m>249) return funcErr;
 size_t n=header(packet,0x30,2+t+m);
 packet[n++]=(uint8_t)(t>>8);packet[n++]=(uint8_t)t;
 memcpy(packet+n,topic,t);n+=t;memcpy(packet+n,message,m);n+=m;
 return send_packet(packet,n);
}
