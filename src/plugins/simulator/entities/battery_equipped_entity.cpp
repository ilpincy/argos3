/**
 * @file <argos3/plugins/simulator/entities/battery_equipped_entity.cpp>
 *
 * @author Adhavan Jayabalan <jadhu94@gmail.com>
 */
#include "battery_equipped_entity.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBatteryEquippedEntity::CBatteryEquippedEntity(CComposableEntity *pc_parent) :
      CEntity(pc_parent),
      m_fFullCharge(1.0),
      m_fAvailableCharge(m_fFullCharge),
      m_pcDischargeModel(nullptr) {
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
      m_pcDischargeModel(nullptr) {
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
      m_pcDischargeModel(nullptr) {
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
      if(CSimulator::GetInstance().GetSpace().GetSimulationClock() > 0) {
         if(m_pcDischargeModel)
            /* Call the discharge model */
            (*m_pcDischargeModel)();
      }
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
      m_pcBattery(nullptr) {
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
      GetNodeAttributeOrDefault(t_tree, "time_factor", m_fTimeFactor, m_fTimeFactor);
   }

   /****************************************/
   /****************************************/

   void CBatteryDischargeModelTime::operator()() {
      if(m_pcBattery->GetAvailableCharge() > 0.0) {
         m_pcBattery->SetAvailableCharge(
            Max<Real>(0.0,
                      m_pcBattery->GetAvailableCharge() - m_fTimeFactor));
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
         auto* cComp = dynamic_cast<CComposableEntity*>(pcRoot);
         if(cComp != nullptr) {
            auto& cBody = cComp->GetComponent<CEmbodiedEntity>("body");
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
      GetNodeAttributeOrDefault(t_tree, "time_factor", m_fTimeFactor, m_fTimeFactor);
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
         auto* cComp = dynamic_cast<CComposableEntity*>(pcRoot);
         if(cComp != nullptr) {
            auto& cBody = cComp->GetComponent<CEmbodiedEntity>("body");
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
               m_fTimeFactor -
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

   std::string CBatteryEquippedEntity::GetQueryDocumentation(const SDocumentationQuerySpec& c_spec) {
     std::string strDoc =
         "You can configure the battery of the " + c_spec.strEntityName + ". By default, the"
         " battery never\ndepletes. The following battery discharge models are available:\n\n"

         "- 'time' - The battery depletes by a 'time_factor' amount at each time step. The\n"
         "           'time_factor' attribute is optional, and defaults to 1e-5 if omitted.\n"
         "           Units of charge/timestep.\n\n"

         "- 'motion' - The battery depletes according to how the " + c_spec.strEntityName + " moves. The 'pos_factor'\n"
         "             and 'orient_factor' attributes can be used to specify how much the\n"
         "             battery should deplete to to changes in position and orientation,\n"
         "             respectively. Units of charge/meter and charge/radian, respectively.\n"
         "             They both default to 1e-3 if omitted.\n\n"

         "- 'time_motion' - A combination of the 'time' and 'motion' models.\n\n"

         "You can define your own models too. Follow the examples in the file\n"
         "argos3/src/plugins/simulator/entities/battery_equipped_entity.cpp.\n\n"

         "You can also specify the initial battery charge with the 'start_charge' attribute\n"
         "and the maximum battery charge wth the 'full_charge' attribute. These both default\n"
         "to 1.0 if omitted.\n\n";

         std::string strExamples =
         "Example XML battery configurations (default values shown):\n\n"

         "  <arena>\n"
         "    ...\n"
         "    <" + c_spec.strEntityName + " ...>\n"
         "      ...\n"
         "      <!-- Time model -->\n"
         "      <battery discharge_model=\"time\"\n"
         "               time_factor=\"1e-3\"\n"
         "               start_charge=\"1.0\"\n"
         "               full_charge=\"1.0\">\n\n"
         "      <!-- Motion model -->\n"
         "      <battery discharge_model=\"motion\"\n"
         "               pos_factor=\"1e-3\"\n"
         "               orient_factor=\"1e-3\"\n"
         "               start_charge=\"1.0\"\n"
         "               full_charge=\"1.0\">\n\n"
         "      <!-- Time-motion model -->\n"
         "      <battery discharge_model=\"time_motion\"\n"
         "               time_factor=\"1e-5\"\n"
         "               pos_factor=\"1e-3\"\n"
         "               orient_factor=\"1e-3\"\n"
         "               start_charge=\"1.0\"\n"
         "               full_charge=\"1.0\">\n\n"
         "      ...\n"
         "    </" + c_spec.strEntityName + ">\n"
         "    ...\n"
         "  </arena>\n\n";
         return strDoc + strExamples;
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
