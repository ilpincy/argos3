/**
 * @file <argos3/plugins/robots/drone/simulator/drone_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_entity.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/datatypes/color.h>

#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/simple_radio_equipped_entity.h>
#include <argos3/plugins/simulator/media/directional_led_medium.h>
#include <argos3/plugins/simulator/media/simple_radio_medium.h>

#include <argos3/plugins/robots/drone/simulator/drone_flight_system_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   const Real CDroneEntity::WIFI_TRANSMISSION_RANGE = 10.0;

   /****************************************/
   /****************************************/

   CDroneEntity::CDroneEntity() :
      CComposableEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcDirectionalLEDEquippedEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcFlightSystemEntity(nullptr),
      m_pcSimpleRadioEquippedEntity(nullptr) {}

   /****************************************/
   /****************************************/

   void CDroneEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* initialize the base class */
         CComposableEntity::Init(t_tree);
         /* create and initialize the embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         SAnchor& sOriginAnchor = m_pcEmbodiedEntity->GetOriginAnchor();
         /* create and initialize the flight system entity */
         m_pcFlightSystemEntity = 
            new CDroneFlightSystemEntity(this, "flight_system_0");
         m_pcFlightSystemEntity->Enable();
         AddComponent(*m_pcFlightSystemEntity);
         /* create and initialize a radio equipped entity for WiFi */
         m_pcSimpleRadioEquippedEntity = new CSimpleRadioEquippedEntity(this, "simple_radios_0");
         std::string strWifiMedium;
         GetNodeAttributeOrDefault(t_tree, "wifi_medium", strWifiMedium, strWifiMedium);
         if(!strWifiMedium.empty()) {
            CSimpleRadioMedium& cWifiMedium = CSimulator::GetInstance().GetMedium<CSimpleRadioMedium>(strWifiMedium);
            m_pcSimpleRadioEquippedEntity->AddRadio("wifi",
                                                    CVector3(0.0f, 0.0f, 0.1f),
                                                    sOriginAnchor,
                                                    cWifiMedium,
                                                    WIFI_TRANSMISSION_RANGE);
            m_pcSimpleRadioEquippedEntity->Enable();
         }
         AddComponent(*m_pcSimpleRadioEquippedEntity);
         /* create and initialize the directional LED equipped entity */
         m_pcDirectionalLEDEquippedEntity = new CDirectionalLEDEquippedEntity(this, "leds_0");
         m_pcDirectionalLEDEquippedEntity->AddLED("led_0",
                                                  CVector3(0.1, 0, 0.1),
                                                  CQuaternion(),
                                                  sOriginAnchor,
                                                  CRadians::PI_OVER_THREE,
                                                  CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_1",
                                                  CVector3(0, 0.1, 0.1),
                                                  CQuaternion(), 
                                                  sOriginAnchor,
                                                  CRadians::PI_OVER_THREE,
                                                  CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_2",
                                                  CVector3(-0.1, 0, 0.1),
                                                  CQuaternion(),
                                                  sOriginAnchor,
                                                  CRadians::PI_OVER_THREE,
                                                  CColor::BLACK);
         m_pcDirectionalLEDEquippedEntity->AddLED("led_3",
                                                  CVector3(0, -0.1, 0.1),
                                                  CQuaternion(),
                                                  sOriginAnchor,
                                                  CRadians::PI_OVER_THREE,
                                                  CColor::BLACK);
         std::string strLedMedium;
         GetNodeAttributeOrDefault(t_tree, "led_medium", strLedMedium, strLedMedium);
         if(!strLedMedium.empty()) {
            CDirectionalLEDMedium& cLedMedium =
               CSimulator::GetInstance().GetMedium<CDirectionalLEDMedium>(strLedMedium);
            m_pcDirectionalLEDEquippedEntity->SetMedium(cLedMedium);
            m_pcDirectionalLEDEquippedEntity->Enable();
         }
         AddComponent(*m_pcDirectionalLEDEquippedEntity);
         /* Create and initialize the controllable entity */
         m_pcControllableEntity = new CControllableEntity(this);
         AddComponent(*m_pcControllableEntity);
         m_pcControllableEntity->Init(GetNode(t_tree, "controller"));
         /* Update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize drone", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDroneEntity::UpdateComponents() {
      /* update all components using base class method */
      CComposableEntity::UpdateComponents();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CDroneEntity,
                   "drone",
                   "1.0",
                   "Michael Allwright [allsey87@gmail.com]",
                   "A drone",
                   "Long description",
                   "Usable"
   );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CDroneEntity);

   /****************************************/
   /****************************************/

}
