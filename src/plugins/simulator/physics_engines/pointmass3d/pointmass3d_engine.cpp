/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_engine.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "pointmass3d_engine.h"
#include "pointmass3d_model.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPointMass3DEngine::CPointMass3DEngine() :
      m_fGravity(-9.81f) {
   }

   /****************************************/
   /****************************************/

   CPointMass3DEngine::~CPointMass3DEngine() {
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::Init(TConfigurationNode& t_tree) {
      /* Init parent */
      CPhysicsEngine::Init(t_tree);
      /* Set gravity */
      GetNodeAttributeOrDefault(t_tree, "gravity", m_fGravity, m_fGravity);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::Reset() {
      for(CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->Reset();
      }
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::Destroy() {
      /* Empty the physics entity map */
      for(CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         delete it->second;
      }
      m_tPhysicsModels.clear();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::Update() {
      /* Update the physics state from the entities */
      for(CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateFromEntityStatus();
      }
      for(size_t i = 0; i < GetIterations(); ++i) {
         /* Perform the step */
         for(CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.begin();
             it != m_tPhysicsModels.end(); ++it) {
            it->second->UpdatePhysics();
         }
      }
      for(CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->Step();
      }
      /* Update the simulated space */
      for(CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateEntityStatus();
      }
   }

   /****************************************/
   /****************************************/

   size_t CPointMass3DEngine::GetNumPhysicsModels() {
      return m_tPhysicsModels.size();
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DEngine::AddEntity(CEntity& c_entity) {
      SOperationOutcome cOutcome =
         CallEntityOperation<CPointMass3DOperationAddEntity, CPointMass3DEngine, SOperationOutcome>
         (*this, c_entity);
      return cOutcome.Value;
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DEngine::RemoveEntity(CEntity& c_entity) {
      SOperationOutcome cOutcome =
         CallEntityOperation<CPointMass3DOperationRemoveEntity, CPointMass3DEngine, SOperationOutcome>
         (*this, c_entity);
      return cOutcome.Value;
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DEngine::IsPointContained(const CVector3& c_point) {
      return true;
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DEngine::IsEntityTransferNeeded() const {
      return false;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::TransferEntities() {
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::CheckIntersectionWithRay(TEmbodiedEntityIntersectionData& t_data,
                                                     const CRay3& c_ray) const {
      Real fTOnRay;
      for(CPointMass3DModel::TMap::const_iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end();
          ++it) {
         if(it->second->CheckIntersectionWithRay(fTOnRay, c_ray)) {
            t_data.push_back(
               SEmbodiedEntityIntersectionItem(
                  &it->second->GetEmbodiedEntity(),
                  fTOnRay));
         }
      }
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::AddPhysicsModel(const std::string& str_id,
                                            CPointMass3DModel& c_model) {
      m_tPhysicsModels[str_id] = &c_model;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::RemovePhysicsModel(const std::string& str_id) {
      CPointMass3DModel::TMap::iterator it = m_tPhysicsModels.find(str_id);
      if(it != m_tPhysicsModels.end()) {
         delete it->second;
         m_tPhysicsModels.erase(it);
      }
      else {
         THROW_ARGOSEXCEPTION("PointMass3D model id \"" << str_id << "\" not found in point-mass 3D engine \"" << GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_PHYSICS_ENGINE(CPointMass3DEngine,
                           "pointmass3d",
                           "Carlo Pinciroli [ilpincy@gmail.com]",
                           "1.0",
                           "A 3D point-mass physics engine.",
                           "This physics engine is a 3D point-mass engine.\n\n"
                           "REQUIRED XML CONFIGURATION\n\n"
                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <pointmass3d id=\"pm3d\" />\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"
                           "The 'id' attribute is necessary and must be unique among the physics engines.\n"
                           "If two engines share the same id, initialization aborts.\n\n"
                           "OPTIONAL XML CONFIGURATION\n\n"
                           "None for the time being.\n\n"
                           ,
                           "Under development"
      );

}
