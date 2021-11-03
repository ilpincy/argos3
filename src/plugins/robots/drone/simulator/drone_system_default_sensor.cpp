/**
 * @file <argos3/plugins/robots/drone/simulator/drone_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDroneSystemDefaultSensor::CDroneSystemDefaultSensor() {}
 
   /****************************************/
   /****************************************/

   CDroneSystemDefaultSensor::~CDroneSystemDefaultSensor() {}
   
   /****************************************/
   /****************************************/

   void CDroneSystemDefaultSensor::SetRobot(CComposableEntity& c_entity) {}

   /****************************************/
   /****************************************/
   
   void CDroneSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_DroneSystemSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the Drone system sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CDroneSystemDefaultSensor::Reset() {
      m_fTime = 0.0f;
   }

   /****************************************/
   /****************************************/
   
   void CDroneSystemDefaultSensor::Update() {
      m_fTime += CPhysicsEngine::GetSimulationClockTick();
   }

   /****************************************/
   /****************************************/
   
   REGISTER_SENSOR(CDroneSystemDefaultSensor,
                  "drone_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The drone system sensor.",
                  "This sensor provides access to the state of the drone.",
                  "Usable"
   );

   /****************************************/
   /****************************************/
   
}

   
