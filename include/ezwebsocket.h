/* EzWebSocket
 *
 * Copyright © 2017 Clemens Kresser
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef WEBSOCKET_H_
#define WEBSOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

//! the 2 different websocket data types
enum ws_data_type {
  //! websocket data type text
  WS_DATA_TYPE_TEXT,
  //! websocket data type binary
  WS_DATA_TYPE_BINARY,
};

//! the websocket close codes
enum ws_close_code {
  //! Successful operation regular socket shutdown
  WS_CLOSE_CODE_NORMAL = 1000,
  //! Client is leaving -> browser tab closing
  WS_CLOSE_CODE_GOING_AWAY = 1001,
  //! Endpoint received a malformed frame
  WS_CLOSE_CODE_PROTOCOL_ERROR = 1002,
  //! endpoint received an unsupported frame
  //! e.g. text on a binary only endpoint
  WS_CLOSE_CODE_UNACCEPTABLE_OPCODE = 1003,
  //! Reserved A meaning might be defined in the future.
  WS_CLOSE_CODE_RESERVED_0 = 1004,
  //! Reserved Indicates that no status code was provided even though one was expected.
  WS_CLOSE_CODE_RESERVED_1 = 1005,
  //! Reserved. Used to indicate that a connection was closed abnormally
  //! (that is, with no close frame being sent) when a status code is expected.
  WS_CLOSE_CODE_RESERVED_2 = 1006,
  //! The endpoint is terminating the connection because a message was received that contained
  //! inconsistent data (e.g., non-UTF-8 data within a text message).
  WS_CLOSE_CODE_INVALID_DATA = 1007,
  //! The endpoint is terminating the connection because it received a message that violates its
  //! policy. This is a generic status code, used when codes 1003 and 1009 are not suitable.
  WS_CLOSE_CODE_POLICY_VIOLATION = 1008,
  //! The endpoint is terminating the connection because a data frame was received that is too
  //! large.
  WS_CLOSE_CODE_MSG_TO_BIG = 1009,
  //! The client is terminating the connection because it expected the server to negotiate one or
  //! more extension, but the server didn't.
  WS_CLOSE_CODE_CLIENT_EXTENSION_UNKNOWN = 1010,
  //! The server is terminating the connection because it encountered an unexpected condition that
  //! prevented it from fulfilling the request.
  WS_CLOSE_CODE_UNEXPECTED_COND = 1011,
  //! Reserved. Indicates that the connection was closed due to a failure to perform a TLS handshake
  //! (e.g., the server certificate can't be verified).
  WS_CLOSE_CODE_RESERVED_3 = 1015,
};

//! descriptor for the websocket server
struct websocket_server_desc;
//! descriptor for the websocket connection
struct websocket_connection_desc;

//! structure to configure a websocket server socket
struct websocket_server_init {
  //! callback that is called when a message is received
  void (*ws_onMessage)(void *websocketUserData, struct websocket_connection_desc *connectionDesc,
                       void *connectionUserData, enum ws_data_type dataType, void *msg, size_t len);
  //! callback that is called when a new connection is established
  void *(*ws_onOpen)(void *websocketUserData, struct websocket_server_desc *wsDesc,
                     struct websocket_connection_desc *connectionDesc);
  //! callback that is called when a connection is closed
  void (*ws_onClose)(struct websocket_server_desc *wsDesc, void *websocketUserData,
                     struct websocket_connection_desc *connectionDesc, void *connectionUserData);
  //! the listening address
  const char *address;
  //! the listening port
  const char *port;
};

//! structure to configure a websocket client socket
struct websocket_client_init {
  //! callback that is called when a message is received
  void (*ws_onMessage)(void *websocketUserData, struct websocket_connection_desc *connectionDesc,
                       void *connectionUserData, enum ws_data_type dataType, void *msg, size_t len);
  //! callback that is called when a new connection is established
  void *(*ws_onOpen)(void *websocketUserData, struct websocket_connection_desc *connectionDesc);
  //! callback that is called when a connection is closed
  void (*ws_onClose)(void *websocketUserData, struct websocket_connection_desc *connectionDesc,
                     void *connectionUserData);
  //! the address of the remote target
  const char *address;
  //! the port of the remote target
  const char *port;
  //! the hostname that should be used
  const char *hostname;
  //! the endpoint of the remote (e.g. /chat)
  const char *endpoint;
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

//! structure to configure a websocket server socket
//! NOTE  LEGACY!! this define is for backward compatibility
//!       it should not be used anymore but will not be
//!       removed unless there's a good reason
//! use struct websocket_server_init
struct websocket_init {
  //! callback that is called when a message is received
  void (*ws_onMessage)(void *websocketUserData, void *clientDesc, void *clientUserData,
                       enum ws_data_type dataType, void *msg, size_t len);
  //! callback that is called when a new connection is established
  void *(*ws_onOpen)(void *wsDesc, void *clientDesc);
  //! callback that is called when a connection is closed
  void (*ws_onClose)(void *socketUserData, void *clientDesc, void *userData);
  //! the listening address
  char *address;
  //! the listening port
  char *port;
};

/**
 * \brief Returns the ipv4 address of the client
 *
 * \param *wsConnectionDesc Pointer to the websocket client descriptor
 *
 */
const char *
websocketServer_getPeerIp(struct websocket_connection_desc *wsConnectionDesc);

/**
 * \brief Returns the ipv4 address of the server interface
 *
 * \param *wsConnectionDesc Pointer to the websocket client descriptor
 *
 */
const char *
websocketServer_getServerIp(struct websocket_connection_desc *wsConnectionDesc);

/**
 * \brief Returns the user data of the given client
 *
 * \param *wsConnectionDesc Pointer to the websocket client descriptor
 *
 * \return Pointer to the websocket user data
 *
 */
void *
websocket_getConnectionUserData(struct websocket_connection_desc *wsConnectionDesc);

/**
 * \brief Opens a websocket server
 *
 * \param *wsInit Pointer to the init struct
 * \param *websocketUserData userData for the socket
 *
 * \return the websocket descriptor or NULL in case of error
 */
struct websocket_server_desc *
websocketServer_open(struct websocket_server_init *wsInit, void *websocketUserData);

/**
 * \brief Opens a websocket client connection
 *
 * \param *wsInit Pointer to the init struct
 * \param *websocketUserData UserData for the socket
 *
 * \return the websocket connection descriptor or NULL in case of error
 */
struct websocket_connection_desc *
websocketClient_open(struct websocket_client_init *wsInit, void *websocketUserData);

/**
 * \brief Closes the given websocket
 *
 * \param *wsDesc Pointer to the websocket descriptor
 *
 */
void
websocketServer_close(struct websocket_server_desc *wsDesc);

/**
 * \brief Closes a websocket client
 *
 * \param *wsConnectionDesc Pointer to the websocket client descriptor
 * \param code The closing code
 */
void
websocketClient_close(struct websocket_connection_desc *wsConnectionDesc, enum ws_close_code code);

/**
 * \brief Returns if the connection is still connected
 *
 * \param *wsConnectionDesc Pointer to the websocket connection descriptor
 *
 * \return true => connected else false
 */
bool
websocketConnection_isConnected(struct websocket_connection_desc *wsConnectionDesc);

/**
 * \brief Increments the reference count of the given object
 *
 * \param *ptr poiner to the object
 */
void
websocket_ref(void *ptr);

/**
 * \brief Decrements the reference count of the given object and frees it if necessary
 *
 * \param *ptr: poiner to the object
 */
void
websocket_unref(void *ptr);

/**
 * \brief Sends binary or text data through websockets
 *
 * \param *wsConnectionDesc Pointer to the websocket connection descriptor
 * \param dataType the datatype (WS_DATA_TYPE_BINARY or WS_DATA_TYPE_TEXT)
 * \param *msg the payload data
 * \param len the payload length
 *
 * \return 0 if successful else -1
 *
 */
int
websocket_sendData(struct websocket_connection_desc *wsConnectionDesc, enum ws_data_type dataType,
                   const void *msg, size_t len);

/**
 * \brief Closes the given websocket connection
 *
 * \param *wsConnectionDesc Pointer to the websocket connection descriptor
 * \param code the closing code
 */
void
websocket_closeConnection(struct websocket_connection_desc *wsConnectionDesc,
                          enum ws_close_code code);

/**
 * \brief Sends fragmented binary or text data through websockets
 *         use websocket_sendDataFragmetedCont for further fragments
 *
 * \param *wsConnectionDesc Pointer to the websocket connection descriptor
 * \param dataType the datatype (WS_DATA_TYPE_BINARY or WS_DATA_TYPE_TEXT)
 * \param *msg the payload data
 * \param len the payload length
 *
 * \return 0 if successful else -1
 *
 */
int
websocket_sendDataFragmentedStart(struct websocket_connection_desc *wsConnectionDesc,
                                  enum ws_data_type dataType, const void *msg, size_t len);
/**
 * \brief Continues a fragmented send
 *         use sendDataFragmentedStop to stop the transmission
 *
 * \param *wsConnectionDesc Pointer to the websocket connection descriptor
 * \param fin true => this is the last fragment else false
 * \param *msg the payload data
 * \param len the payload length
 *
 * \return 0 if successful else -1
 *
 */
int
websocket_sendDataFragmentedCont(struct websocket_connection_desc *wsConnectionDesc, bool fin,
                                 const void *msg, size_t len);

/* ------------------------------ LEGACY FUNCTIONS ------------------------------ */

/**
 * \brief Opens a websocket server
 *
 * \param *wsInit Pointer to the init struct
 * \param *websocketUserData: userData for the socket
 *
 * \return the websocket descriptor or NULL in case of error
 *
 * \note LEGACY!!! This function is for backward compatibility
 *       it should not be used anymore but will not be
 *       removed unless there's a good reason
 */
void *
websocket_open(struct websocket_init *wsInit, void *websocketUserData);

/**
 * \brief Closes the given websocket server
 *
 * \param *wsDesc Pointer to the websocket descriptor
 *
 * \note LEGACY!!! This function is for backward compatibility
 *       it should not be used anymore but will not be
 *       removed unless there's a good reason
 */
void
websocket_close(void *wsDesc);

/**
 * \brief Returns the user data of the given client
 *
 * \param *wsClientDesc Pointer to the websocket client descriptor
 *
 * \note LEGACY!!! This function is for backward compatibility
 *       it should not be used anymore but will not be
 *       removed unless there's a good reason
 */
void *
websocket_getClientUserData(void *wsClientDesc);

void
websocket_init();

#ifdef __cplusplus
}
#endif

#endif /* WEBSOCKET_H_ */
