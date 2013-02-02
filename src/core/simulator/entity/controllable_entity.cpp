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
                                            const std::string& str_id,
                                            const std::string& str_controller_id) :
      CEntity(pc_parent, str_id),
      m_pcController(NULL) {
      SetController(str_controller_id);
   }

   /****************************************/
   /****************************************/

   CControllableEntity::CControllableEntity(CComposableEntity* pc_parent,
                                            const std::string& str_id,
                                            const std::string& str_controller_id,
                                            TConfigurationNode& t_controller_config) :
      CEntity(pc_parent, str_id),
      m_pcController(NULL) {
      SetController(str_controller_id, t_controller_config);
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
         GetNodeAttribute(t_tree, "controller", strControllerId);
         /* Check if the tree has parameters to pass to the controller */
         if(NodeExists(t_tree, "controller_parameters")) {
            /* Set the controller */
            SetController(strControllerId,
                          GetNode(t_tree,
                                  "controller_parameters"));
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

   void CControllableEntity::Destroy() {
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
   
   void CControllableEntity::SetController(const std::string& str_controller_id) {
      try {
         TConfigurationNode& tConfig = CSimulator::GetInstance().GetConfigForController(str_controller_id);
         TConfigurationNode& tParams = GetNode(tConfig, "params");
         SetController(str_controller_id, tParams);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set controller for controllable entity \"" << GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CControllableEntity::SetController(const std::string& str_controller_id,
                                           TConfigurationNode& t_parameters) {
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
            CActuator* pcAct = CFactory<CActuator>::New(itAct->Value() + "$$" + strImpl);
            pcAct->SetEntity(GetParent());
            m_mapActuators[itAct->Value()] = pcAct;
            pcAct->Init(*itAct);
            m_pcController->AddActuator(itAct->Value(), pcAct);
         }
         /* Go through sensors */
         TConfigurationNode& tSensors = GetNode(tConfig, "sensors");
         TConfigurationNodeIterator itSens;
         for(itSens = itSens.begin(&tSensors);
             itSens != itSens.end();
             ++itSens) {
            /* itSens->Value() is the name of the current actuator */
            GetNodeAttribute(*itSens, "implementation", strImpl);
            CSensor* pcSens = CFactory<CSensor>::New(itSens->Value() + "$$" + strImpl);
            pcSens->SetEntity(GetParent());
            m_mapSensors[itSens->Value()] = pcSens;
            pcSens->Init(*itSens);
            m_pcController->AddSensor(itSens->Value(), pcSens);
         }
         /* Configure the controller */
         m_pcController->Init(t_parameters);
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
      for(std::map<std::string, CSensor*>::iterator it = m_mapSensors.begin();
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
      for(std::map<std::string, CActuator*>::iterator it = m_mapActuators.begin();
          it != m_mapActuators.end(); ++it) {
         it->second->Update();
      }
   }

   /****************************************/
   /****************************************/

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

   /****************************************/
   /****************************************/

}
