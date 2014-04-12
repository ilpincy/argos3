/**
 * @file <argos3/core/utility/datatypes/byte_array.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "byte_array.h"

#include <arpa/inet.h>
#include <cstring>

namespace argos {

   /****************************************/
   /****************************************/

   static UInt64 htonll(UInt64 un_value) {
      /* Define a test variable for endianness - 42 is 'the' answer */
      static const SInt32 nTest = 42;
      /* Test for host endianness */
      if(*reinterpret_cast<const UInt8*>(&nTest) == nTest) {
         /* Host is little endian, network is big -> convert to big */
         const UInt32 unHighWord = htonl(static_cast<UInt32>(un_value >> 32));
         const UInt32 unLowWord  = htonl(static_cast<UInt32>(un_value & 0xFFFFFFFFLL));
         return static_cast<UInt64>(unLowWord) << 32 | unHighWord;
      }
      else {
         /* Host is big endian - leave as is */
         return un_value;
      }
   }

   static UInt64 ntohll(UInt64 un_value) {
      /* Define a test variable for endianness - 42 is 'the' answer */
      static const SInt32 nTest = 42;
      /* Test for host endianness */
      if(*reinterpret_cast<const UInt8*>(&nTest) == nTest) {
         /* Host is little endian, network is big -> convert to big */
         const UInt32 unHighWord = ntohl(static_cast<UInt32>(un_value >> 32));
         const UInt32 unLowWord  = ntohl(static_cast<UInt32>(un_value & 0xFFFFFFFFLL));
         return static_cast<UInt64>(unLowWord) << 32 | unHighWord;
      }
      else {
         /* Host is big endian - leave as is */
         return un_value;
      }
   }
   
   /****************************************/
   /****************************************/

   CByteArray::CByteArray(const UInt8* pun_buffer,
                          size_t un_size) {
      AddBuffer(pun_buffer, un_size);
   }

   /****************************************/
   /****************************************/

   CByteArray::CByteArray(size_t un_size,
                          UInt8 un_value) {
      m_vecBuffer.assign(un_size, un_value);
   }

   /****************************************/
   /****************************************/

   void CByteArray::Zero() {
      ::memset(&m_vecBuffer[0], 0, sizeof(UInt8) * Size());
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator=(const CByteArray& c_byte_array) {
      if(this != &c_byte_array) {
         m_vecBuffer = c_byte_array.m_vecBuffer;
      }
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::AddBuffer(const UInt8* pun_buffer,
                                     size_t un_size) {      
      for(size_t i = 0; i < un_size; ++i) {
         m_vecBuffer.push_back(pun_buffer[i]);
      }
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::FetchBuffer(UInt8* pun_buffer,
                                       size_t un_size) {
      if(Size() < un_size) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (" << un_size << " requested, " << Size() << " available)");
      for(size_t i = 0; i < un_size; ++i) {
         *(pun_buffer+i) = m_vecBuffer[i];
      }
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + un_size);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(UInt8 un_value) {
      m_vecBuffer.push_back(un_value);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(UInt8& un_value) {
      if(Size() < 1) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (1 requested, " << Size() << " available)");
      un_value = m_vecBuffer.front();
      m_vecBuffer.erase(m_vecBuffer.begin());
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(SInt8 n_value) {
      m_vecBuffer.push_back(n_value);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(SInt8& n_value) {
      if(Size() < 1) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (1 requested, " << Size() << " available)");
      n_value = m_vecBuffer.front();
      m_vecBuffer.erase(m_vecBuffer.begin());
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(UInt16 un_value) {
      un_value = htons(un_value);
      UInt8* punByte = reinterpret_cast<UInt8*>(&un_value);
      m_vecBuffer.push_back(punByte[0]);
      m_vecBuffer.push_back(punByte[1]);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(UInt16& un_value) {
      if(Size() < 2) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (2 requested, " << Size() << " available)");
      UInt8* punByte = reinterpret_cast<UInt8*>(&un_value);
      punByte[0] = m_vecBuffer[0];
      punByte[1] = m_vecBuffer[1];
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + 2);
      un_value = ntohs(un_value);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(SInt16 n_value) {
      n_value = htons(n_value);
      UInt8* punByte = reinterpret_cast<UInt8*>(&n_value);
      m_vecBuffer.push_back(punByte[0]);
      m_vecBuffer.push_back(punByte[1]);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(SInt16& n_value) {
      if(Size() < 2) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (2 requested, " << Size() << " available)");
      UInt8* punByte = reinterpret_cast<UInt8*>(&n_value);
      punByte[0] = m_vecBuffer[0];
      punByte[1] = m_vecBuffer[1];
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + 2);
      n_value = ntohs(n_value);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(UInt32 un_value) {
      un_value = htonl(un_value);
      UInt8* punByte = reinterpret_cast<UInt8*>(&un_value);
      m_vecBuffer.push_back(punByte[0]);
      m_vecBuffer.push_back(punByte[1]);
      m_vecBuffer.push_back(punByte[2]);
      m_vecBuffer.push_back(punByte[3]);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(UInt32& un_value) {
      if(Size() < 4) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (4 requested, " << Size() << " available)");
      UInt8* punByte = reinterpret_cast<UInt8*>(&un_value);
      punByte[0] = m_vecBuffer[0];
      punByte[1] = m_vecBuffer[1];
      punByte[2] = m_vecBuffer[2];
      punByte[3] = m_vecBuffer[3];
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + 4);
      un_value = ntohl(un_value);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(SInt32 n_value) {
      n_value = htonl(n_value);
      UInt8* punByte = reinterpret_cast<UInt8*>(&n_value);
      m_vecBuffer.push_back(punByte[0]);
      m_vecBuffer.push_back(punByte[1]);
      m_vecBuffer.push_back(punByte[2]);
      m_vecBuffer.push_back(punByte[3]);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(SInt32& n_value) {
      if(Size() < 4) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (4 requested, " << Size() << " available)");
      UInt8* punByte = reinterpret_cast<UInt8*>(&n_value);
      punByte[0] = m_vecBuffer[0];
      punByte[1] = m_vecBuffer[1];
      punByte[2] = m_vecBuffer[2];
      punByte[3] = m_vecBuffer[3];
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + 4);
      n_value = ntohl(n_value);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(UInt64 un_value) {
      un_value = htonll(un_value);
      UInt8* punByte = reinterpret_cast<UInt8*>(&un_value);
      m_vecBuffer.push_back(punByte[0]);
      m_vecBuffer.push_back(punByte[1]);
      m_vecBuffer.push_back(punByte[2]);
      m_vecBuffer.push_back(punByte[3]);
      m_vecBuffer.push_back(punByte[4]);
      m_vecBuffer.push_back(punByte[5]);
      m_vecBuffer.push_back(punByte[6]);
      m_vecBuffer.push_back(punByte[7]);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(UInt64& un_value) {
      if(Size() < 8) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (8 requested, " << Size() << " available)");         
      UInt8* punByte = reinterpret_cast<UInt8*>(&un_value);
      punByte[0] = m_vecBuffer[0];
      punByte[1] = m_vecBuffer[1];
      punByte[2] = m_vecBuffer[2];
      punByte[3] = m_vecBuffer[3];
      punByte[4] = m_vecBuffer[4];
      punByte[5] = m_vecBuffer[5];
      punByte[6] = m_vecBuffer[6];
      punByte[7] = m_vecBuffer[7];
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + 8);
      un_value = ntohll(un_value);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(SInt64 n_value) {
      n_value = htonll(n_value);
      UInt8* punByte = reinterpret_cast<UInt8*>(&n_value);
      m_vecBuffer.push_back(punByte[0]);
      m_vecBuffer.push_back(punByte[1]);
      m_vecBuffer.push_back(punByte[2]);
      m_vecBuffer.push_back(punByte[3]);
      m_vecBuffer.push_back(punByte[4]);
      m_vecBuffer.push_back(punByte[5]);
      m_vecBuffer.push_back(punByte[6]);
      m_vecBuffer.push_back(punByte[7]);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(SInt64& n_value) {
      if(Size() < 8) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (8 requested, " << Size() << " available)");         
      UInt8* punByte = reinterpret_cast<UInt8*>(&n_value);
      punByte[0] = m_vecBuffer[0];
      punByte[1] = m_vecBuffer[1];
      punByte[2] = m_vecBuffer[2];
      punByte[3] = m_vecBuffer[3];
      punByte[4] = m_vecBuffer[4];
      punByte[5] = m_vecBuffer[5];
      punByte[6] = m_vecBuffer[6];
      punByte[7] = m_vecBuffer[7];
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + 8);
      n_value = ntohll(n_value);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(unsigned long int un_value) {
      if(sizeof(un_value) == sizeof(UInt32)) {
         *this << static_cast<UInt32>(un_value);
      }
      else if(sizeof(un_value) == sizeof(UInt64)) {
         *this << static_cast<UInt64>(un_value);
      }
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(unsigned long int& un_value) {
      if(sizeof(un_value) == sizeof(UInt32)) {
         *this >> *reinterpret_cast<UInt32*>(&un_value);
      }      
      else if(sizeof(un_value) == sizeof(UInt64)) {
         *this >> *reinterpret_cast<UInt64*>(&un_value);
      }      
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(signed long int n_value) {
      if(sizeof(n_value) == sizeof(SInt32)) {
         *this << static_cast<SInt32>(n_value);
      }
      else if(sizeof(n_value) == sizeof(SInt64)) {
         *this << static_cast<SInt64>(n_value);
      }
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(signed long int& n_value) {
      if(sizeof(n_value) == sizeof(SInt32)) {
         *this >> *reinterpret_cast<SInt32*>(&n_value);
      }      
      else if(sizeof(n_value) == sizeof(SInt64)) {
         *this >> *reinterpret_cast<SInt64*>(&n_value);
      }      
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(Real f_value) {
#ifdef ARGOS_DOUBLE_PRECISION
      UInt64 unValue;
#else
      UInt32 unValue;
#endif
      ::memcpy(&unValue, &f_value, sizeof(unValue));
      *this << unValue;
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(Real& f_value) {
      if(Size() < sizeof(f_value)) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (" << sizeof(f_value) << " requested, " << Size() << " available)");
#ifdef ARGOS_DOUBLE_PRECISION
      UInt64 unValue;
#else
      UInt32 unValue;
#endif
      *this >> unValue;
      ::memcpy(&f_value, &unValue, sizeof(unValue));
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(const std::string& str_value) {
      /* Insert string contents */
      for(size_t i = 0; i < str_value.size(); ++i) {
         *this << static_cast<UInt8>(str_value[i]);
      }
      /* Terminate string with a \0 */
      *this << static_cast<UInt8>(0);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(std::string& str_value) {
      if(Empty()) THROW_ARGOSEXCEPTION("Attempting to extract values from empty byte array");
      str_value.clear();
      size_t i = 0;
      while(i < Size() && m_vecBuffer[i] != '\0') {
         str_value += m_vecBuffer[i];
         ++i;
      }
      if(m_vecBuffer[i] == '\0') {
         ++i;
      }
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + i);
      return *this;
   }

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os, const CByteArray& c_byte_array) {
      c_os << "CByteArray [";
      for(size_t i = 0; i < c_byte_array.Size(); ++i) {
         c_os << " " << c_byte_array.m_vecBuffer[i];
      }
      c_os << " ]" << std::endl;
      return c_os;
   }

   /****************************************/
   /****************************************/

}
