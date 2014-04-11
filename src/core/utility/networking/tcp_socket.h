#ifndef TCPSOCKET_H
#define TCPSOCKET_H

namespace argos {
   class CTCPSocket;
}

#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/utility/datatypes/datatypes.h>

namespace argos {

   class CTCPSocket {

   public:

      CTCPSocket(int n_stream = -1);

      ~CTCPSocket();

      inline bool IsConnected() const {
         return m_nStream != -1;
      }

      /**
       * Connects this socket to the specified hostname and port.
       * @param str_hostname The wanted hostname
       * @param n_port The wanted port
       * @throws CARGoSException in case of error
       */
      void Connect(const std::string& str_hostname,
                   SInt32 n_port);

      /**
       * Accept a connection from a client on the specified local port.
       * @param c_socket The socket on which the connection has been created
       * @param n_port The wanted port
       * @param n_queue_length The maximum length of the queue of pending connections (also called the backlog)
       * @throws CARGoSException in case of error
       */
      void Accept(CTCPSocket& c_socket,
                  SInt32 n_port,
                  SInt32 n_queue_length = 10);

      /**
       * Close the socket.
       * @throws CARGoSException in case of error
       */
      void Disconnect();

      /**
       * Sends the passed buffer through the socket.
       * @param pun_buffer The wanted buffer
       * @param un_size The size of the buffer
       * @throws CARGoSException in case of error
       */
      void SendBuffer(const UInt8* pun_buffer,
                      size_t un_size);

      /**
       * Fills the passed buffer with the data received through the socket.
       * @param pun_buffer The buffer to fill
       * @param un_size The size of the buffer
       * @return <tt>true</tt> if the buffer was filled correctly; <tt>false</tt> if the connection was closed by the other peer
       * @throws CARGoSException in case of error
       */
      bool ReceiveBuffer(UInt8* pun_buffer,
                         size_t un_size);

      /**
       * Sends the passed byte array through the socket.
       * Internally, this function first sends the size of the
       * byte array as a long int, and then sends the content of
       * the byte array.
       * It is meant to the be used in conjunction with
       * ReceiveByteArray().
       * @param c_byte_array The byte array
       * @throws CARGoSException in case of error
       * @see CByteArray
       * @see SendBuffer
       * @see ReceiveByteArray
       */
      void SendByteArray(const CByteArray& c_byte_array);

      /**
       * Receives the passed byte array through the socket.
       * Internally, this function first receives the size of the
       * byte array as a long int, and then receives the content of
       * the byte array.
       * It is meant to the be used in conjunction with
       * SendByteArray().
       * @param c_byte_array The byte array
       * @return <tt>true</tt> if the buffer was filled correctly; <tt>false</tt> if the connection was closed by the other peer
       * @throws CARGoSException in case of error
       * @see CByteArray
       * @see ReceiveBuffer
       * @see SendByteArray
       */
      bool ReceiveByteArray(CByteArray& c_byte_array);

   private:

      /** The socket stream */
      int m_nStream;

   };

}

#endif
