#ifndef LED_MEDIUM_H
#define LED_MEDIUM_H

namespace argos {
   class CLEDMedium;
   class CLEDEntity;
}

#include <argos3/core/simulator/medium/medium.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/plugins/simulator/entities/led_entity.h>

namespace argos {

   class CLEDMedium : public CMedium {

   public:

      /**
       * Class constructor.
       */
      CLEDMedium();

      /**
       * Class destructor.
       */
      virtual ~CLEDMedium();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void PostSpaceInit();
      virtual void Reset();
      virtual void Destroy();
      virtual void Update();

     /**
      * Adds the specified entity to the list of managed entities.
      * @param c_entity The entity to add.
      */
      void AddEntity(CLEDEntity& c_entity);

     /**
      * Removes the specified entity from the list of managed entities.
      * @param c_entity The entity to remove.
      */
      void RemoveEntity(CLEDEntity& c_entity);

      /**
       * Returns the LED positional index.
       * @return The LED positional index.
       */
      CPositionalIndex<CLEDEntity>& GetIndex() {
         return *m_pcLEDEntityIndex;
      }

   private:

      /** A positional index for the LED entities */
      CPositionalIndex<CLEDEntity>* m_pcLEDEntityIndex;

      /** The update operation for the grid positional index */
      CLEDEntityGridUpdater* m_pcLEDEntityGridUpdateOperation;

   };

}

#endif
