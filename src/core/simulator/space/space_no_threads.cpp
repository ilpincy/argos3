/**
 * @file <argos3/core/simulator/space/space_no_threads.cpp>
 *
 * @brief This file provides the implementation of the Swarmanoid 3D Space.
 *
 * This file provides the implementation of the Swarmanoid 3D Space.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "space_no_threads.h"
#include <argos3/core/simulator/simulator.h>

namespace argos {

   /****************************************/
   /****************************************/

   CSpaceNoThreads::CSpaceNoThreads() {
     LOG << "[INFO] Not using threads" << std::endl;
   }

   /****************************************/
   /****************************************/

   void CSpaceNoThreads::UpdateControllableEntitiesAct() {
      for(size_t i = 0; i < m_vecControllableEntities.size(); ++i) {
         if(m_vecControllableEntities[i]->IsEnabled())
            m_vecControllableEntities[i]->Act();
      }
   }

   /****************************************/
   /****************************************/

   void CSpaceNoThreads::UpdatePhysics() {
      /* Update the physics engines */
      for(size_t i = 0; i < m_ptPhysicsEngines->size(); ++i) {
         (*m_ptPhysicsEngines)[i]->Update();
      }
      /* Perform entity transfer from engine to engine, if needed */
      for(size_t i = 0; i < m_ptPhysicsEngines->size(); ++i) {
         if((*m_ptPhysicsEngines)[i]->IsEntityTransferNeeded()) {
            (*m_ptPhysicsEngines)[i]->TransferEntities();
         }
      }
   }

   /****************************************/
   /****************************************/

   void CSpaceNoThreads::UpdateMedia() {
      for(size_t i = 0; i < m_ptMedia->size(); ++i) {
         (*m_ptMedia)[i]->Update();
      }
   }

   /****************************************/
   /****************************************/

   void CSpaceNoThreads::UpdateControllableEntitiesSenseStep() {
      for(size_t i = 0; i < m_vecControllableEntities.size(); ++i) {
         if(m_vecControllableEntities[i]->IsEnabled()) {
            m_vecControllableEntities[i]->Sense();
            m_vecControllableEntities[i]->ControlStep();
         }
      }
   }

   /****************************************/
   /****************************************/

   void CSpaceNoThreads::IterateOverControllableEntities(
       const TControllableEntityIterCBType& c_cb) {
     for (size_t i = 0; i < m_vecControllableEntities.size(); ++i) {
       c_cb(m_vecControllableEntities[i]);
     } /* for(i..) */
   } /* IterateOverControllableEntities() */

   /****************************************/
   /****************************************/

}
