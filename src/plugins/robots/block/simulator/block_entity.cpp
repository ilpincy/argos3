/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "block_entity.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/simple_radio_equipped_entity.h>
#include <argos3/plugins/simulator/entities/tag_equipped_entity.h>
#include <argos3/plugins/simulator/media/directional_led_medium.h>
#include <argos3/plugins/simulator/media/simple_radio_medium.h>
#include <argos3/plugins/simulator/media/tag_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   const Real CBlockEntity::TAG_SIDE_LENGTH = 0.0235;

   const Real CBlockEntity::NFC_TRANSMISSION_RANGE = 0.020;

   const Real CBlockEntity::BLOCK_SIDE_LENGTH = 0.055;

   /* these face descriptors are used for calculating the positions of the LEDs radios, and tags.
      Since these entities need to be detected by sensors, the offset is increased by 1% so that
      the entity sits just above the surface of the block */
   const std::array<std::tuple<std::string, CVector3, CQuaternion>, 6> CBlockEntity::FACE_DESCRIPTORS = {
      std::make_tuple("north",
                      CVector3( 0.505,  0.000,  0.505) * BLOCK_SIDE_LENGTH,
                      CQuaternion( 0.5 * CRadians::PI, CVector3::Y)),
      std::make_tuple("east",
                      CVector3( 0.000, -0.505,  0.505) * BLOCK_SIDE_LENGTH,
                      CQuaternion( 0.5 * CRadians::PI, CVector3::X)),
      std::make_tuple("south",
                      CVector3(-0.505,  0.000,  0.505) * BLOCK_SIDE_LENGTH,
                      CQuaternion(-0.5 * CRadians::PI, CVector3::Y)),
      std::make_tuple("west",
                      CVector3( 0.000,  0.505,  0.505) * BLOCK_SIDE_LENGTH,
                      CQuaternion(-0.5 * CRadians::PI, CVector3::X)),
      std::make_tuple("top",
                      CVector3( 0.000,  0.000,  1.010) * BLOCK_SIDE_LENGTH,
                      CQuaternion( 0.0 * CRadians::PI, CVector3::X)),
      std::make_tuple("bottom",
                      CVector3( 0.000,  0.000, -0.010) * BLOCK_SIDE_LENGTH,
                      CQuaternion( 1.0 * CRadians::PI, CVector3::X)),
   };

   const std::array<CVector3, 4> CBlockEntity::LED_DESCRIPTORS = {
      CVector3( 0.02,  0.00, 0.0),
      CVector3( 0.00,  0.02, 0.0),
      CVector3(-0.02,  0.00, 0.0),
      CVector3( 0.00, -0.02, 0.0),
   };

   /****************************************/
   /****************************************/

   CBlockEntity::CBlockEntity() :
      CComposableEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcDirectionalLEDEquippedEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcTagEquippedEntity(nullptr),
      m_pcSimpleRadioEquippedEntity(nullptr) {}


   /****************************************/
   /****************************************/

   void CBlockEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Initialize the parent */
         CComposableEntity::Init(t_tree);
         /* create and initialize the embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this, "body_0");
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         /* check if this block is movable */
         bool bMovable = true;
         GetNodeAttributeOrDefault(t_tree, "movable", bMovable, bMovable);
         m_pcEmbodiedEntity->SetMovable(bMovable);
         /* get a reference to the origin anchor */
         SAnchor& sOriginAnchor = m_pcEmbodiedEntity->GetOriginAnchor();
         /* create and initialize the tags */
         m_pcTagEquippedEntity = new CTagEquippedEntity(this, "tags_0");
         for(const std::tuple<std::string, CVector3, CQuaternion>& c_face : FACE_DESCRIPTORS) {
            /* add a tag */
            m_pcTagEquippedEntity->AddTag(std::get<std::string>(c_face),
                                          std::get<CVector3>(c_face),
                                          std::get<CQuaternion>(c_face),
                                          sOriginAnchor,
                                          CRadians::PI_OVER_THREE,
                                          TAG_SIDE_LENGTH,
                                          std::string("0"));
         }
         CTagMedium& cTagMedium = 
            CSimulator::GetInstance().GetMedium<CTagMedium>("tags");
         m_pcTagEquippedEntity->SetMedium(cTagMedium);
         m_pcTagEquippedEntity->Enable();
         AddComponent(*m_pcTagEquippedEntity);
         /* create and initialize the radios */
         CSimpleRadioMedium& cRadioMedium = 
            CSimulator::GetInstance().GetMedium<CSimpleRadioMedium>("nfc");
         m_pcSimpleRadioEquippedEntity = new CSimpleRadioEquippedEntity(this, "simple_radios_0");
         for(const std::tuple<std::string, CVector3, CQuaternion>& c_face : FACE_DESCRIPTORS) {
            /* add a radio */
            m_pcSimpleRadioEquippedEntity->AddRadio(std::get<std::string>(c_face),
                                                    std::get<CVector3>(c_face),
                                                    sOriginAnchor,
                                                    cRadioMedium,
                                                    NFC_TRANSMISSION_RANGE);
         }
         m_pcSimpleRadioEquippedEntity->Enable();
         AddComponent(*m_pcSimpleRadioEquippedEntity);
         /* create and initialize the directional LEDs */
         CColor cInitLedColor = CColor::BLACK;
         GetNodeAttributeOrDefault(t_tree, "init_led_color", cInitLedColor, cInitLedColor);
         m_pcDirectionalLEDEquippedEntity = new CDirectionalLEDEquippedEntity(this, "directional_leds_0");
         for(const std::tuple<std::string, CVector3, CQuaternion>& c_face : FACE_DESCRIPTORS) {
            for(UInt32 un_index = 0; un_index < LED_DESCRIPTORS.size(); un_index++) {
               /* calculate the LED offset from the origin anchor */
               CVector3 cPosition(LED_DESCRIPTORS[un_index]);
               cPosition.Rotate(std::get<CQuaternion>(c_face));
               cPosition += std::get<CVector3>(c_face);
               /* create an id for the LED */
               std::string strId(std::get<std::string>(c_face));
               strId += ("_" + std::to_string(un_index));
               /* add the LED */
               m_pcDirectionalLEDEquippedEntity->AddLED(strId,
                                                        cPosition,
                                                        std::get<CQuaternion>(c_face),
                                                        sOriginAnchor,
                                                        CRadians::PI_OVER_THREE,
                                                        cInitLedColor);
            }
         }
         CDirectionalLEDMedium& cDirectionalLEDMedium = 
            CSimulator::GetInstance().GetMedium<CDirectionalLEDMedium>("directional_leds");
         m_pcDirectionalLEDEquippedEntity->SetMedium(cDirectionalLEDMedium);
         m_pcDirectionalLEDEquippedEntity->Enable();
         AddComponent(*m_pcDirectionalLEDEquippedEntity);
         /* create and initialize the controllable entity */
         m_pcControllableEntity = new CControllableEntity(this);
         AddComponent(*m_pcControllableEntity);
         m_pcControllableEntity->Init(GetNode(t_tree, "controller"));
         /* update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize Block", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBlockEntity::UpdateComponents() {
      /* update all components using base class method */
      CComposableEntity::UpdateComponents();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CBlockEntity,
                   "block",
                   "1.0",
                   "Michael Allwright [allsey87@gmail.com]",
                   "Model of the stigmergic block",
                   "Long description",
                   "Usable"
   );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CBlockEntity);

   /****************************************/
   /****************************************/

}
