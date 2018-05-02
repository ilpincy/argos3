/**
 * @file <argos3/plugins/simulator/media/radio_medium.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef RADIO_MEDIUM_H
#define RADIO_MEDIUM_H

namespace argos {
   class CRadioMedium;
   class CRadioEntity;
}

#include <argos3/core/simulator/medium/medium.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/plugins/simulator/entities/radio_entity.h>

namespace argos {

   class CRadioMedium : public CMedium {

   public:

      /**
       * Class constructor.
       */
      CRadioMedium() :
         m_pcRadioEntityIndex(nullptr),
         m_pcRadioEntityGridUpdateOperation(nullptr) {}

      /**
       * Class destructor.
       */
      virtual ~CRadioMedium() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void PostSpaceInit();
      virtual void Reset();
      virtual void Destroy();
      virtual void Update();

     /**
      * Adds the specified entity to the list of managed entities.
      * @param c_entity The entity to add.
      */
      void AddEntity(CRadioEntity& c_entity);

     /**
      * Removes the specified entity from the list of managed entities.
      * @param c_entity The entity to remove.
      */
      void RemoveEntity(CRadioEntity& c_entity);

      /**
       * Returns the radio positional index.
       * @return The radio positional index.
       */
      CPositionalIndex<CRadioEntity>& GetIndex() {
         return *m_pcRadioEntityIndex;
      }


   private:
      
      /** A positional index for the radio entities */
      CPositionalIndex<CRadioEntity>* m_pcRadioEntityIndex;

      /** The update operation for the grid positional index */
      CRadioEntityGridUpdater* m_pcRadioEntityGridUpdateOperation;

   };

}

#endif
