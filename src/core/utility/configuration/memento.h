/**
 * @file <argos3/core/utility/configuration/memento.h>
 *
 * @brief This file provides the definition of the memento interface.
 *
 * The <a href="http://en.wikipedia.org/wiki/Memento_pattern">Memento pattern</a>
 * is a software design pattern that provides the ability to restore an object to
 * its previous state.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef MEMENTO_H
#define MEMENTO_H

namespace argos {
   class CMemento;
}

#include <argos3/core/utility/datatypes/byte_array.h>

namespace argos {

   /**
    * The interface inherited by all classes whose state must be saved and restored.
    */
   class CMemento {

   public:

      /**
       * Class destructor.
       */
      virtual ~CMemento() {}

      /**
       * Saves the state of the object to the given buffer.
       * Data is added to the buffer.
       * @param c_buffer the target buffer
       */
      virtual void SaveState(CByteArray& c_buffer) = 0;

      /**
       * Restores the state of the object from the given buffer.
       * The read data is removed from the buffer.
       * @param c_buffer the source buffer
       */
      virtual void LoadState(CByteArray& c_buffer) = 0;

   };

}

#endif
