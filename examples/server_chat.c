/*
 * server_chat.c
 *
 *  Created on: May 14, 2021
 *      Author: Stefan Ursella <stefan.ursella@wolfvision.net>
 *     License: MIT
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ezwebsocket.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

static bool stop = false;

struct app_ctx
{
  char *test_msg;
};

void sigIntHandler(int dummy)
{
  stop = true;
}

void* onOpen(void *socketUserData, struct websocket_server_desc *wsDesc,
             struct websocket_connection_desc *connectionDesc)
{
  struct app_ctx *ctx = calloc(1, sizeof(*ctx));
  printf("connection to %s opened\n", websocketServer_getPeerIp(connectionDesc));

  ctx->test_msg = strdup("foo");
  return ctx;
}

void onMessage(void *socketUserData, struct websocket_connection_desc *connectionDesc, void *userData,
               enum ws_data_type dataType, void *msg, size_t len)
{
  printf("received: %.*s\n", (int)len, (char *)msg);
  websocket_sendData(connectionDesc, dataType, msg, len);
}

void onClose(struct websocket_server_desc *wsDesc, void *socketUserData,
             struct websocket_connection_desc *connectionDesc, void *userData)
{
  struct app_ctx *ctx = (struct app_ctx *) userData;
  printf("connection to %s closed test_msg: %s\n", websocketServer_getPeerIp(connectionDesc), ctx->test_msg);
  
  assert(!strcmp(ctx->test_msg, "foo"));
  free(ctx->test_msg);
  free(ctx);
}

int main(int argc, char *argv[])
{
  struct websocket_server_init websocketInit;
  struct websocket_server_desc *wsDesc;

  signal(SIGINT, sigIntHandler);

  websocketInit.port = "9001";
  websocketInit.address = "0.0.0.0";
  websocketInit.ws_onOpen = onOpen;
  websocketInit.ws_onClose = onClose;
  websocketInit.ws_onMessage = onMessage;

  wsDesc = websocketServer_open(&websocketInit, NULL);
  if(wsDesc == NULL)
    return -1;

  while(!stop)
  {
    usleep(300000);
  }

  websocketServer_close(wsDesc);

  return 0;
}
