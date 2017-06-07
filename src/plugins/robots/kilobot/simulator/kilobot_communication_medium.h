#ifndef KILOBOT_COMMUNICATION_MEDIUM_H
#define KILOBOT_COMMUNICATION_MEDIUM_H

namespace argos {
   class CKilobotCommunicationMedium;
   class CKilobotCommunicationEntity;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/medium/medium.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/plugins/robots/kilobot/simulator/kilobot_communication_entity.h>

namespace argos {

   class CKilobotCommunicationMedium : public CMedium {

   public:

      /**
       * Class constructor.
       */
      CKilobotCommunicationMedium();

      /**
       * Class destructor.
       */
      virtual ~CKilobotCommunicationMedium();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void PostSpaceInit();
      virtual void Reset();
      virtual void Destroy();
      virtual void Update();

      /**
       * Adds the specified entity to the list of managed entities.
       * @param c_entity The entity to add.
       */
      void AddEntity(CKilobotCommunicationEntity& c_entity);
      
      /**
       * Removes the specified entity from the list of managed entities.
       * @param c_entity The entity to remove.
       */
      void RemoveEntity(CKilobotCommunicationEntity& c_entity);

      /**
       * Returns an immutable vector of entities that can communicate with the given entity.
       * @param c_entity The wanted entity.
       * @return An immutable vector of entities that can communicate with the given entity.       
       * @throws CARGoSException If the passed entity is not managed by this medium.
       */
      const CSet<CKilobotCommunicationEntity*>& GetKilobotsCommunicatingWith(CKilobotCommunicationEntity& c_entity) const;

   private:

      /** Defines the adjacency matrix */
      typedef std::map<CKilobotCommunicationEntity*, CSet<CKilobotCommunicationEntity*> > TAdjacencyMatrix;

      /** The adjacency matrix, that associates each entity with the entities that communicate with it */
      TAdjacencyMatrix m_tCommMatrix;

      /** The adjacency matrix of neighbors of a transmitting robot who are also transmitting */
      TAdjacencyMatrix m_tTxNeighbors;

      /** A positional index for the kilobot communication entities */
      CPositionalIndex<CKilobotCommunicationEntity>* m_pcKilobotIndex;

      /** The update operation for the grid positional index */
      CKilobotCommunicationEntityGridEntityUpdater* m_pcGridUpdateOperation;

      /** Random number generator */
      CRandom::CRNG* m_pcRNG;

      /** Probability of receiving a message */
      Real m_fRxProb;

   };

}

#endif
