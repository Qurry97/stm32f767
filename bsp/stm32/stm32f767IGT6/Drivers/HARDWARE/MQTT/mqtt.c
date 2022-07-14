
#include "qx_rtt.h"


/************************************************************
 * Fuction:MQTT配置参数
 * wifi_id:esp8266连接的WiFi名称
 * wifi_pw：esp8266连接的WiFi密码
 * *********************************************************/
uint8_t mqtt_init(char *wifi_id, char * wifi_pw)
{
    char *pt = (char *)rt_malloc(128);

    if(esp8266_at_cmd_send("AT+CWMODE=3", "OK" , 100)){
        printf("esp8266 mode setting fail\r\n");
        if(pt != RT_NULL){
		    rt_free((void *)pt);
	    }
        return 1;
    }

    esp8266_at_cmd_send("AT+RST", "OK" , 50);
    rt_thread_mdelay(3000);
    
    if(esp8266_at_cmd_send("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"", "OK" , 100)){
        printf("open SNTP server fail\r\n");
        if(pt != RT_NULL){
		    rt_free((void *)pt);
	    }
        return 1;
    }  

    sprintf(pt,"AT+CWJAP=\"%s\",\"%s\"",wifi_id,wifi_pw);
    if(esp8266_at_cmd_send(pt, "OK" , 500)){
        printf("esp8266 link WIFI fail\r\n");
        if(pt != RT_NULL){
		    rt_free((void *)pt);
	    }
        return 1;
    }

    if(pt != RT_NULL){
		rt_free((void *)pt);
	}

    return 0;   
}


/************************************************************
 * Fuction:MQTT连接阿里云
 * host_url、port：阿里云创建设备生成的MQTT连接参数host和端口号。
 * client_id、user_name、device_pw：阿里云创建设备生成的MQTT连接参数id、用户名、密码.
 * *********************************************************/
void mqtt_connect_aliyun(char * host_url, char *port, char * client_id, char * user_name, char * device_pw)
{
    char *pt = (char *)rt_malloc(164);

	sprintf(pt, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"",client_id,user_name,device_pw);
		if(esp8266_at_cmd_send(pt, "OK" , 300)){
			printf("MQTT user config fail\r\n");
			if(pt != RT_NULL){
				rt_free((void *)pt);
			}
		return;
    }

    if(esp8266_at_cmd_send("AT+MQTTCONNCFG=0,120,0,\"\",\"\",0,1", "OK" , 300)){
        printf("MQTT set config fail\r\n");
        if(pt != RT_NULL){
		    rt_free((void *)pt);
			return ;
	    }
    }
	
	memset(pt ,'\0', 164);
    sprintf(pt, "AT+MQTTCONN=0,\"%s\",%s,0",host_url,port);
    if(esp8266_at_cmd_send(pt, "OK" , 300)){
        printf("MQTT connect fail\r\n");
    }

    if(pt != RT_NULL){
		rt_free((void *)pt);
	}
}


/************************************************************
 * Fuction:MQTT断开连接阿里云
 * *********************************************************/
void mqtt_close_connect()
{
    if(esp8266_at_cmd_send("AT+MQTTCLEAN=0", "OK" , 100)==0){
        printf("MQTT connect close!\r\n");
    }
}



/************************************************
 * Fuction:MQTT订阅topic
 * *********************************************/
void mqtt_sub_topic(char * Topic)
{
    char *pt = (char *)rt_malloc(100);
    sprintf(pt, "AT+MQTTSUB=0,\"%s\",1",Topic);
    esp8266_at_cmd_send(pt, "OK" , 100);

    if(pt != RT_NULL){
		rt_free((void *)pt);
	}
}



/************************************************
 * Fuction:MQTT取消订阅topic
 * *********************************************/
void mqtt_unsub_topic(char * Topic)
{
    char *pt = (char *)rt_malloc(100);
    sprintf(pt, "AT+MQTTUNSUB=0,\"%s\"",Topic);
    esp8266_at_cmd_send(pt, "OK" , 100);

    if(pt != RT_NULL){
		rt_free((void *)pt);
	}
}



/************************************************
 * Fuction:MQTT发布topic
 * *********************************************/
void mqtt_publish_topic(char * Topic ,char * Data)
{
    char *pt = (char *)rt_malloc(164);
    sprintf(pt,"AT+MQTTPUB=0,\"%s\",\"{\\\"id\\\":135789\\,\\\"params\\\":{\\\"aliyun_qmttdata:RoomTemp\\\":%s}\\,%s\\,%s}\",1,0",Topic,Data,VERSION,METHOD);
    esp8266_at_cmd_send(pt, "OK" , 300);

    if(pt != RT_NULL){
		rt_free((void *)pt);
	}
}











