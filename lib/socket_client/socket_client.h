/**
 * \file      socket_client.h
 * \author    Clemens Kresser
 * \date      Jun 26, 2020
 * \copyright Copyright 2017-2021 Clemens Kresser MIT License
 * \brief     Event based socket client implementation
 *
 */
#ifndef SOCKET_CLIENT_SOCKET_CLIENT_H_
#define SOCKET_CLIENT_SOCKET_CLIENT_H_

#include <stddef.h>
#include "utils/dyn_buffer.h"

//! structure with data needed to create a socket client
struct socket_client_init
{
  //! callback that should be called when a message is received use NULL if not used
  size_t (*socket_onMessage)(void *socketUserData, void *socketDesc, void *sessionData, void *msg, size_t len);
  //! callback that should be called when the socket is connected use NULL if not used
  void* (*socket_onOpen)(void *socketUserData, void *socketDesc);
  //! callback that should be called when the socket is closed use NULL if not used
  void (*socket_onClose)(void *socketUserData, void *socketDesc, void *sessionData);
  //! the remote port we want to connect to
  unsigned short port;
  //! the address we want to connect to
  const char *address;
  //! Enable or disable keepalive
  bool keepalive;
  //! How long to wait before sending out the first probe on an idle connection
  int keep_idle_sec;
  //! The number of unanswered probes required to force closure of the socket
  int keep_cnt;
  //! The frequency of keepalive packets after the first one is sent
  int keep_intvl;
  int secure;
};

int socketClient_send(void *socketDescriptor, void *msg, size_t len);
void socketClient_start(void *socketDescriptor);
void *socketClient_open(struct socket_client_init *socketInit, void *socketUserData);
void socketClient_close(void *socketDescriptor);
void socketClient_closeConnection(void *socketDescriptor);

#endif /* SOCKET_CLIENT_SOCKET_CLIENT_H_ */
