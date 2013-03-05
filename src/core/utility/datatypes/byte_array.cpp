/**
 * @file <argos3/core/utility/datatypes/byte_array.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "byte_array.h"

namespace argos {

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
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(SInt16 n_value) {
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
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(UInt32 un_value) {
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
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(SInt32 un_value) {
      UInt8* punByte = reinterpret_cast<UInt8*>(&un_value);
      m_vecBuffer.push_back(punByte[0]);
      m_vecBuffer.push_back(punByte[1]);
      m_vecBuffer.push_back(punByte[2]);
      m_vecBuffer.push_back(punByte[3]);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(SInt32& un_value) {
      if(Size() < 4) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (4 requested, " << Size() << " available)");
      UInt8* punByte = reinterpret_cast<UInt8*>(&un_value);
      punByte[0] = m_vecBuffer[0];
      punByte[1] = m_vecBuffer[1];
      punByte[2] = m_vecBuffer[2];
      punByte[3] = m_vecBuffer[3];
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + 4);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(UInt64 un_value) {
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
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(SInt64 un_value) {
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

   CByteArray& CByteArray::operator>>(SInt64& un_value) {
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
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(unsigned long int un_value) {
      UInt32 unSize = sizeof(un_value);
      UInt8* punByte = reinterpret_cast<UInt8*>(&un_value);
      for(UInt32 i = 0; i < unSize; ++i) {
         m_vecBuffer.push_back(punByte[i]);
      }
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(unsigned long int& un_value) {
      UInt32 unSize = sizeof(un_value);
      if(Size() < unSize) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (" << unSize << " requested, " << Size() << " available)");
      UInt8* punByte = reinterpret_cast<UInt8*>(&un_value);
      for(UInt32 i = 0; i < unSize; ++i) {
         *punByte = m_vecBuffer[i];
         ++punByte;
      }
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + unSize);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(signed long int n_value) {
      UInt32 unSize = sizeof(n_value);
      UInt8* punByte = reinterpret_cast<UInt8*>(&n_value);
      for(UInt32 i = 0; i < unSize; ++i) {
         m_vecBuffer.push_back(punByte[i]);
      }
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(signed long int& n_value) {
      UInt32 unSize = sizeof(n_value);
      if(Size() < unSize) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (" << unSize << " requested, " << Size() << " available)");
      UInt8* punByte = reinterpret_cast<UInt8*>(&n_value);
      for(UInt32 i = 0; i < unSize; ++i) {
         *punByte = m_vecBuffer[i];
         ++punByte;
      }
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + unSize);
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator<<(Real f_value) {
      UInt32 unSize = sizeof(f_value);
      UInt8* punByte = reinterpret_cast<UInt8*>(&f_value);
      for(UInt32 i = 0; i < unSize; ++i) {
         m_vecBuffer.push_back(punByte[i]);
      }
      return *this;
   }

   /****************************************/
   /****************************************/

   CByteArray& CByteArray::operator>>(Real& f_value) {
      UInt32 unSize = sizeof(f_value);
      if(Size() < unSize) THROW_ARGOSEXCEPTION("Attempting to extract too many bytes from byte array (" << unSize << " requested, " << Size() << " available)");
      UInt8* punByte = reinterpret_cast<UInt8*>(&f_value);
      for(UInt32 i = 0; i < unSize; ++i) {
         *punByte = m_vecBuffer[i];
         ++punByte;
      }
      m_vecBuffer.erase(m_vecBuffer.begin(), m_vecBuffer.begin() + unSize);
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
