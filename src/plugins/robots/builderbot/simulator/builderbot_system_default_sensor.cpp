/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotSystemDefaultSensor::CBuilderBotSystemDefaultSensor() {}
 
   /****************************************/
   /****************************************/

   CBuilderBotSystemDefaultSensor::~CBuilderBotSystemDefaultSensor() {}
   
   /****************************************/
   /****************************************/

   void CBuilderBotSystemDefaultSensor::SetRobot(CComposableEntity& c_entity) {}

   /****************************************/
   /****************************************/
   
   void CBuilderBotSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotSystemSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot system sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotSystemDefaultSensor::Reset() {
      m_fTime = 0.0f;
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotSystemDefaultSensor::Update() {
      m_fTime += CPhysicsEngine::GetSimulationClockTick();
   }

   /****************************************/
   /****************************************/
   
   REGISTER_SENSOR(CBuilderBotSystemDefaultSensor,
                  "builderbot_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot system sensor.",
                  "This sensor provides access to the state of the builderbot.",
                  "Usable"
   );

   /****************************************/
   /****************************************/
   
}

   
