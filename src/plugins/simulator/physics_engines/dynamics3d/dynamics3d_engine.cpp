/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_engine.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_plugin.h>

#include <algorithm>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DEngine::CDynamics3DEngine() :
      m_pcRNG(nullptr),
      m_cRandomSeedRange(0,1000),
      m_fDefaultFriction(1.0f),
      m_cBroadphase(),
      m_cConfiguration(),
      m_cDispatcher(&m_cConfiguration),
      m_cSolver(),
      m_cWorld(&m_cDispatcher,
               &m_cBroadphase,
               &m_cSolver,
               &m_cConfiguration) {}

   /****************************************/
   /****************************************/

   void CDynamics3DEngine::Init(TConfigurationNode& t_tree) {
      /* Initialize parent */
      CPhysicsEngine::Init(t_tree);
      /* Create the random number generator */
      m_pcRNG = CRandom::CreateRNG("argos");
      /* Set random seed */
      m_cSolver.setRandSeed(m_pcRNG->Uniform(m_cRandomSeedRange));
      /* Disable gravity by default */
      m_cWorld.setGravity(btVector3(0.0,0.0,0.0));
      /* Load the plugins */
      TConfigurationNodeIterator tPluginIterator;
      for(tPluginIterator = tPluginIterator.begin(&t_tree);
          tPluginIterator != tPluginIterator.end();
          ++tPluginIterator) {
         CDynamics3DPlugin* pcPlugin = CFactory<CDynamics3DPlugin>::New(tPluginIterator->Value());
         pcPlugin->SetEngine(*this);
         pcPlugin->Init(*tPluginIterator);
         AddPhysicsPlugin(tPluginIterator->Value(), *pcPlugin);
      }
      GetNodeAttributeOrDefault(t_tree, "debug_file", m_strDebugFilename, m_strDebugFilename);
      GetNodeAttributeOrDefault(t_tree, "default_friction", m_fDefaultFriction, m_fDefaultFriction);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEngine::PostSpaceInit() {
      /* Call reset to set up the simulation */
      Reset();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEngine::Reset() {
      /* Remove and reset all physics models */
      for(CDynamics3DModel::TMap::iterator itModel = std::begin(m_tPhysicsModels);
          itModel != std::end(m_tPhysicsModels);
          ++itModel) {
         /* Remove model from plugins */
         for(CDynamics3DPlugin::TMap::iterator itPlugin = std::begin(m_tPhysicsPlugins);
             itPlugin != std::end(m_tPhysicsPlugins);
             ++itPlugin) {
            itPlugin->second->UnregisterModel(*itModel->second);
         }
         /* Remove model from world */
         itModel->second->RemoveFromWorld(m_cWorld);
         /* Reset the model */
         itModel->second->Reset();
      }
      /* Run the destructors on bullet's components */
      m_cWorld.~btMultiBodyDynamicsWorld();
      m_cSolver.~btMultiBodyConstraintSolver();
      m_cDispatcher.~btCollisionDispatcher();
      m_cConfiguration.~btDefaultCollisionConfiguration();
      m_cBroadphase.~btDbvtBroadphase();
      /* Rerun the constructors for the bullet's components */
      new (&m_cBroadphase) btDbvtBroadphase;
      new (&m_cConfiguration) btDefaultCollisionConfiguration;
      new (&m_cDispatcher) btCollisionDispatcher(&m_cConfiguration);
      new (&m_cSolver) btMultiBodyConstraintSolver;
      new (&m_cWorld) btMultiBodyDynamicsWorld(&m_cDispatcher,
                                               &m_cBroadphase,
                                               &m_cSolver,
                                               &m_cConfiguration);
      /* Provide the same random seed to the solver */
      m_cSolver.setRandSeed(m_pcRNG->Uniform(m_cRandomSeedRange));
      /* Reset the plugins */
      for(CDynamics3DPlugin::TMap::iterator itPlugin = std::begin(m_tPhysicsPlugins);
          itPlugin != std::end(m_tPhysicsPlugins);
          ++itPlugin) {
         itPlugin->second->Reset();
      }
      /* Set up the call back for the plugins */
      m_cWorld.setInternalTickCallback([] (btDynamicsWorld* pc_world, btScalar f_time_step) {
         CDynamics3DEngine* pc_engine = static_cast<CDynamics3DEngine*>(pc_world->getWorldUserInfo());
         pc_world->clearForces();
         for(std::pair<const std::string, CDynamics3DPlugin*>& c_plugin :
             pc_engine->GetPhysicsPlugins()) {
            c_plugin.second->Update();
         }
      }, static_cast<void*>(this), true);
      /* Add the models back into the engine */
      for(CDynamics3DModel::TMap::iterator itModel = std::begin(m_tPhysicsModels);
          itModel != std::end(m_tPhysicsModels);
          ++itModel) {
         /* Add model to plugins */
         for(CDynamics3DPlugin::TMap::iterator itPlugin = std::begin(m_tPhysicsPlugins);
             itPlugin != std::end(m_tPhysicsPlugins);
             ++itPlugin) {
            itPlugin->second->RegisterModel(*itModel->second);
         }
         /* Add model to world */
         itModel->second->AddToWorld(m_cWorld);
      }
      /* Initialize any multi-body constraints */
      for (SInt32 i = 0; i < m_cWorld.getNumMultiBodyConstraints(); i++) {
         m_cWorld.getMultiBodyConstraint(i)->finalizeMultiDof();
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEngine::Destroy() {
      /* Destroy all physics models */
      for(CDynamics3DModel::TMap::iterator itModel = std::begin(m_tPhysicsModels);
          itModel != std::end(m_tPhysicsModels);
          ++itModel) {
         /* Remove model from the plugins first */
         for(CDynamics3DPlugin::TMap::iterator itPlugin = std::begin(m_tPhysicsPlugins);
             itPlugin != std::end(m_tPhysicsPlugins);
             ++itPlugin) {
            itPlugin->second->UnregisterModel(*itModel->second);
         }
         /* Destroy the model */
         itModel->second->RemoveFromWorld(m_cWorld);
         delete itModel->second;
      }
      /* Destroy all plug-ins */
      for(CDynamics3DPlugin::TMap::iterator itPlugin = std::begin(m_tPhysicsPlugins);
          itPlugin != std::end(m_tPhysicsPlugins);
          ++itPlugin) {
         itPlugin->second->Destroy();
         delete itPlugin->second;
      }
      /* Empty the maps */
      m_tPhysicsPlugins.clear();
      m_tPhysicsModels.clear();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEngine::Update() {
      /* Update the physics state from the entities */
      for(CDynamics3DModel::TMap::iterator it = m_tPhysicsModels.begin();
          it != std::end(m_tPhysicsModels); ++it) {
         it->second->UpdateFromEntityStatus();
      }
      /* Step the simuation forwards */
      m_cWorld.stepSimulation(GetSimulationClockTick(),
                              GetIterations(),
                              GetPhysicsClockTick());
      /* Update the simulated space */
      for(CDynamics3DModel::TMap::iterator it = std::begin(m_tPhysicsModels);
          it != std::end(m_tPhysicsModels);
          ++it) {
         it->second->UpdateEntityStatus();
      }
      /* Dump the state of the world to a bullet file (if requested) */
      if(!m_strDebugFilename.empty()) {
         btDefaultSerializer cSerializer;
         m_cWorld.serialize(&cSerializer);
         std::ofstream cDebugOutput(m_strDebugFilename);
         if(cDebugOutput.is_open()) {
            cDebugOutput.write(reinterpret_cast<const char*>(cSerializer.getBufferPointer()),
                               cSerializer.getCurrentBufferSize());
         }
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEngine::CheckIntersectionWithRay(TEmbodiedEntityIntersectionData& t_data,
                                                    const CRay3& c_ray) const {
      /* Convert the start and end ray vectors to the bullet coordinate system */
      btVector3 cRayStart(c_ray.GetStart().GetX(), c_ray.GetStart().GetZ(), -c_ray.GetStart().GetY());
      btVector3 cRayEnd(c_ray.GetEnd().GetX(), c_ray.GetEnd().GetZ(), -c_ray.GetEnd().GetY());
      btCollisionWorld::ClosestRayResultCallback cResult(cRayStart, cRayEnd);
      /* The default flag/algorithm 'kF_UseSubSimplexConvexCastRaytest' is too approximate for
         our purposes */
      cResult.m_flags |= btTriangleRaycastCallback::kF_UseGjkConvexCastRaytest;
      /* Run the ray test */
      m_cWorld.rayTest(cRayStart, cRayEnd, cResult);
      /* Examine the results */
      if (cResult.hasHit() && cResult.m_collisionObject->getUserPointer() != nullptr) {
         Real f_t = (cResult.m_hitPointWorld - cRayStart).length() / c_ray.GetLength();
         CDynamics3DModel* pcModel =
            static_cast<CDynamics3DModel*>(cResult.m_collisionObject->getUserPointer());
         t_data.push_back(SEmbodiedEntityIntersectionItem(&(pcModel->GetEmbodiedEntity()), f_t));
      }
   }

   /****************************************/
   /****************************************/

   size_t CDynamics3DEngine::GetNumPhysicsModels() {
      return m_tPhysicsModels.size();
   }

   /****************************************/
   /****************************************/

   bool CDynamics3DEngine::AddEntity(CEntity& c_entity) {
      SOperationOutcome cOutcome =
         CallEntityOperation<CDynamics3DOperationAddEntity, CDynamics3DEngine, SOperationOutcome>
         (*this, c_entity);
      return cOutcome.Value;
   }

   /****************************************/
   /****************************************/

   bool CDynamics3DEngine::RemoveEntity(CEntity& c_entity) {
      SOperationOutcome cOutcome =
         CallEntityOperation<CDynamics3DOperationRemoveEntity, CDynamics3DEngine, SOperationOutcome>
         (*this, c_entity);
      return cOutcome.Value;
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEngine::AddPhysicsModel(const std::string& str_id,
                                           CDynamics3DModel& c_model) {
      /* Add model to world */
      c_model.AddToWorld(m_cWorld);
      /* Notify the plugins of the added model */
      for(CDynamics3DPlugin::TMap::iterator itPlugin = std::begin(m_tPhysicsPlugins);
          itPlugin != std::end(m_tPhysicsPlugins);
          ++itPlugin) {
         itPlugin->second->RegisterModel(c_model);
      }
      /* Add a pointer to the model to the map of models */
      m_tPhysicsModels[str_id] = &c_model;
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEngine::RemovePhysicsModel(const std::string& str_id) {
      CDynamics3DModel::TMap::iterator itModel = m_tPhysicsModels.find(str_id);
      if(itModel != std::end(m_tPhysicsModels)) {
         /* Notify the plugins of model removal */
         for(CDynamics3DPlugin::TMap::iterator itPlugin = std::begin(m_tPhysicsPlugins);
             itPlugin != std::end(m_tPhysicsPlugins);
             ++itPlugin) {
            itPlugin->second->UnregisterModel(*(itModel->second));
         }
         /* Remove the model from world */
         itModel->second->RemoveFromWorld(m_cWorld);
         /* Destroy the model */
         delete itModel->second;
         /* Remove the model from the map */
         m_tPhysicsModels.erase(itModel);
      }
      else {
         THROW_ARGOSEXCEPTION("The model \"" << str_id <<
                              "\" was not found in the dynamics 3D engine \"" <<
                              GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEngine::AddPhysicsPlugin(const std::string& str_id,
                                            CDynamics3DPlugin& c_plugin) {
      m_tPhysicsPlugins[str_id] = &c_plugin;
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEngine::RemovePhysicsPlugin(const std::string& str_id) {
      CDynamics3DPlugin::TMap::iterator it = m_tPhysicsPlugins.find(str_id);
      if(it != std::end(m_tPhysicsPlugins)) {
         delete it->second;
         m_tPhysicsPlugins.erase(it);
      }
      else {
         THROW_ARGOSEXCEPTION("The plugin \"" << str_id <<
                              "\" was not found in the dynamics 3D engine \"" <<
                              GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_PHYSICS_ENGINE(CDynamics3DEngine,
                           "dynamics3d",
                           "Michael Allwright [allsey87@gmail.com]",
                           "1.0",
                           "A 3D dynamics physics engine",
                           "This physics engine is a 3D dynamics engine based on the Bullet Physics SDK\n"
                           "(https://github.com/bulletphysics/bullet3).\n\n"

                           "REQUIRED XML CONFIGURATION\n\n"
                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics3d id=\"dyn3d\" />\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"

                           "The 'id' attribute is necessary and must be unique among the physics engines.\n\n"

                           "OPTIONAL XML CONFIGURATION\n\n"

                           "It is possible to change the default friction used in the simulation from\n"
                           "its initial value of 1.0 using the default_friction attribute as shown\n"
                           "below. For debugging purposes, it is also possible to provide a filename\n"
                           "via the debug_file attribute which will cause the Bullet world to be\n"
                           "serialized and written out to a file at the end of each step. This file can\n"
                           "then be opened using the Bullet example browser and can provide useful\n"
                           "insights into the stability of a simulation.\n\n"

                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics3d id=\"dyn3d\"\n"
                           "                default_friction=\"1.0\"\n"
                           "                debug_file=\"dynamics3d.bullet\"/>\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"

                           "The physics engine supports a number of plugins that add features to the\n"
                           "simulation. In the example below, a floor plane has been added which has a\n"
                           "height of 1 cm and the dimensions of the floor as specified by the arena\n"
                           "node. It is possible to change the coefficient of friction of the floor\n"
                           "using the friction attribute. This will override the default friction used\n"
                           "by the physics engine. By default, there will be no gravity in the\n"
                           "simulation. This can be changed by adding a gravity node with a single\n"
                           "attribute 'g' which is the downwards acceleration caused by gravity.\n"
                           "Finally, there is a magnetism plugin. This plugin applies forces and\n"
                           "torques to bodies in the simulation that contains magnetic dipoles. The\n"
                           "'max_distance' attribute is an optional optimization that sets the maximum\n"
                           "distance at which two magnetic dipoles will interact with each other. In\n"
                           "the example below, this distance has been set to 4 cm.\n\n"

                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics3d id=\"dyn3d\" default_friction=\"2.0\">\n"
                           "      <floor height=\"0.01\" friction=\"0.05\"/>\n"
                           "      <gravity g=\"10\" />\n"
                           "      <magnetism max_distance=\"0.04\" />\n"
                           "    </dynamics3d>\n"
                           "    ...\n"
                           "  </physics_engines>\n\n",

                           "Usable"
      );

   /****************************************/
   /****************************************/

}
