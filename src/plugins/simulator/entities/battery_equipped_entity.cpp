/**
 * @file <argos3/plugins/simulator/entities/battery_equipped_entity.cpp>
 *
 * @author Adhavan Jayabalan <jadhu94@gmail.com>
 */
#include "battery_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBatteryEquippedEntity::CBatteryEquippedEntity(CComposableEntity *pc_parent) :
      CEntity(pc_parent),
      m_fFullCharge(1.0),
      m_fAvailableCharge(m_fFullCharge),
      m_pcDischargeModel(NULL) {
      SetDischargeModel(new CBatteryDischargeModelTime());
      Disable();
   }

   /****************************************/
   /****************************************/

   CBatteryEquippedEntity::CBatteryEquippedEntity(CComposableEntity *pc_parent,
                                                  const std::string &str_id,
                                                  CBatteryDischargeModel* pc_discharge_model,
                                                  Real f_start_charge,
                                                  Real f_full_charge) :
      CEntity(pc_parent, str_id),
      m_fFullCharge(f_full_charge),
      m_fAvailableCharge(f_start_charge),
      m_pcDischargeModel(NULL) {
      SetDischargeModel(pc_discharge_model);
      Disable();
   }

   /****************************************/
   /****************************************/

   CBatteryEquippedEntity::CBatteryEquippedEntity(CComposableEntity *pc_parent,
                                                  const std::string& str_id,
                                                  const std::string& str_discharge_model,
                                                  Real f_start_charge,
                                                  Real f_full_charge) :
      CEntity(pc_parent, str_id),
      m_fFullCharge(f_full_charge),
      m_fAvailableCharge(f_start_charge),
      m_pcDischargeModel(NULL) {
      SetDischargeModel(str_discharge_model);
      Disable();
   }

   /****************************************/
   /****************************************/

   CBatteryEquippedEntity::~CBatteryEquippedEntity() {
      /* Get rid of battery discharge model */
      delete m_pcDischargeModel;
   }

   /****************************************/
   /****************************************/

   void CBatteryEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         CEntity::Init(t_tree);
         /* Get initial battery level */
         std::string strDischargeModel = "time";
         GetNodeAttributeOrDefault(t_tree, "discharge_model", strDischargeModel, strDischargeModel);
         SetDischargeModel(strDischargeModel);
         m_pcDischargeModel->Init(t_tree);
         /* Get initial battery charge */
         GetNodeAttributeOrDefault(t_tree, "start_charge",  m_fAvailableCharge,  m_fAvailableCharge);
         /* Get full battery charge */
         GetNodeAttributeOrDefault(t_tree, "full_charge",   m_fFullCharge,       m_fFullCharge);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the battery sensor equipped entity \"" << GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBatteryEquippedEntity::Update() {
      if(m_pcDischargeModel)
         /* Call the discharge model */
         (*m_pcDischargeModel)();
   }

   /****************************************/
   /****************************************/

   void CBatteryEquippedEntity::SetDischargeModel(CBatteryDischargeModel* pc_model) {
      if(m_pcDischargeModel) delete m_pcDischargeModel;
      m_pcDischargeModel = pc_model;
      if(m_pcDischargeModel)
         m_pcDischargeModel->SetBattery(this);
   }

   /****************************************/
   /****************************************/

   void CBatteryEquippedEntity::SetDischargeModel(const std::string& str_model) {
      if(m_pcDischargeModel) delete m_pcDischargeModel;
      if(str_model != "") {
         m_pcDischargeModel = TFactoryBatteryDischargeModel::New(str_model);
         m_pcDischargeModel->SetBattery(this);
      }
   }

   /****************************************/
   /****************************************/

   CBatteryDischargeModel::CBatteryDischargeModel() :
      m_pcBattery(NULL) {
   }

   /****************************************/
   /****************************************/

   CBatteryDischargeModel::~CBatteryDischargeModel() {
   }

   /****************************************/
   /****************************************/

   void CBatteryDischargeModel::SetBattery(CBatteryEquippedEntity* pc_battery) {
      m_pcBattery = pc_battery;
   }

   /****************************************/
   /****************************************/

   void CBatteryDischargeModelTime::Init(TConfigurationNode& t_tree) {
      GetNodeAttributeOrDefault(t_tree, "factor", m_fDelta, m_fDelta);
   }

   /****************************************/
   /****************************************/

   void CBatteryDischargeModelTime::operator()() {
      if(m_pcBattery->GetAvailableCharge() > 0.0) {
         m_pcBattery->SetAvailableCharge(
            Max<Real>(0.0,
                      m_pcBattery->GetAvailableCharge() - m_fDelta));
      }
   }

   /****************************************/
   /****************************************/

   void CBatteryDischargeModelMotion::Init(TConfigurationNode& t_tree) {
      GetNodeAttributeOrDefault(t_tree, "pos_factor", m_fPosFactor, m_fPosFactor);
      GetNodeAttributeOrDefault(t_tree, "orient_factor", m_fOrientFactor, m_fOrientFactor);
   }

   /****************************************/
   /****************************************/

   void CBatteryDischargeModelMotion::SetBattery(CBatteryEquippedEntity* pc_battery) {
      try {
         /* Execute default logic */
         CBatteryDischargeModel::SetBattery(pc_battery);
         /* Get a hold of the body and anchor of the entity that contains the battery */
         CEntity* pcRoot = &pc_battery->GetRootEntity();
         CComposableEntity* cComp = dynamic_cast<CComposableEntity*>(pcRoot);
         if(cComp != NULL) {
            CEmbodiedEntity& cBody = cComp->GetComponent<CEmbodiedEntity>("body");
            m_psAnchor = &cBody.GetOriginAnchor();
            m_cOldPosition = m_psAnchor->Position;
         }
         else {
            THROW_ARGOSEXCEPTION("Root entity is not composable");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("While setting body for battery model \"motion\"", ex);
      }
   }

   /****************************************/
   /****************************************/
      
   void CBatteryDischargeModelMotion::operator()() {
      if(m_pcBattery->GetAvailableCharge() > 0.0) {
         /* Calculate delta position */
         Real fDeltaPos = Distance(m_psAnchor->Position,
                                   m_cOldPosition);
         /* Calculate delta orientation */
         CQuaternion cDeltaOrient =
            m_cOldOrientation.Inverse() *
            m_psAnchor->Orientation;
         CRadians cDeltaAngle;
         CVector3 cDeltaAxis;
         cDeltaOrient.ToAngleAxis(cDeltaAngle, cDeltaAxis);
         /* Calculate new level */
         m_pcBattery->SetAvailableCharge(
            Max<Real>(
               0.0,
               m_pcBattery->GetAvailableCharge() -
               m_fPosFactor * fDeltaPos -
               m_fOrientFactor * cDeltaAngle.GetValue()
               ));
         /* Save position for next step */
         m_cOldPosition = m_psAnchor->Position;
         m_cOldOrientation = m_psAnchor->Orientation;
      }
   }
   
   /****************************************/
   /****************************************/

   void CBatteryDischargeModelTimeMotion::Init(TConfigurationNode& t_tree) {
      GetNodeAttributeOrDefault(t_tree, "time_factor", m_fDelta, m_fDelta);
      GetNodeAttributeOrDefault(t_tree, "pos_factor", m_fPosFactor, m_fPosFactor);
      GetNodeAttributeOrDefault(t_tree, "orient_factor", m_fOrientFactor, m_fOrientFactor);
   }

   /****************************************/
   /****************************************/

   void CBatteryDischargeModelTimeMotion::SetBattery(CBatteryEquippedEntity* pc_battery) {
      try {
         /* Execute default logic */
         CBatteryDischargeModel::SetBattery(pc_battery);
         /* Get a hold of the body and anchor of the entity that contains the battery */
         CEntity* pcRoot = &pc_battery->GetRootEntity();
         CComposableEntity* cComp = dynamic_cast<CComposableEntity*>(pcRoot);
         if(cComp != NULL) {
            CEmbodiedEntity& cBody = cComp->GetComponent<CEmbodiedEntity>("body");
            m_psAnchor = &cBody.GetOriginAnchor();
            m_cOldPosition = m_psAnchor->Position;
         }
         else {
            THROW_ARGOSEXCEPTION("Root entity is not composable");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("While setting body for battery model \"time_motion\"", ex);
      }
   }

   /****************************************/
   /****************************************/
      
   void CBatteryDischargeModelTimeMotion::operator()() {
      if(m_pcBattery->GetAvailableCharge() > 0.0) {
         /* Calculate delta position */
         Real fDeltaPos = Distance(m_psAnchor->Position,
                                   m_cOldPosition);
         /* Calculate delta orientation */
         CQuaternion cDeltaOrient =
            m_cOldOrientation.Inverse() *
            m_psAnchor->Orientation;
         CRadians cDeltaAngle;
         CVector3 cDeltaAxis;
         cDeltaOrient.ToAngleAxis(cDeltaAngle, cDeltaAxis);
         /* Calculate new level */
         m_pcBattery->SetAvailableCharge(
            Max<Real>(
               0.0,
               m_pcBattery->GetAvailableCharge() -
               m_fDelta -
               m_fPosFactor * fDeltaPos -
               m_fOrientFactor * cDeltaAngle.GetValue()
               ));
         /* Save position for next step */
         m_cOldPosition = m_psAnchor->Position;
         m_cOldOrientation = m_psAnchor->Orientation;
      }
   }
   
   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CBatteryEquippedEntity);
   REGISTER_BATTERY_DISCHARGE_MODEL(CBatteryDischargeModelTime,       "time");
   REGISTER_BATTERY_DISCHARGE_MODEL(CBatteryDischargeModelMotion,     "motion");
   REGISTER_BATTERY_DISCHARGE_MODEL(CBatteryDischargeModelTimeMotion, "time_motion");

   /****************************************/
   /****************************************/

}
