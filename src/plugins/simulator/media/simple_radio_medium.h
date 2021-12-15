/**
 * @file <argos3/plugins/simulator/media/simple_radio_medium.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef SIMPLE_RADIO_MEDIUM_H
#define SIMPLE_RADIO_MEDIUM_H

namespace argos {
   class CSimpleRadioMedium;
   class CSimpleRadioEntity;
}

#include <argos3/core/simulator/medium/medium.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/plugins/simulator/entities/simple_radio_entity.h>

namespace argos {

   class CSimpleRadioMedium : public CMedium {

   public:

      /**
       * Class constructor.
       */
      CSimpleRadioMedium() :
         m_pcEntityIndex(nullptr),
         m_pcEntityGridUpdateOperation(nullptr) {}

      /**
       * Class destructor.
       */
      virtual ~CSimpleRadioMedium() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void PostSpaceInit();
      virtual void Reset();
      virtual void Destroy();
      virtual void Update();

     /**
      * Adds the specified entity to the list of managed entities.
      * @param c_entity The entity to add.
      */
      void AddEntity(CSimpleRadioEntity& c_entity);

     /**
      * Removes the specified entity from the list of managed entities.
      * @param c_entity The entity to remove.
      */
      void RemoveEntity(CSimpleRadioEntity& c_entity);

      /**
       * Returns the radio positional index.
       * @return The radio positional index.
       */
      CPositionalIndex<CSimpleRadioEntity>& GetIndex() {
         return *m_pcEntityIndex;
      }


   private:
      
      /** A positional index for the radio entities */
      CPositionalIndex<CSimpleRadioEntity>* m_pcEntityIndex;

      /** The update operation for the grid positional index */
      CSimpleRadioEntityGridUpdater* m_pcEntityGridUpdateOperation;

   };

}

#endif
