/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CPiPuckSystemDefaultSensor::SetRobot(CComposableEntity& c_entity) {}

   /****************************************/
   /****************************************/
   
   void CPiPuckSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckSystemSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the PiPuck system sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CPiPuckSystemDefaultSensor::Reset() {
      m_fTime = 0.0f;
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckSystemDefaultSensor::Update() {
      m_fTime += CPhysicsEngine::GetSimulationClockTick();
   }

   /****************************************/
   /****************************************/
   
   REGISTER_SENSOR(CPiPuckSystemDefaultSensor,
                  "pipuck_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The Pi-Puck system sensor.",
                  "The documentation for this sensor is \e]8;;https://github.com/ilpincy/argos3/blob/master/src/plugins/robots/pi-puck/README.md#pipuck_system\aavailable on Github\e]8;;\a.",
                  "Usable"
   );

   /****************************************/
   /****************************************/
   
}

   
