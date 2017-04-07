/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_engine.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "pm3d_engine.h"
#include "pm3d_model.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPM3DEngine::CPM3DEngine() :
      m_fGravity(-9.81f) {
   }

   /****************************************/
   /****************************************/

   CPM3DEngine::~CPM3DEngine() {
   }

   /****************************************/
   /****************************************/

   void CPM3DEngine::Init(TConfigurationNode& t_tree) {
      /* Init parent */
      CPhysicsEngine::Init(t_tree);
      /* Set gravity */
      GetNodeAttributeOrDefault(t_tree, "gravity", m_fGravity, m_fGravity);
   }

   /****************************************/
   /****************************************/

   void CPM3DEngine::Reset() {
      for(CPM3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->Reset();
      }
   }

   /****************************************/
   /****************************************/

   void CPM3DEngine::Destroy() {
      /* Empty the physics entity map */
      for(CPM3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         delete it->second;
      }
      m_tPhysicsModels.clear();
   }

   /****************************************/
   /****************************************/

   void CPM3DEngine::Update() {
      /* Update the physics state from the entities */
      for(CPM3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateFromEntityStatus();
      }
      for(size_t i = 0; i < GetIterations(); ++i) {
         /* Perform the step */
         for(CPM3DModel::TMap::iterator it = m_tPhysicsModels.begin();
             it != m_tPhysicsModels.end(); ++it) {
            it->second->Step();
         }
      }
      /* Update the simulated space */
      for(CPM3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateEntityStatus();
      }
   }

   /****************************************/
   /****************************************/

   size_t CPM3DEngine::GetNumPhysicsModels() {
      return m_tPhysicsModels.size();
   }

   /****************************************/
   /****************************************/

   bool CPM3DEngine::AddEntity(CEntity& c_entity) {
      SOperationOutcome cOutcome =
         CallEntityOperation<CPM3DOperationAddEntity, CPM3DEngine, SOperationOutcome>
         (*this, c_entity);
      return cOutcome.Value;
   }

   /****************************************/
   /****************************************/

   bool CPM3DEngine::RemoveEntity(CEntity& c_entity) {
      SOperationOutcome cOutcome =
         CallEntityOperation<CPM3DOperationRemoveEntity, CPM3DEngine, SOperationOutcome>
         (*this, c_entity);
      return cOutcome.Value;
   }

   /****************************************/
   /****************************************/

   bool CPM3DEngine::IsPointContained(const CVector3& c_point) {
      return true;
   }

   /****************************************/
   /****************************************/

   bool CPM3DEngine::IsEntityTransferNeeded() const {
      return false;
   }

   /****************************************/
   /****************************************/

   void CPM3DEngine::TransferEntities() {
   }

   /****************************************/
   /****************************************/

   void CPM3DEngine::CheckIntersectionWithRay(TEmbodiedEntityIntersectionData& t_data,
                                                     const CRay3& c_ray) const {
      Real fTOnRay;
      for(CPM3DModel::TMap::const_iterator it = m_tPhysicsModels.begin();
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

   void CPM3DEngine::AddPhysicsModel(const std::string& str_id,
                                            CPM3DModel& c_model) {
      m_tPhysicsModels[str_id] = &c_model;
   }

   /****************************************/
   /****************************************/

   void CPM3DEngine::RemovePhysicsModel(const std::string& str_id) {
      CPM3DModel::TMap::iterator it = m_tPhysicsModels.find(str_id);
      if(it != m_tPhysicsModels.end()) {
         delete it->second;
         m_tPhysicsModels.erase(it);
      }
      else {
         THROW_ARGOSEXCEPTION("PM3D model id \"" << str_id << "\" not found in point-mass 3D engine \"" << GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_PHYSICS_ENGINE(CPM3DEngine,
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
                           "You can set the intensity of gravity in this engine. Gravity won't affect every\n"
                           "entity, though - only the entities that use it internally, such as quad-rotors.\n\n"
                           "Gravity acts along the Z axis. A positive value pushes entities up; a negative\n"
                           "value pushes entities down. The default value of the gravity intensity is -9.81.\n"
                           "To set the gravity intensity, use the 'gravity' attribute:\n\n"
                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <pointmass3d id=\"pm3d\" gravity=\"-9.81\"/>\n"
                           "    ...\n"
                           "  </physics_engines>\n\n",
                           "Under development"
      );

}
