/*
 * ESPLib.c
 *
 *  Created on: Feb 14, 2022
 *      Author: Can Güveren
 */

#include "ESPLib.h"

uint8_t uartRxData = 0;
uint16_t DataCounter = 0;
volatile uint8_t RxInterruptFlag = 0;
volatile uint8_t uartTimeCounter = 0;
volatile uint8_t uartPacketComplatedFlag = 0;

volatile uint8_t temp_mqttMsgLen = 0;
volatile uint8_t temp_mqttTopicLen = 0;

char Buffer[BUFFERSIZE];
char rxBuffer[BUFFERSIZE];
char mqttBuffer[BUFFERSIZE];
char mqttPacket[BUFFERSIZE];
/* Keep the same UART handle that CubeMX passes to HAL_UART_IRQHandler. */
static UART_HandleTypeDef *esp_uart;

funcState_t checkResponse(const char *response)
{
    size_t length = strlen(Buffer);
    size_t expected = strlen(response);

    return expected && length >= expected &&
           memcmp(Buffer + length - expected, response, expected) == 0
               ? funcOk : funcErr;
}

static funcState_t sendData(const char *data, size_t length, const char *reply, uint32_t timeout)
{
    size_t used = 0;
    uint32_t started;

    Buffer[0] = '\0';
    if (HAL_UART_Transmit(esp_uart, (const uint8_t *)data, length, 100) != HAL_OK)
        return funcErr;

    started = HAL_GetTick();
    while (HAL_GetTick() - started < timeout)
    {
        uint8_t byte;
        if (HAL_UART_Receive(esp_uart, &byte, 1, 10) != HAL_OK)
            continue;
        if (byte == '\0')
        {
            used = 0;
            Buffer[0] = '\0';
            continue;
        }
        if (used == sizeof(Buffer) - 1)
        {
            /* Preserve enough of the suffix to match a reply across the boundary. */
            size_t keep = sizeof("SEND OK\r\n") - 1;
            memmove(Buffer, Buffer + used - keep, keep);
            used = keep;
        }
        Buffer[used++] = (char)byte;
        Buffer[used] = '\0';
        if (checkResponse(reply) == funcOk)
            return funcOk;
        if (checkResponse("ERROR\r\n") == funcOk ||
            checkResponse("FAIL\r\n") == funcOk)
            return funcErr;
    }

    return funcErr;
}

static funcState_t sendCommand(const char *command, uint32_t timeout)
{
    return sendData(command, strlen(command), "OK\r\n", timeout);
}

funcState_t ESP8266_Init(espMode_t mode, UART_HandleTypeDef *uart)
{
    char command[20];

    esp_uart = uart;
    if (sendCommand("AT\r\n", 2000) != funcOk ||
        sendCommand("ATE0\r\n", 2000) != funcOk)
        return funcErr;

    snprintf(command, sizeof(command), "AT+CWMODE=%d\r\n", mode);
    return sendCommand(command, 2000);
}

funcState_t ESP8266_Reset(void)
{
    return sendCommand("AT+RST\r\n", 2000);
}

funcState_t ESP8266_wifiConnect(char *ssid, char *password)
{
    char command[128];
    int length;

    if (strpbrk(ssid, "\"\\,\r\n") || strpbrk(password, "\"\\,\r\n"))
        return funcErr;
    length = snprintf(command, sizeof(command), "AT+CWJAP=\"%s\",\"%s\"\r\n",
                      ssid, password);
    if (length < 0 || (size_t)length >= sizeof(command))
        return funcErr;

    return sendCommand(command, 25000);
}

funcState_t ESP8266_portConnect(char *type, char *remoteIP, char *remotePort)
{
    char command[128];
    int length = snprintf(command, sizeof(command), "AT+CIPSTART=\"%s\",\"%s\",%s\r\n",
                          type, remoteIP, remotePort);

    if (length < 0 || (size_t)length >= sizeof(command))
        return funcErr;
    sendCommand("AT+CIPCLOSE\r\n", 2000);
    if (sendCommand("AT+CIPMODE=0\r\n", 2000) != funcOk ||
        sendCommand("AT+CIPMUX=0\r\n", 2000) != funcOk)
        return funcErr;
    return sendCommand(command, 50000);
}

funcState_t ESP8266_sendMessage(char *msg, uint8_t msgSize)
{
    char command[24];
    int length = snprintf(command, sizeof(command), "AT+CIPSEND=%u\r\n", (unsigned)msgSize);
    funcState_t result;

    if (HAL_UART_AbortReceive(esp_uart) != HAL_OK)
        return funcErr;
    result = sendData(command, length, ">", 3000);
    if (result == funcOk)
        result = sendData(msg, msgSize, "SEND OK\r\n", 5000);
    if (HAL_UART_Receive_IT(esp_uart, &uartRxData, 1) != HAL_OK)
        return funcErr;
    return result;
}

/******************************************************************************************************************************/
/********************************************************MQTT Functions********************************************************/
/******************************************************************************************************************************/

funcState_t MQTT_connectBroker(MQTT_InitTypeDef_t *MQTTConnect)
{
	char *protocolName = "MQTT";

	uint16_t protocolLenght  = strlen(protocolName); //protocolLenght 2 byte. "MQTT" 4 byte.
	size_t clientIDLenght = strlen(MQTTConnect->clientID);

	int mqttLenght;

	memset(mqttPacket, 0, sizeof(mqttPacket));

	if(MQTTConnect->connectFlag == (PasswordFlag | UserNameFlag))
	{
		uint16_t usernameLenght = strlen(MQTTConnect->username);
		uint16_t passwordLenght = strlen(MQTTConnect->password);
		uint8_t remainingLenght = 2 + protocolLenght + 4 + 2 + clientIDLenght + 2 + usernameLenght + 2 + passwordLenght;

		mqttLenght = snprintf(mqttPacket, sizeof(mqttPacket), "%c%c%c%c%s%c%c%c%c%c%c%s%c%c%s%c%c%s", (char)MQTTconnect, (char)remainingLenght, (char)(protocolLenght >> 8), (char)(protocolLenght & 0x00FF),
							 protocolName,(char)MQTTlevel,(char)MQTTConnect->connectFlag, (char)(MQTTConnect->keepAlive >> 8), (char)MQTTConnect->keepAlive, (char)(clientIDLenght >> 8), (char)(clientIDLenght & 0x00FF),
							 MQTTConnect->clientID, (char)(usernameLenght >> 8), (char)usernameLenght, MQTTConnect->username, (char)(passwordLenght >> 8), (char)passwordLenght, MQTTConnect->password);
	}
	else if(MQTTConnect->connectFlag == PasswordFlag)
	{
		uint16_t passwordLenght = strlen(MQTTConnect->password);
		uint8_t remainingLenght = 2 + protocolLenght + 4 + 2 + clientIDLenght + 2 + passwordLenght;

		mqttLenght = snprintf(mqttPacket, sizeof(mqttPacket), "%c%c%c%c%s%c%c%c%c%c%c%s%c%c%s", (char)MQTTconnect, (char)remainingLenght, (char)(protocolLenght >> 8), (char)(protocolLenght & 0x00FF),
							 protocolName,(char)MQTTlevel,(char)MQTTConnect->connectFlag, (char)(MQTTConnect->keepAlive >> 8), (char)MQTTConnect->keepAlive, (char)(clientIDLenght >> 8), (char)(clientIDLenght & 0x00FF),
							 MQTTConnect->clientID, (char)(passwordLenght >> 8), (char)passwordLenght, MQTTConnect->password);
	}
	else if(MQTTConnect->connectFlag == UserNameFlag)
	{
		uint16_t usernameLenght = strlen(MQTTConnect->username);
		uint8_t remainingLenght = 2 + protocolLenght + 4 + 2 + clientIDLenght + 2 + usernameLenght;

		mqttLenght = snprintf(mqttPacket, sizeof(mqttPacket), "%c%c%c%c%s%c%c%c%c%c%c%s%c%c%s", (char)MQTTconnect, (char)remainingLenght, (char)(protocolLenght >> 8), (char)(protocolLenght & 0x00FF),
							 protocolName,(char)MQTTlevel,(char)MQTTConnect->connectFlag, (char)(MQTTConnect->keepAlive >> 8), (char)MQTTConnect->keepAlive, (char)(clientIDLenght >> 8), (char)(clientIDLenght & 0x00FF),
							 MQTTConnect->clientID, (char)(usernameLenght >> 8), (char)usernameLenght, MQTTConnect->username);
	}
	else
	{
		uint8_t remainingLenght = 2 + protocolLenght + 4 + 2 + clientIDLenght;

		mqttLenght = snprintf(mqttPacket, sizeof(mqttPacket), "%c%c%c%c%s%c%c%c%c%c%c%s", (char)MQTTconnect, (char)remainingLenght, (char)(protocolLenght >> 8), (char)(protocolLenght & 0x00FF),
							 protocolName,(char)MQTTlevel,(char)MQTTConnect->connectFlag, (char)(MQTTConnect->keepAlive >> 8), (char)MQTTConnect->keepAlive, (char)(clientIDLenght >> 8), (char)(clientIDLenght & 0x00FF),
							 MQTTConnect->clientID);
	}

	if (mqttLenght <= 0 || (size_t)mqttLenght >= sizeof(mqttPacket))
        return funcErr;
    return ESP8266_sendMessage(mqttPacket, mqttLenght);

}

funcState_t MQTT_publishTopic(char *topic, char *msg, uint8_t qos)
{
    size_t topic_length = strlen(topic);
    size_t remaining = 2 + topic_length + strlen(msg);
    int packet_length;

    if (qos != QoS0 || remaining > 127 || remaining + 2 >= sizeof(mqttPacket))
        return funcErr;

    packet_length = snprintf(mqttPacket, sizeof(mqttPacket), "%c%c%c%c%s%s",
                             MQTTpublish, (int)remaining, (int)(topic_length >> 8),
                             (int)(topic_length & 0xff), topic, msg);
    if (packet_length <= 0 || (size_t)packet_length >= sizeof(mqttPacket))
        return funcErr;

    return ESP8266_sendMessage(mqttPacket, packet_length);
}

void MQTT_subscribeTopic(char *topic, uint8_t QoS)
{
	uint8_t mqttLenght;

	uint16_t topicLenght = strlen(topic);

	uint8_t remainingLenght = 5 + topicLenght;

	uint16_t packetID = 0x01;
	memset(mqttPacket, 0, sizeof(mqttPacket));

	mqttLenght = sprintf(mqttPacket, "%c%c%c%c%c%c%s%c", (char)MQTTsubscribe, (char)remainingLenght, (char)packetID >> 8, (char)packetID, (char)topicLenght >> 8, (char)topicLenght, topic, QoS);

	ESP8266_sendMessage(mqttPacket, mqttLenght);
}


void MQTT_unsubscribeTopic(char *topic)
{
	uint8_t mqttLenght;

	uint16_t topicLenght = strlen(topic);

	uint8_t remainingLenght = 4 + topicLenght;

	uint16_t packetID = 0x01;

	memset(mqttPacket, 0, 100);

	mqttLenght = sprintf(mqttPacket, "%c%c%c%c%c%c%s", (char)MQTTunsub, (char)remainingLenght, (char)packetID >> 8, (char)packetID, (char)topicLenght >> 8, (char)topicLenght, topic);

	ESP8266_sendMessage(mqttPacket, mqttLenght);
}

void MQTT_disconnectBroker(void)
{
	uint8_t mqttLenght;
	uint8_t remainingLenght = 0;

	memset(mqttPacket, 0, 100);

	mqttLenght = sprintf(mqttPacket, "%c%c", (char)MQTTdisconnect, (char)remainingLenght);

	ESP8266_sendMessage(mqttPacket, mqttLenght);

}

void MQTT_pingReq()
{
	uint8_t mqttLenght;
	uint8_t remainingLenght = 0;

	memset(mqttPacket, 0, 100);

	mqttLenght = sprintf(mqttPacket, "%c%c", (char)MQTTpingreq, (char)remainingLenght);

	ESP8266_sendMessage(mqttPacket, mqttLenght);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart != esp_uart)
        return;
    RxInterruptFlag = SET;
    if (DataCounter < sizeof(mqttBuffer))
        mqttBuffer[DataCounter++] = uartRxData;

    HAL_UART_Receive_IT(esp_uart, &uartRxData, 1);
    uartTimeCounter = 0;
}


void MQTTDataHandler(char *mqttTopic, char *mqttMsg)
{
	uint8_t mqttMsgLen, digit_mqttMsgLen;
	uint16_t mqttTopicLen;
	uint8_t colonIndex = 0;
	uint8_t i;
	uint8_t digitNumber = 0;

	if(uartPacketComplatedFlag == SET)     //Data receiving is finished
	{
		uartPacketComplatedFlag = RESET;

		for(i = 0; i < DataCounter; i++)
		{
			if(mqttBuffer[i] == ':')
			{
				colonIndex = i;
				break;
			}
		}

		if (i == DataCounter || colonIndex + 2 >= DataCounter)
		{
			DataCounter = 0;
			return;
		}

		memset(mqttTopic, 0, temp_mqttTopicLen);
		memset(mqttMsg, 0, temp_mqttMsgLen);

		mqttMsgLen = mqttBuffer[colonIndex + 2];
		digit_mqttMsgLen = mqttMsgLen + 2;

	    while(digit_mqttMsgLen > 0){
	    	digit_mqttMsgLen = digit_mqttMsgLen / 10;
	        digitNumber++;
	    }

		mqttTopicLen = (mqttBuffer[10 + digitNumber] << 8 ) | (mqttBuffer[11 + digitNumber]);
		memcpy(mqttMsg, &mqttBuffer[15 + digitNumber], mqttMsgLen - mqttTopicLen - 2);
		memcpy(mqttTopic, &mqttBuffer[12 + digitNumber], mqttTopicLen);
		memset(mqttBuffer, 0, DataCounter);

		temp_mqttMsgLen = mqttMsgLen;
		temp_mqttTopicLen = mqttTopicLen;
		DataCounter = 0;

	}
}

/* This function should be called in systick timer */
void MQTTTimer(void)
{
	if(RxInterruptFlag == SET)
	{
		if(uartTimeCounter++ > 100)
		{

			RxInterruptFlag = RESET;
			uartTimeCounter = 0;
			uartPacketComplatedFlag = SET;
		}
	}
}

