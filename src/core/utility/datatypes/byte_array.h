/**
 * @file <argos3/core/utility/datatypes/byte_array.h>
 *
 * @brief Defines the byte array class.
 *
 * This file provides the definition of the argos::CByteArray class.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef BYTE_ARRAY_H
#define BYTE_ARRAY_H

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <vector>
#include <iterator>

namespace argos {

   /**
    * Byte array utility class.
    * This class is useful for serializing any kind of data into a byte array,
    * to be then streamed to something. It is used by the argos::CMemento class
    * as a basic tool to store the state of an object and retrieve it.
    */
   class CByteArray {

   public:

      /**
       * Class constructor.
       */
      CByteArray() {}

      /**
       * Class copy constructor.
       */
      CByteArray(const CByteArray& c_byte_array) :
         m_vecBuffer(c_byte_array.m_vecBuffer) {}

      /**
       * Class constructor.
       * Copies the given buffer into the byte array. The original
       * buffer can be safely deleted.
       * @param pun_buffer the original buffer to copy from.
       * @param un_size the size of the original buffer.
       */
      CByteArray(const UInt8* pun_buffer,
                 size_t un_size);

      /**
       * Class constructor.
       * Creates a byte array that contains <tt>un_size</tt> copies of <tt>un_value</tt>.
       * @param un_size the initial size of the byte array.
       * @param un_value the value to use to populate the byte array.
       */
      CByteArray(size_t un_size,
                 UInt8 un_value = 0);

      /**
       * Returns the current size of the byte array.
       * @return the current size of the byte array.
       */
      inline size_t Size() const {
         return m_vecBuffer.size();
      }

      /**
       * Returns <tt>true</tt> if the byte array is empty.
       * @return <tt>true</tt> if the byte array is empty.
       */
      inline bool Empty() const {
         return m_vecBuffer.empty();
      }

      /**
       * Returns the contents of the byte array as a const c-style array.
       * The returned pointer is valid under two conditions:
       * (i) the CByteArray object has not been deallocated, and
       * (ii) the contents of the byte array have not been changed.
       * If the byte array is empty, this method returns <tt>NULL</tt>.
       * @return the contents of the byte array as a const c-style array.
       */
      inline const UInt8* ToCArray() const {
         return !Empty() ? &m_vecBuffer[0] : NULL;
      }

      /**
       * Assignment operator.
       * Deep-copies the given byte array into the current byte array.
       */
      inline CByteArray& operator=(const CByteArray& c_byte_array) {
         if(this != &c_byte_array) {
            m_vecBuffer = c_byte_array.m_vecBuffer;
         }
         return *this;
      }

      /**
       * Read/write index operator.
       * @param un_index the index of the wanted element.
       * @returns a reference to the wanted element.
       */
      inline UInt8& operator[](size_t un_index) {
         if(un_index >= Size()) THROW_ARGOSEXCEPTION("CByteArray: index out of bounds [index = " << un_index << ", size=" << Size() << "]");
         return m_vecBuffer.at(un_index);
      }

      /**
       * Read-only index operator.
       * @param un_index the index of the wanted element.
       * @returns the value of the wanted element.
       */
      inline UInt8 operator[](size_t un_index) const {
         if(un_index >= Size()) THROW_ARGOSEXCEPTION("CByteArray: index out of bounds [index = " << un_index << ", size=" << Size() << "]");
         return m_vecBuffer.at(un_index);
      }

      /**
       * Appends bytes to the byte array.
       * @param pun_buffer the byte buffer to copy from.
       * @param un_size the size of the byte buffer.
       * @return a reference to this byte array.
       */
      CByteArray& AddBuffer(const UInt8* pun_buffer,
                            size_t un_size);

      /**
       * Moves elements from the byte array into the passed buffer.
       * The elements whose values were written into the target buffer are removed from the byte array.
       * @param pun_buffer the byte buffer to write into.
       * @param un_size the size of the target byte buffer.
       * @return a reference to this byte array.
       */
      CByteArray& FetchBuffer(UInt8* pun_buffer,
                              size_t un_size);

      /**
       * Appends a 8-bit unsigned integer to the byte array.
       * @param un_value the value of the 8-bit unsigned integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(UInt8 un_value);
      /**
       * Moves an 8-bit unsigned integer from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * @param un_value the buffer for the 8-bit unsigned integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(UInt8& un_value);
      /**
       * Appends a 8-bit signed integer to the byte array.
       * @param n_value the value of the 8-bit signed integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(SInt8 n_value);
      /**
       * Moves an 8-bit signed integer from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * @param n_value the buffer for the 8-bit unsigned integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(SInt8& n_value);
      /**
       * Appends a 16-bit unsigned integer to the byte array.
       * @param un_value the value of the 16-bit unsigned integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(UInt16 un_value);
      /**
       * Moves a 16-bit unsigned integer from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * @param un_value the buffer for the 16-bit unsigned integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(UInt16& un_value);
      /**
       * Appends a 16-bit signed integer to the byte array.
       * @param n_value the value of the 16-bit signed integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(SInt16 n_value);
      /**
       * Moves a 16-bit signed integer from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * @param n_value the buffer for the 16-bit signed integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(SInt16& n_value);
      /**
       * Appends a 32-bit unsigned integer to the byte array.
       * @param un_value the value of the 32-bit unsigned integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(UInt32 un_value);
      /**
       * Moves a 32-bit unsigned integer from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * @param un_value the buffer for the 32-bit unsigned integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(UInt32& un_value);
      /**
       * Appends a 32-bit signed integer to the byte array.
       * @param n_value the value of the 32-bit signed integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(SInt32 n_value);
      /**
       * Moves a 32-bit signed integer from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * @param n_value the buffer for the 32-bit signed integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(SInt32& n_value);
      /**
       * Appends a 64-bit unsigned integer to the byte array.
       * @param un_value the value of the 64-bit unsigned integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(UInt64 un_value);
      /**
       * Moves a 64-bit unsigned integer from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * @param un_value the buffer for the 64-bit unsigned integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(UInt64& un_value);
      /**
       * Appends a 64-bit signed integer to the byte array.
       * @param n_value the value of the 64-bit signed integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(SInt64 n_value);
      /**
       * Moves a 64-bit signed integer from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * @param n_value the buffer for the 64-bit signed integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(SInt64& n_value);
      /**
       * Appends an unsigned long integer to the byte array.
       * This function should be avoided as much as possible because
       * the size of long integers changes on 32- and 64-bit
       * architectures.
       * @param un_value the value of the unsigned long integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(unsigned long int un_value);
      /**
       * Moves an unsigned long integer from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * This function should be avoided as much as possible because
       * the size of long integers changes on 32- and 64-bit
       * architectures.
       * @param un_value the buffer for the unsigned long integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(unsigned long int& un_value);
      /**
       * Appends a signed long integer to the byte array.
       * This function should be avoided as much as possible because
       * the size of long integers changes on 32- and 64-bit
       * architectures.
       * @param n_value the value of the signed long integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(signed long int n_value);
      /**
       * Moves a signed long integer from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * This function should be avoided as much as possible because
       * the size of long integers changes on 32- and 64-bit
       * architectures.
       * @param n_value the buffer for the signed long integer.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(signed long int& n_value);
      /**
       * Appends an argos::Real to the byte array.
       * @param f_value the value of the argos::Real.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(Real f_value);
      /**
       * Moves an argos::Real from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * @param f_value the buffer for the argos::Real.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(Real& f_value);
      /**
       * Appends a <tt>std::string</tt> to the byte array.
       * @param str_value the value of the <tt>std::string</tt>.
       * @return a reference to this byte array.
       */
      CByteArray& operator<<(const std::string& str_value);
      /**
       * Moves a <tt>std::string</tt> from the byte array to the target variable.
       * The element whose value was written into the target buffer are removed from the byte array.
       * @param str_value the buffer for the <tt>std::string</tt>.
       * @return a reference to this byte array.
       */
      CByteArray& operator>>(std::string& str_value);

      /**
       * Stream operator.
       * It streams the byte array through the given stream. Useful for files, for instance.
       * @param c_os the C++ output stream the byte array to.
       * @param c_byte_array the byte array to stream.
       * @return the new state of the output stream.
       */
      friend std::ostream& operator<<(std::ostream& c_os, const CByteArray& c_byte_array);

   private:

      std::vector<UInt8> m_vecBuffer;

   };

}

#endif
