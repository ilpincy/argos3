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
      for(auto it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->Reset();
      }
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::Destroy() {
      /* Empty the physics entity map */
      for(auto it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         delete it->second;
      }
      m_tPhysicsModels.clear();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEngine::Update() {
      /* Update the physics state from the entities */
      for(auto it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->UpdateFromEntityStatus();
      }
      for(size_t i = 0; i < GetIterations(); ++i) {
         /* Perform the step */
         for(auto it = m_tPhysicsModels.begin();
             it != m_tPhysicsModels.end(); ++it) {
            it->second->UpdatePhysics();
         }
      }
      for(auto it = m_tPhysicsModels.begin();
          it != m_tPhysicsModels.end(); ++it) {
         it->second->Step();
      }
      /* Update the simulated space */
      for(auto it = m_tPhysicsModels.begin();
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
      for(auto it = m_tPhysicsModels.begin();
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
      auto it = m_tPhysicsModels.find(str_id);
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

                           "By default, there will -9.81 m/s gravity in the simulation. This can be\n"
                           "changed by specifying the 'gravity' attribute:\n\n"

                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <pointmass3d id=\"pm3d\" gravity=\"-9.81\"/>\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"

                           "Multiple physics engines can also be used. If multiple physics engines are used,\n"
                           "the disjoint union of the 3D volumes within the arena assigned to each engine must cover\n"
                           "the entire arena without overlapping. If the entire arena is not covered, robots can\n"
                           "\"escape\" the configured physics engines and cause a fatal exception (this is not an\n"
                           "issue when a single physics engine is used, because the engine covers the entire arena\n"
                           "by default). To use multiple physics engines, use the following syntax (all attributes\n"
                           "are mandatory):\n\n"

                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <pointmass3d id=\"pm0\">\n"
                           "      <boundaries>\n"
                           "        <top height=\"1.0\"/>\n"
                           "        <botton height=\"0.0\"/>\n"
                           "        <sides>\n"
                           "          <vertex point=\"0.0, 0.0\"/>\n"
                           "          <vertex point=\"4.0, 0.0\"/>\n"
                           "          <vertex point=\"4.0, 4.0\"/>\n"
                           "          <vertex point=\"0.0, 4.0\"/>\n"
                           "        </sides>\n"
                           "      </boundaries>\n"
                           "    </pointmass3d>\n"
                           "    <pointmass3d id=\"pm1\">\n"
                           "     ...\n"
                           "    </pointmass3d>\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"

                           "A physics engine can be defined having any number of sides >= 3, as long as the\n"
                           "sides from a closed polygon in the 2D plane (vertices must be declared in the\n"
                           "XML file in counter-clockwise order).  In the above example, the physics engine\n"
                           "'pm0' is assigned to the area within the arena with lower left coordinates (0,0)\n"
                           "and upper right coordinates (4,4) and vertices are specified in counter\n"
                           "clockwise order: LL, LR, UR, UL.\n\n"

                           "OPTIMIZATION HINTS\n\n"

                           "1. A single physics engine is generally sufficient for small swarms (say <= 50\n"
                           "   robots) within a reasonably small arena to obtain faster than real-time\n"
                           "   performance with optimized code. For larger swarms and/or large arenas multiple\n"
                           "   engines should be used for maximum performance.\n\n"

                           "2. In general, using the same number of ARGoS threads as physics engines gives\n"
                           "   maximum performance (1-thread per engine per CPU core).\n\n"

                           "3. Using multiple engines in simulations with any of the following characteristics\n"
                           "   generally incurs more overhead due to thread context switching than the\n"
                           "   performance benefits from multiple engines:\n\n"

                           "   - Small swarms\n"
                           "   - Small arenas\n"
                           "   - Less available ARGoS threads than assigned physics engines\n"
                           "   - Less available CPU cores than assigned ARGoS threads\n\n"


                           "4. A good starting strategy for physics engine boundary assignment is to assign\n"
                           "   each physics engine the same amount of area within the arena. This will be\n"
                           "   sufficient for most cases. Depending on the nature of the simulation, using\n"
                           "   non-homogeneous physics engine sizes may yield increased performance. An example\n"
                           "   would be a narrow hallway between larger open areas in the arena--the hallway\n"
                           "   will likely experience increased robot density and assigning more physics\n"
                           "   engines to that area than the relatively unpopulated open areas may increase\n"
                           "   performance.\n\n",

                           "Usable"
                           );

}
