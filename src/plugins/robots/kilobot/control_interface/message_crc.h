#ifndef __MESSAGE_CRC_H__
#define __MESSAGE_CRC_H__

#include "message.h"

/**
 * @brief Function to compute the CRC of a message struct.
 *
 * This function receives as input a pointer to a message structure @p
 * msg, and uses the message data and type to compute and return a @c
 * uint16_t CRC value.
 *
 * @code
 * message_t msg;
 * msg.data[0] = 1;
 * msg.data[1] = 2;
 * ...
 * msg.type = NORMAL;
 * msg.crc = message_crc(&msg);
 * @endcode
 *
 * @param msg Pointer to an input message.
 * @return A 16 bit CRC of the message payload.
 *
 * @see message_t
 * @see message_type_t
 * @note It is imperative to set the CRC value of a message before
 * transmission, otherwise on reception the message will be classified
 * as being corrupt and discarded.
 */

uint16_t message_crc(const message_t *msg);

#endif//__MESSAGES_CRC_H__
