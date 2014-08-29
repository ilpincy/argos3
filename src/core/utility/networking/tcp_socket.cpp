#include "tcp_socket.h"

#include <argos3/core/utility/string_utilities.h>

#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

namespace argos {

   /****************************************/
   /****************************************/

   CTCPSocket::CTCPSocket(int n_stream) :
      m_nStream(n_stream) {
   }

   /****************************************/
   /****************************************/

   CTCPSocket::~CTCPSocket() {
      Disconnect();
   }

   /****************************************/
   /****************************************/

   void CTCPSocket::Connect(const std::string& str_hostname,
                            SInt32 n_port) {
      /* Used to store the return value of the network function calls */
      int nRetVal;
      /* Get information on the available interfaces */
      ::addrinfo tHints, *ptInterfaceInfo;
      ::memset(&tHints, 0, sizeof(tHints));
      tHints.ai_family = AF_INET;       /* Only IPv4 is accepted */
      tHints.ai_socktype = SOCK_STREAM; /* TCP socket */
      nRetVal = ::getaddrinfo(str_hostname.c_str(),
                              ToString(n_port).c_str(),
                              &tHints,
                              &ptInterfaceInfo);
      if(nRetVal != 0) {
         THROW_ARGOSEXCEPTION("Error getting address information: " << ::gai_strerror(nRetVal));
      }
      /* Bind on the first interface available */
      m_nStream = -1;
      ::addrinfo* ptInterface = NULL;
      for(ptInterface = ptInterfaceInfo;
          (ptInterface != NULL) && (m_nStream == -1);
          ptInterface = ptInterface->ai_next) {
         m_nStream = ::socket(ptInterface->ai_family,
                              ptInterface->ai_socktype,
                              ptInterface->ai_protocol);
         if(m_nStream > 0) {
            if(::connect(m_nStream,
                         ptInterface->ai_addr,
                         ptInterface->ai_addrlen) == -1) {
               m_nStream = -1;
               THROW_ARGOSEXCEPTION("Can't connect to host: " << ::strerror(errno));
            }
         }
      }
      ::freeaddrinfo(ptInterfaceInfo);
   }

   /****************************************/
   /****************************************/

   void CTCPSocket::Listen(SInt32 n_port,
                           SInt32 n_queue_length) {
      /* Used to store the return value of the network function calls */
      int nRetVal;
      /* Get information on the available interfaces */
      ::addrinfo tHints, *ptInterfaceInfo;
      ::memset(&tHints, 0, sizeof(tHints));
      tHints.ai_family = AF_INET;       /* Only IPv4 is accepted */
      tHints.ai_socktype = SOCK_STREAM; /* TCP socket */
      tHints.ai_flags = AI_PASSIVE;     /* Necessary for bind() later on */
      nRetVal = ::getaddrinfo(NULL,
                              ToString(n_port).c_str(),
                              &tHints,
                              &ptInterfaceInfo);
      if(nRetVal != 0) {
         THROW_ARGOSEXCEPTION("Error getting local address information: " << ::gai_strerror(nRetVal));
      }
      /* Bind on the first interface available */
      m_nStream = -1;
      ::addrinfo* ptInterface = NULL;
      for(ptInterface = ptInterfaceInfo;
          (ptInterface != NULL) && (m_nStream == -1);
          ptInterface = ptInterface->ai_next) {
         m_nStream = ::socket(ptInterface->ai_family,
                              ptInterface->ai_socktype,
                              ptInterface->ai_protocol);
         if(m_nStream > 0) {
            int nTrue = 1;
            if((::setsockopt(m_nStream,
                             SOL_SOCKET,
                             SO_REUSEADDR,
                             &nTrue,
                             sizeof(nTrue)) != -1)
               &&
               (::bind(m_nStream,
                       ptInterface->ai_addr,
                       ptInterface->ai_addrlen) == -1)) {
               Disconnect();
            }
         }
      }
      ::freeaddrinfo(ptInterfaceInfo);
      if(m_nStream == -1) {
         THROW_ARGOSEXCEPTION("Can't bind socket to any interface");
      }
      /* Listen on the socket */
      if(::listen(m_nStream, n_queue_length) == -1) {
         Disconnect();
         THROW_ARGOSEXCEPTION("Can't listen on the socket" << ::strerror(errno));
      }
   }

   /****************************************/
   /****************************************/

   void CTCPSocket::Accept(CTCPSocket& c_socket) {
      /* Accept connections */
      ::sockaddr tAddress;
      ::socklen_t tAddressLen = sizeof(tAddress);
      int nNewStream = ::accept(m_nStream, &tAddress, &tAddressLen);
      if(nNewStream == -1) {
         Disconnect();
         THROW_ARGOSEXCEPTION("Error accepting connection: " << ::strerror(errno));
      }
      c_socket.m_nStream = nNewStream;
      c_socket.m_strAddress = ::inet_ntoa(reinterpret_cast< ::sockaddr_in* >(&tAddress)->sin_addr);
   }

   /****************************************/
   /****************************************/

   void CTCPSocket::Disconnect() {
      ::close(m_nStream);
      m_nStream = -1;
      m_strAddress = "";
   }

   /****************************************/
   /****************************************/

   void CTCPSocket::SendBuffer(const UInt8* pun_buffer,
                               size_t un_size) {
      ssize_t nSent;
      while(un_size > 0) {
         nSent = ::send(m_nStream, pun_buffer, un_size, 0);
         if(nSent < 0) {
            Disconnect();
            THROW_ARGOSEXCEPTION("Error sending data: " << ::strerror(errno));
         }
         un_size -= nSent;
         pun_buffer += nSent;
      }
   }

   /****************************************/
   /****************************************/

   bool CTCPSocket::ReceiveBuffer(UInt8* pun_buffer,
                                  size_t un_size) {
      ssize_t nReceived;
      while(un_size > 0) {
         nReceived = ::recv(m_nStream, pun_buffer, un_size, 0);
         if(nReceived < 0){
            Disconnect();
             THROW_ARGOSEXCEPTION("Error receiving data: " << ::strerror(errno));
         }
         if(nReceived == 0) return false;
         un_size -= nReceived;
         pun_buffer += nReceived;
      }
      return true;
   }

   /****************************************/
   /****************************************/

   void CTCPSocket::SendByteArray(const CByteArray& c_byte_array) {
      /* Send the length of the byte array */
      UInt32 unSizeNBO = htonl(c_byte_array.Size());
      SendBuffer(reinterpret_cast<UInt8*>(&unSizeNBO), sizeof(unSizeNBO));
      /* Send the actual data */
      SendBuffer(c_byte_array.ToCArray(), c_byte_array.Size());
   }

   /****************************************/
   /****************************************/

   bool CTCPSocket::ReceiveByteArray(CByteArray& c_byte_array) {
      /* Receive the length of the byte array */
      UInt32 unSizeNBO;
      if(ReceiveBuffer(reinterpret_cast<UInt8*>(&unSizeNBO), sizeof(unSizeNBO))) {
         /* Receive the actual data */
         c_byte_array.Resize(ntohl(unSizeNBO));
         if(ReceiveBuffer(c_byte_array.ToCArray(), c_byte_array.Size())) {
            return true;
         }
      }
      return false;
   }

   /****************************************/
   /****************************************/

}
