#ifndef  _ESP8266_H_
#define  _ESP8266_H_

#include "qx_rtt.h"

enum ESP_Mode{
    AP_TCP_SERVER_MODE=0,
    AP_TCP_CLIENT_MODE,
    AP_UDP_MODE,
    STA_TCP_SERVER_MODE,
    STA_TCP_CLIENT_MODE,
    STA_UDP_MODE
};

extern enum ESP_Mode Esp_mode;

uint8_t esp8266_at_cmd_send(char *CMD ,char *ACK , uint16_t timeout);
void esp8266_data_send(char *DATA);
uint8_t esp8266_quit_trans(void);
uint8_t esp8266_ack_check(char *Cmd ,char *Str);
void esp8266_data_check(void);
void esp8266_ap_tcp_server(char *SSID , char *PWD , uint8_t CH , uint8_t ECN , char *portnum);
void esp8266_ap_tcp_client(char *SSID , char *PWD , uint8_t CH , uint8_t ECN , char * tcp_ip , char *portnum);
void esp8266_ap_udp(char *SSID , char *PWD , uint8_t CH , uint8_t ECN , char * tcp_ip , char *portnum);
void esp8266_sta_tcp_server(char *SSID , char *PWD , char *portnum);
void esp8266_sta_tcp_client(char *SSID , char *PWD , char * tcp_ip , char *portnum);
void esp8266_sta_udp(char *SSID , char *PWD , char * tcp_ip , char *portnum);
uint8_t esp8266_server_data_send(char *data_str);
void esp8266_client_data_send(char *data_str);
uint8_t esp8266_UDP_data_send(char *data_str);

#endif




