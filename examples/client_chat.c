/*
 * client_autobahn.c
 *
 *  Created on: Nov 19, 2020
 *      Author: Clemens Kresser
 *      License: MIT
 */

#define _GNU_SOURCE

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <ezwebsocket.h>
#include <ezwebsocket_log.h>

static bool stop = false;

void sigIntHandler(int dummy)
{
  stop = true;
}

void* onOpen(void *socketUserData, struct websocket_connection_desc *connectionDesc)
{
  return NULL;
}

void onMessage(void *socketUserData, struct websocket_connection_desc *connectionDesc, void *userData,
               enum ws_data_type dataType, void *msg, size_t len)
{
	if(dataType == WS_DATA_TYPE_TEXT)
	  printf("TEXT resp: %.*s\n", (int)len, (char *)msg);
	else
	{
	  int i;
	  printf("resp: ");
	  for(i = 0; i < len; i++)
	    printf("0x%X ", ((char *)msg)[i] & 0xFF);
	  printf("\n");
	}
}

void onClose(void *socketUserData, struct websocket_connection_desc *connectionDesc, void *userData)
{
	printf("connection closed\n");
}

/**
 * \brief This runs the test as required by the autobahn testsuite
 */
void runTest(void)
{
  struct websocket_client_init websocketInit = { 0 };
  struct websocket_connection_desc *wsConnectionDesc;
  char msg[4] = { 0x08, 0xCB, 0x00, 0x00 };
  

  int i = 12, rc = 0;

  websocketInit.port = "443";
  websocketInit.address = "192.168.200.12";
  websocketInit.ws_onOpen = onOpen;
  websocketInit.ws_onClose = onClose;
  websocketInit.ws_onMessage = onMessage;
  websocketInit.endpoint = strdup("/xxx");;
  websocketInit.keepalive = true;
  websocketInit.keep_idle_sec = 10;
  websocketInit.keep_cnt = 3;
  websocketInit.keep_intvl = 10;
  websocketInit.secure = 1;

  wsConnectionDesc = websocketClient_open(&websocketInit, NULL);
  do
  {
      if(i-- < 0)
      {
        rc = websocket_sendData(wsConnectionDesc, WS_DATA_TYPE_BINARY, msg, sizeof(msg));
	i = 12;
      }
      sleep(1);

  }while(websocketConnection_isConnected(wsConnectionDesc) && !stop && !rc);

  websocketClient_close(wsConnectionDesc, WS_CLOSE_CODE_NORMAL);
}

/**
 * \brief The main function
 */
int main(int argc, char *argv[])
{
  ezwebsocket_set_level(EZLOG_DEBUG);
  websocket_init();  
  signal(SIGINT, sigIntHandler);
  runTest();
}
