#ifndef RAB_MEDIUM_H
#define RAB_MEDIUM_H

namespace argos {
   class CRABMedium;
   class CRABEquippedEntity;
}

#include <argos3/core/simulator/medium/medium.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>

namespace argos {

   class CRABMedium : public CMedium {

   public:

      /**
       * Class constructor.
       */
      CRABMedium();

      /**
       * Class destructor.
       */
      virtual ~CRABMedium();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void PostSpaceInit();
      virtual void Reset();
      virtual void Destroy();
      virtual void Update();

      /**
       * Adds the specified entity to the list of managed entities.
       * @param c_entity The entity to add.
       */
      void AddEntity(CRABEquippedEntity& c_entity);
      
      /**
       * Removes the specified entity from the list of managed entities.
       * @param c_entity The entity to remove.
       */
      void RemoveEntity(CRABEquippedEntity& c_entity);

      /**
       * Returns an immutable vector of RAB entities that can communicated with the given entity.
       * @param c_entity The wanted entity.
       * @return An immutable vector of RAB entities that can communicated with the given entity.       
       * @throws CARGoSException If the passed entity is not managed by this medium.
       */
      const CSet<CRABEquippedEntity*,SEntityComparator>& GetRABsCommunicatingWith(CRABEquippedEntity& c_entity) const;

   protected:

      /** Defines the routing table */
      typedef unordered_map<ssize_t, CSet<CRABEquippedEntity*,SEntityComparator> > TRoutingTable;

      /** The routing table, that associates each RAB with the RABs that can send a message to it */
      TRoutingTable m_tRoutingTable;

      /** A positional index for the RAB entities */
      CPositionalIndex<CRABEquippedEntity>* m_pcRABEquippedEntityIndex;

      /** The update operation for the grid positional index */
      CRABEquippedEntityGridEntityUpdater* m_pcRABEquippedEntityGridUpdateOperation;

      /* Whether occlusions should be considered or not */
      bool m_bCheckOcclusions;

   };

}

#endif
