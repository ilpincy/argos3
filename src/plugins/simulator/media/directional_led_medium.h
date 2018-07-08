/**
 * @file <argos3/plugins/simulator/media/directional_led_medium.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DIRECTIONAL_LED_MEDIUM_H
#define DIRECTIONAL_LED_MEDIUM_H

namespace argos {
   class CDirectionalLEDMedium;
   class CDirectionalLEDEntity;
}

#include <argos3/core/simulator/medium/medium.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/plugins/simulator/entities/directional_led_entity.h>

namespace argos {

   class CDirectionalLEDMedium : public CMedium {

   public:

      /**
       * Class constructor.
       */
      CDirectionalLEDMedium() :
         m_pcDirectionalLEDEntityIndex(nullptr),
         m_pcDirectionalLEDEntityGridUpdateOperation(nullptr) {}

      /**
       * Class destructor.
       */
      virtual ~CDirectionalLEDMedium() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void PostSpaceInit();
      virtual void Reset();
      virtual void Destroy();
      virtual void Update();

     /**
      * Adds the specified entity to the list of managed entities.
      * @param c_entity The entity to add.
      */
      void AddEntity(CDirectionalLEDEntity& c_entity);

     /**
      * Removes the specified entity from the list of managed entities.
      * @param c_entity The entity to remove.
      */
      void RemoveEntity(CDirectionalLEDEntity& c_entity);

      /**
       * Returns the directional LED positional index.
       * @return The directional LED positional index.
       */
      CPositionalIndex<CDirectionalLEDEntity>& GetIndex() {
         return *m_pcDirectionalLEDEntityIndex;
      }

   private:

      /** A positional index for the LED entities */
      CPositionalIndex<CDirectionalLEDEntity>* m_pcDirectionalLEDEntityIndex;

      /** The update operation for the grid positional index */
      CDirectionalLEDEntityGridUpdater* m_pcDirectionalLEDEntityGridUpdateOperation;

   };

}

#endif
