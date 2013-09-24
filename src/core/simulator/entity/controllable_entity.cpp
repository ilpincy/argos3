/**
 * @file <argos3/core/simulator/entity/controllable_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "controllable_entity.h"
#include <argos3/core/simulator/actuator.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CControllableEntity::CControllableEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent),
      m_pcController(NULL) {}

   /****************************************/
   /****************************************/

   CControllableEntity::CControllableEntity(CComposableEntity* pc_parent,
                                            const std::string& str_id) :
      CEntity(pc_parent, str_id),
      m_pcController(NULL) {
   }

   /****************************************/
   /****************************************/

   CControllableEntity::~CControllableEntity() {
      if(m_pcController != NULL) {
         delete m_pcController;
      }
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CEntity::Init(t_tree);
         /* Get the controller id */
         std::string strControllerId;
         GetNodeAttribute(t_tree, "config", strControllerId);
         /* Check if the tree has parameters to pass to the controller */
         if(NodeExists(t_tree, "params")) {
            /* Set the controller */
            SetController(strControllerId,
                          GetNode(t_tree,
                                  "params"));
         }
         else {
            /* Set the controller */
            SetController(strControllerId);
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize controllable entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::Reset() {
      /* Clear rays */
      m_vecCheckedRays.clear();
      m_vecIntersectionPoints.clear();
      /* Reset sensors */
      for(CCI_Sensor::TMap::iterator it = m_pcController->GetAllSensors().begin();
          it != m_pcController->GetAllSensors().end(); ++it) {
         it->second->Reset();
      }
      /* Reset actuators */
      for(CCI_Actuator::TMap::iterator it = m_pcController->GetAllActuators().begin();
          it != m_pcController->GetAllActuators().end(); ++it) {
         it->second->Reset();
      }
      /* Reset user-defined controller */
      m_pcController->Reset();
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::Destroy() {
      /* Clear rays */
      m_vecCheckedRays.clear();
      m_vecIntersectionPoints.clear();
      if(m_pcController) {
         /* Destroy sensors */
         for(CCI_Sensor::TMap::iterator it = m_pcController->GetAllSensors().begin();
             it != m_pcController->GetAllSensors().end(); ++it) {
            it->second->Destroy();
         }
         /* Destroy actuators */
         for(CCI_Actuator::TMap::iterator it = m_pcController->GetAllActuators().begin();
             it != m_pcController->GetAllActuators().end(); ++it) {
            it->second->Destroy();
         }
         /* Destroy user-defined controller */
         m_pcController->Destroy();
      }
   }

   /****************************************/
   /****************************************/
   
   const CCI_Controller& CControllableEntity::GetController() const {
      if(m_pcController != NULL) {
         return *m_pcController;
      }
      else {
         THROW_ARGOSEXCEPTION("Entity " << GetId() << " does not have any controller associated.");
      }
   }
   
   /****************************************/
   /****************************************/
   
   CCI_Controller& CControllableEntity::GetController() {
      if(m_pcController != NULL) {
         return *m_pcController;
      }
      else {
         THROW_ARGOSEXCEPTION("Entity " << GetId() << " does not have any controller associated.");
      }
   }
   
   /****************************************/
   /****************************************/
   
   void CControllableEntity::SetController(const std::string& str_controller_id) {
      TConfigurationNode& tConfig = CSimulator::GetInstance().GetConfigForController(str_controller_id);
      TConfigurationNode& tParams = GetNode(tConfig, "params");
      SetController(str_controller_id, tParams);
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::SetController(const std::string& str_controller_id,
                                           TConfigurationNode& t_controller_config) {
      try {
         /* Look in the map for the parsed XML configuration of the wanted controller */
         TConfigurationNode& tConfig = CSimulator::GetInstance().GetConfigForController(str_controller_id);
         /* tConfig is the base of the XML section of the wanted controller */
         std::string strImpl;
         /* Create the controller */
         m_pcController = CFactory<CCI_Controller>::New(tConfig.Value());
         m_pcController->SetId(GetParent().GetId());
         /* Go through actuators */
         TConfigurationNode& tActuators = GetNode(tConfig, "actuators");
         TConfigurationNodeIterator itAct;
         for(itAct = itAct.begin(&tActuators);
             itAct != itAct.end();
             ++itAct) {
            /* itAct->Value() is the name of the current actuator */
            GetNodeAttribute(*itAct, "implementation", strImpl);
            CSimulatedActuator* pcAct = CFactory<CSimulatedActuator>::New(itAct->Value() + " (" + strImpl + ")");
            CCI_Actuator* pcCIAct = dynamic_cast<CCI_Actuator*>(pcAct);
            if(pcCIAct == NULL) {
               THROW_ARGOSEXCEPTION("BUG: actuator \"" << itAct->Value() << "\" does not inherit from CCI_Actuator");
            }
            pcAct->SetRobot(GetParent());
            pcCIAct->Init(*itAct);
            m_mapActuators[itAct->Value()] = pcAct;
            m_pcController->AddActuator(itAct->Value(), pcCIAct);
         }
         /* Go through sensors */
         TConfigurationNode& tSensors = GetNode(tConfig, "sensors");
         TConfigurationNodeIterator itSens;
         for(itSens = itSens.begin(&tSensors);
             itSens != itSens.end();
             ++itSens) {
            /* itSens->Value() is the name of the current actuator */
            GetNodeAttribute(*itSens, "implementation", strImpl);
            CSimulatedSensor* pcSens = CFactory<CSimulatedSensor>::New(itSens->Value() + " (" + strImpl + ")");
            CCI_Sensor* pcCISens = dynamic_cast<CCI_Sensor*>(pcSens);
            if(pcCISens == NULL) {
               THROW_ARGOSEXCEPTION("BUG: sensor \"" << itSens->Value() << "\" does not inherit from CCI_Sensor");
            }
            pcSens->SetRobot(GetParent());
            pcCISens->Init(*itSens);
            m_mapSensors[itSens->Value()] = pcSens;
            m_pcController->AddSensor(itSens->Value(), pcCISens);
         }
         /* Configure the controller */
         m_pcController->Init(t_controller_config);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set controller for controllable entity \"" << GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::Sense() {
      m_vecCheckedRays.clear();
      m_vecIntersectionPoints.clear();
      for(std::map<std::string, CSimulatedSensor*>::iterator it = m_mapSensors.begin();
          it != m_mapSensors.end(); ++it) {
         it->second->Update();
      }
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::ControlStep() {
      if(m_pcController != NULL) {
         m_pcController->ControlStep();
      }
      else {
         THROW_ARGOSEXCEPTION("Entity " << GetId() << " does not have any controller associated.");
      }
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::Act() {
      for(std::map<std::string, CSimulatedActuator*>::iterator it = m_mapActuators.begin();
          it != m_mapActuators.end(); ++it) {
         it->second->Update();
      }
   }

   /****************************************/
   /****************************************/

   /**
    * @cond HIDDEN_SYMBOLS
    */
   class CSpaceOperationAddControllableEntity : public CSpaceOperationAddEntity {
   public:
      void ApplyTo(CSpace& c_space, CControllableEntity& c_entity) {
         c_space.AddEntity(c_entity);
         c_space.AddControllableEntity(c_entity);
      }
   };
   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity,
                            CSpaceOperationAddControllableEntity,
                            CControllableEntity);

   class CSpaceOperationRemoveControllableEntity : public CSpaceOperationRemoveEntity {
   public:
      void ApplyTo(CSpace& c_space, CControllableEntity& c_entity) {
         c_space.RemoveControllableEntity(c_entity);
         c_space.RemoveEntity(c_entity);
      }
   };
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity,
                            CSpaceOperationRemoveControllableEntity,
                            CControllableEntity);
   /**
    * @endcond
    */

   /****************************************/
   /****************************************/

}
