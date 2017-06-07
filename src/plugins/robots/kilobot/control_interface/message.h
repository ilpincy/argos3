#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <stdint.h>

typedef enum {
    NORMAL = 0
} message_type_t;

/**
 * @brief Message structure.
 *
 * A message structure is 12 bytes in length and is composed of three
 * parts: the payload (9 bytes), the message type (1 byte), and a CRC (2
 * bytes).
 *
 * @note When preparing a message for transmission, at a minimum you
 * must specify the type (use a value between 0 and 127 for user
 * messages) and the CRC (use the message_crc() function for this
 * purpose).
 *
 * @see message_crc, kilo_message_rx, kilo_message_tx,
 * kilo_message_tx_success
 */
typedef struct {
    uint8_t data[9]; ///< message payload.
    uint8_t type;    ///< message type. 
    uint16_t crc;    ///< message crc.
} message_t;

#endif//__MESSAGES_H__
