/**
 * @file <argos3/core/simulator/entity/controllable_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "controllable_entity.h"
#include <argos3/core/simulator/actuator.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   /****************************************/
   /****************************************/

   CControllableEntity::~CControllableEntity()
   {
      if(m_pcController != NULL) {
         delete m_pcController;
      }
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::Init(TConfigurationNode& t_tree)
   {
      try {
         /* Init parent */
         CEntity::Init(t_tree);
         /* Get the controller of the controllable */
         GetNodeAttribute(t_tree, "controller", m_strControllerId);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize controllable entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::Reset()
   {
      /* Clear rays */
      m_vecCheckedRays.clear();
      m_vecIntersectionPoints.clear();

      /* Reset sensors */
      for(std::map<std::string, CSensor*>::iterator it = m_mapSensors.begin();
          it != m_mapSensors.end(); ++it) {
         it->second->Reset();
      }
      /* Reset actuators */
      for(std::map<std::string, CActuator*>::iterator it = m_mapActuators.begin();
          it != m_mapActuators.end(); ++it) {
         it->second->Reset();
      }
      /* Reset user-defined controller */
      m_pcController->Reset();
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::Destroy()
   {
      /* Clear rays */
      m_vecCheckedRays.clear();
      m_vecIntersectionPoints.clear();

      /* Destroy sensors */
      for(std::map<std::string, CSensor*>::iterator it = m_mapSensors.begin();
          it != m_mapSensors.end(); ++it) {
         it->second->Destroy();
      }
      /* Destroy actuators */
      for(std::map<std::string, CActuator*>::iterator it = m_mapActuators.begin();
          it != m_mapActuators.end(); ++it) {
         it->second->Destroy();
      }
      /* Destroy user-defined controller */
      if(m_pcController) {
         m_pcController->Destroy();
      }
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::SetController(CCI_Controller& pc_controller) {
      /* Set the controller */
      m_pcController = &pc_controller;
      /* Set the simulated actuator list */
      for(CCI_Actuator::TMap::iterator it = m_pcController->GetRobot().GetAllActuators().begin();
          it != m_pcController->GetRobot().GetAllActuators().end(); ++it) {
         m_mapActuators[it->first] = dynamic_cast<CActuator*>(it->second);
      }
      /* Set the simulated sensor list */
      for(CCI_Sensor::TMap::iterator it = m_pcController->GetRobot().GetAllSensors().begin();
          it != m_pcController->GetRobot().GetAllSensors().end(); ++it) {
         m_mapSensors[it->first] = dynamic_cast<CSensor*>(it->second);
      }
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::Sense() {
      m_vecCheckedRays.clear();
      m_vecIntersectionPoints.clear();

      for(std::map<std::string, CSensor*>::iterator it = m_mapSensors.begin();
          it != m_mapSensors.end(); ++it) {
         it->second->Update();
      }
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::Act() {
      for(std::map<std::string, CActuator*>::iterator it = m_mapActuators.begin();
          it != m_mapActuators.end(); ++it) {
         it->second->Update();
      }
   }

   /****************************************/
   /****************************************/

}
