/**
 * @file <argos3/plugins/simulator/media/tag_medium.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef TAG_MEDIUM_H
#define TAG_MEDIUM_H

namespace argos {
   class CTagMedium;
   class CTagEntity;
}

#include <argos3/core/simulator/medium/medium.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/plugins/simulator/entities/tag_entity.h>

namespace argos {

   class CTagMedium : public CMedium {

   public:

      /**
       * Class constructor.
       */
      CTagMedium() :
         m_pcTagEntityIndex(nullptr),
         m_pcTagEntityGridUpdateOperation(nullptr) {}

      /**
       * Class destructor.
       */
      virtual ~CTagMedium() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void PostSpaceInit();
      virtual void Reset();
      virtual void Destroy();
      virtual void Update();

     /**
      * Adds the specified entity to the list of managed entities.
      * @param c_entity The entity to add.
      */
      void AddEntity(CTagEntity& c_entity);

     /**
      * Removes the specified entity from the list of managed entities.
      * @param c_entity The entity to remove.
      */
      void RemoveEntity(CTagEntity& c_entity);

      /**
       * Returns the tag positional index.
       * @return The tag positional index.
       */
      CPositionalIndex<CTagEntity>& GetIndex() {
         return *m_pcTagEntityIndex;
      }

   private:

      /** A positional index for the tag entities */
      CPositionalIndex<CTagEntity>* m_pcTagEntityIndex;

      /** The update operation for the grid positional index */
      CTagEntityGridUpdater* m_pcTagEntityGridUpdateOperation;

   };

}

#endif
