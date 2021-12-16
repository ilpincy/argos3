/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_entity.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/simple_radio_equipped_entity.h>
#include <argos3/plugins/simulator/entities/tag_equipped_entity.h>
#include <argos3/plugins/simulator/media/directional_led_medium.h>
#include <argos3/plugins/simulator/media/simple_radio_medium.h>
#include <argos3/plugins/simulator/media/tag_medium.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_differential_drive_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_electromagnet_system_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_lift_system_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   const Real CBuilderBotEntity::WIFI_TRANSMISSION_RANGE = 10.0;
   const CVector3 CBuilderBotEntity::WIFI_ANCHOR_OFFSET = {-0.030, 0.045, 0.11};

   const Real CBuilderBotEntity::NFC_TRANSMISSION_RANGE  = 0.02;
   const CVector3 CBuilderBotEntity::NFC_ANCHOR_OFFSET = {-0.0343, 0.0, -0.0275};
   
   const Real CBuilderBotEntity::TAG_SIDE_LENGTH = 0.0235;
   const CVector3 CBuilderBotEntity::TAG_OFFSET_POSITION = {0.0, 0.0, 0.3875};

   /****************************************/
   /****************************************/

   CBuilderBotEntity::CBuilderBotEntity() :
      CComposableEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcDifferentialDriveEntity(nullptr),
      m_pcElectromagnetSystemEntity(nullptr),
      m_pcLiftSystemEntity(nullptr),
      m_pcSimpleRadioEquippedEntity(nullptr),
      m_pcTagEquippedEntity(nullptr),
      m_bDebug(false) {}

   /****************************************/
   /****************************************/

   void CBuilderBotEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* initialize the base class */
         CComposableEntity::Init(t_tree);
         /* check if we are debugging */
         GetNodeAttributeOrDefault(t_tree, "debug", m_bDebug, m_bDebug);
         /* create and initialize the embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         /* create anchors */
         SAnchor& sLowerBaseAnchor =
            m_pcEmbodiedEntity->AddAnchor("lower_base", {-0.02f, 0.0f, 0.002f});
         SAnchor& sEndEffectorAnchor =
            m_pcEmbodiedEntity->AddAnchor("end_effector", {0.0980875, 0, 0.055});
         if(m_bDebug) {
            /* create additional anchors for drawing the links from the dynamics 3D engine */
            m_pcEmbodiedEntity->AddAnchor("left_wheel");
            m_pcEmbodiedEntity->AddAnchor("right_wheel");
            m_pcEmbodiedEntity->AddAnchor("rear_pivot");
            m_pcEmbodiedEntity->AddAnchor("upper_base");
            m_pcEmbodiedEntity->AddAnchor("lift_column");
            m_pcEmbodiedEntity->AddAnchor("end_effector_support");
         }
         /* create and initialize the differential drive entity */
         m_pcDifferentialDriveEntity 
            = new CBuilderBotDifferentialDriveEntity(this, "differential_drive_0");
         AddComponent(*m_pcDifferentialDriveEntity);
         m_pcDifferentialDriveEntity->Enable();
         /* create and initialize the electromagnet system entity */
         m_pcElectromagnetSystemEntity = 
            new CBuilderBotElectromagnetSystemEntity(this, "electromagnet_system_0");
         AddComponent(*m_pcElectromagnetSystemEntity);
         m_pcElectromagnetSystemEntity->Enable();
         /* create and initialize the lift system entity */
         m_pcLiftSystemEntity = 
            new CBuilderBotLiftSystemEntity(this, "lift_system_0", sEndEffectorAnchor);
         AddComponent(*m_pcLiftSystemEntity);
         m_pcLiftSystemEntity->Enable();
         /* create and initialize the tags */
         m_pcTagEquippedEntity = new CTagEquippedEntity(this, "tags_0");
         m_pcTagEquippedEntity->AddTag("tag_0",
                                       TAG_OFFSET_POSITION,
                                       CQuaternion(),
                                       m_pcEmbodiedEntity->GetOriginAnchor(),
                                       CRadians::PI_OVER_THREE,
                                       TAG_SIDE_LENGTH,
                                       GetId());
         std::string strTagMedium("tags");
         GetNodeAttributeOrDefault(t_tree, "tag_medium", strTagMedium, strTagMedium);
         CTagMedium& cTagMedium = 
            CSimulator::GetInstance().GetMedium<CTagMedium>(strTagMedium);
         m_pcTagEquippedEntity->SetMedium(cTagMedium);
         m_pcTagEquippedEntity->Enable();
         AddComponent(*m_pcTagEquippedEntity);
         /* create and initialize a radio equipped entity */
         std::string strWifiMedium("wifi");
         GetNodeAttributeOrDefault(t_tree, "wifi_medium", strWifiMedium, strWifiMedium);
         CSimpleRadioMedium& cWifiMedium =
            CSimulator::GetInstance().GetMedium<CSimpleRadioMedium>(strWifiMedium);
         std::string strNFCMedium("nfc");
         GetNodeAttributeOrDefault(t_tree, "nfc_medium", strNFCMedium, strNFCMedium);
         CSimpleRadioMedium& cNFCMedium =
            CSimulator::GetInstance().GetMedium<CSimpleRadioMedium>(strNFCMedium);
         m_pcSimpleRadioEquippedEntity = new CSimpleRadioEquippedEntity(this, "simple_radios_0");
         m_pcSimpleRadioEquippedEntity->AddRadio("wifi",
                                                 WIFI_ANCHOR_OFFSET,
                                                 sLowerBaseAnchor,
                                                 cWifiMedium,
                                                 WIFI_TRANSMISSION_RANGE);
         m_pcSimpleRadioEquippedEntity->AddRadio("nfc",
                                                 NFC_ANCHOR_OFFSET,
                                                 sEndEffectorAnchor,
                                                 cNFCMedium,
                                                 NFC_TRANSMISSION_RANGE);
         m_pcSimpleRadioEquippedEntity->Enable();
         AddComponent(*m_pcSimpleRadioEquippedEntity);
         /* create and initialize the directional LED equipped entity */
         m_pcDirectionalLEDEquippedEntity = new CDirectionalLEDEquippedEntity(this);
         AddComponent(*m_pcDirectionalLEDEquippedEntity);
         for(UInt32 i = 0; i<12; i++) {
            m_pcDirectionalLEDEquippedEntity->AddLED("led_" + std::to_string(i),
                                                   CVector3(0.0, 0.0, 0.0),
                                                   CQuaternion(),
                                                   sLowerBaseAnchor,
                                                   CRadians::PI_OVER_THREE,
                                                   CColor::BLACK);
         }
         /* Create and initialize the controllable entity */
         m_pcControllableEntity = new CControllableEntity(this);
         AddComponent(*m_pcControllableEntity);
         m_pcControllableEntity->Init(GetNode(t_tree, "controller"));
         /* Update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize BuilderBot", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotEntity::UpdateComponents() {
      /* update all components using base class method */
      CComposableEntity::UpdateComponents();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CBuilderBotEntity,
                   "builderbot",
                   "1.0",
                   "Michael Allwright [mallwright@learnrobotics.io]",
                   "The BuilderBot construction robot",
                   "<TODO>",
                   "Usable"
   );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CBuilderBotEntity);

   /****************************************/
   /****************************************/

}
