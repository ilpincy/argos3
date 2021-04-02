/**
 * @file <argos3/plugins/robots/e-puck/simulator/epuck_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "epuck_entity.h"

#include <argos3/core/utility/math/matrix/rotationmatrix3.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>
#include <argos3/plugins/simulator/entities/ground_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/light_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/proximity_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/battery_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real BODY_RADIUS                = 0.035f;
   static const Real BODY_HEIGHT                = 0.086f;

   static const Real INTERWHEEL_DISTANCE        = 0.053f;
   static const Real HALF_INTERWHEEL_DISTANCE   = INTERWHEEL_DISTANCE * 0.5f;
   static const Real WHEEL_RADIUS               = 0.0205f;

   static const Real PROXIMITY_SENSOR_RING_ELEVATION       = 0.06f;
   static const Real PROXIMITY_SENSOR_RING_RADIUS          = BODY_RADIUS;
   static const CRadians PROXIMITY_SENSOR_RING_START_ANGLE = CRadians((2 * ARGOS_PI / 8.0f) * 0.5f);
   static const Real PROXIMITY_SENSOR_RING_RANGE           = 0.1f;

   static const CRadians LED_RING_START_ANGLE   = CRadians((ARGOS_PI / 8.0f) * 0.5f);
   static const Real LED_RING_RADIUS            = BODY_RADIUS + 0.007;
   static const Real LED_RING_ELEVATION         = 0.086f;

  const Real CEPuckEntity::RAB_DEFAULT_RANGE = 0.8;
  const size_t CEPuckEntity::RAB_DEFAULT_MSG_SIZE = 2;
  const Real CEPuckEntity::RAB_DEFAULT_ELEVATION = LED_RING_ELEVATION;
  const CQuaternion CEPuckEntity::RAB_DEFAULT_ROT_OFFSET = CQuaternion();

   /****************************************/
   /****************************************/

   CEPuckEntity::CEPuckEntity() :
      CComposableEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcGroundSensorEquippedEntity(nullptr),
      m_pcLEDEquippedEntity(nullptr),
      m_pcLightSensorEquippedEntity(nullptr),
      m_pcProximitySensorEquippedEntity(nullptr),
      m_pcRABEquippedEntity(nullptr),
      m_pcWheeledEntity(nullptr),
      m_pcBatteryEquippedEntity(nullptr) {
   }

   /****************************************/
   /****************************************/

   CEPuckEntity::CEPuckEntity(const std::string& str_id,
                              const std::string& str_controller_id,
                              const CVector3& c_position,
                              const CQuaternion& c_orientation,
                              Real f_rab_range,
                              size_t un_rab_data_size,
                              const std::string& str_bat_model) :
      CComposableEntity(nullptr, str_id),
      m_pcControllableEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcGroundSensorEquippedEntity(nullptr),
      m_pcLEDEquippedEntity(nullptr),
      m_pcLightSensorEquippedEntity(nullptr),
      m_pcProximitySensorEquippedEntity(nullptr),
      m_pcRABEquippedEntity(nullptr),
      m_pcWheeledEntity(nullptr),
      m_pcBatteryEquippedEntity(nullptr) {
      try {
         /*
          * Create and init components
          */
         /* Embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this, "body_0", c_position, c_orientation);
         AddComponent(*m_pcEmbodiedEntity);
         /* Wheeled entity and wheel positions (left, right) */
         m_pcWheeledEntity = new CWheeledEntity(this, "wheels_0", 2);
         AddComponent(*m_pcWheeledEntity);
         m_pcWheeledEntity->SetWheel(0, CVector3(0.0f,  HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
         m_pcWheeledEntity->SetWheel(1, CVector3(0.0f, -HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
         /* LED equipped entity */
         m_pcLEDEquippedEntity = new CLEDEquippedEntity(this, "leds_0");
         AddComponent(*m_pcLEDEquippedEntity);
         m_pcLEDEquippedEntity->AddLEDRing(
            CVector3(0.0f, 0.0f, LED_RING_ELEVATION),
            LED_RING_RADIUS,
            LED_RING_START_ANGLE,
            8,
            m_pcEmbodiedEntity->GetOriginAnchor());
         /* Proximity sensor equipped entity */
         m_pcProximitySensorEquippedEntity =
            new CProximitySensorEquippedEntity(this,
                                               "proximity_0");
         AddComponent(*m_pcProximitySensorEquippedEntity);


         /*m_pcProximitySensorEquippedEntity->AddSensorRing(
            CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
            PROXIMITY_SENSOR_RING_RADIUS,
            PROXIMITY_SENSOR_RING_START_ANGLE,
            PROXIMITY_SENSOR_RING_RANGE,
            8,
            m_pcEmbodiedEntity->GetOriginAnchor());*/
         CRadians sensor_angle[8];
         sensor_angle[0] = CRadians::PI / 10.5884f;
         sensor_angle[1] = CRadians::PI / 3.5999f;
         sensor_angle[2] = CRadians::PI_OVER_TWO; //side sensor
         sensor_angle[3] = CRadians::PI / 1.2f;   // back sensor
         sensor_angle[4] = CRadians::PI / 0.8571f; // back sensor
         sensor_angle[5] = CRadians::PI / 0.6667f; //side sensor
         sensor_angle[6] = CRadians::PI / 0.5806f;
         sensor_angle[7] = CRadians::PI / 0.5247f;

         CRadians cAngle;
         CVector3 cOff, cDir, c_center = CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION);
         for(UInt32 i = 0; i < 8; ++i)
         {
            cAngle = sensor_angle[i];
            cAngle.SignedNormalize();
            cOff.Set(PROXIMITY_SENSOR_RING_RADIUS, 0.0f, 0.0f);
            cOff.RotateZ(cAngle);
            cOff += c_center;
            cDir.Set(PROXIMITY_SENSOR_RING_RANGE, 0.0f, 0.0f);
            cDir.RotateZ(cAngle);
            m_pcProximitySensorEquippedEntity->AddSensor(cOff, cDir, PROXIMITY_SENSOR_RING_RANGE, m_pcEmbodiedEntity->GetOriginAnchor());
         }


         /* Light sensor equipped entity */
         m_pcLightSensorEquippedEntity =
            new CLightSensorEquippedEntity(this,
                                           "light_0");
         AddComponent(*m_pcLightSensorEquippedEntity);
         m_pcLightSensorEquippedEntity->AddSensorRing(
            CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
            PROXIMITY_SENSOR_RING_RADIUS,
            PROXIMITY_SENSOR_RING_START_ANGLE,
            PROXIMITY_SENSOR_RING_RANGE,
            8,
            m_pcEmbodiedEntity->GetOriginAnchor());
         /* Ground sensor equipped entity */
         m_pcGroundSensorEquippedEntity =
            new CGroundSensorEquippedEntity(this,
                                            "ground_0");
         AddComponent(*m_pcGroundSensorEquippedEntity);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f, -0.009f),
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE,
                                                   m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f,  0.0f),
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE,
                                                   m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f,  0.009f),
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE,
                                                   m_pcEmbodiedEntity->GetOriginAnchor());
         /* RAB equipped entity */
         m_pcRABEquippedEntity = new CRABEquippedEntity(this,
                                                        "rab_0",
                                                        un_rab_data_size,
                                                        f_rab_range,
                                                        m_pcEmbodiedEntity->GetOriginAnchor(),
                                                        *m_pcEmbodiedEntity,
                                                        CVector3(0.0f, 0.0f, RAB_DEFAULT_ELEVATION),
                                                        RAB_DEFAULT_ROT_OFFSET);
         AddComponent(*m_pcRABEquippedEntity);
         /* Battery equipped entity */
         m_pcBatteryEquippedEntity = new CBatteryEquippedEntity(this, "battery_0", str_bat_model);
         AddComponent(*m_pcBatteryEquippedEntity);
         /* Controllable entity
            It must be the last one, for actuators/sensors to link to composing entities correctly */
         m_pcControllableEntity = new CControllableEntity(this, "controller_0");
         AddComponent(*m_pcControllableEntity);
         m_pcControllableEntity->SetController(str_controller_id);
         /* Update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckEntity::Init(TConfigurationNode& t_tree) {
      try {
         /*
          * Init parent
          */
         CComposableEntity::Init(t_tree);
         /*
          * Create and init components
          */
         /* Embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         /* Wheeled entity and wheel positions (left, right) */
         m_pcWheeledEntity = new CWheeledEntity(this, "wheels_0", 2);
         AddComponent(*m_pcWheeledEntity);
         m_pcWheeledEntity->SetWheel(0, CVector3(0.0f,  HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
         m_pcWheeledEntity->SetWheel(1, CVector3(0.0f, -HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
         /* LED equipped entity, with LEDs [0-11] and beacon [12] */
         m_pcLEDEquippedEntity = new CLEDEquippedEntity(this, "leds_0");
         AddComponent(*m_pcLEDEquippedEntity);
         m_pcLEDEquippedEntity->AddLEDRing(
            CVector3(0.0f, 0.0f, LED_RING_ELEVATION),
            LED_RING_RADIUS,
            LED_RING_START_ANGLE,
            8,
            m_pcEmbodiedEntity->GetOriginAnchor());

         /* Proximity sensor equipped entity */
         m_pcProximitySensorEquippedEntity =
            new CProximitySensorEquippedEntity(this,
                                               "proximity_0");
         AddComponent(*m_pcProximitySensorEquippedEntity);
         /*m_pcProximitySensorEquippedEntity->AddSensorRing(
            CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
            PROXIMITY_SENSOR_RING_RADIUS,
            PROXIMITY_SENSOR_RING_START_ANGLE,
            PROXIMITY_SENSOR_RING_RANGE,
            8,
            m_pcEmbodiedEntity->GetOriginAnchor());*/

	 CRadians sensor_angle[8];
         sensor_angle[0] = CRadians::PI / 10.5884f;
         sensor_angle[1] = CRadians::PI / 3.5999f;
         sensor_angle[2] = CRadians::PI_OVER_TWO; //side sensor
         sensor_angle[3] = CRadians::PI / 1.2f;   // back sensor
         sensor_angle[4] = CRadians::PI / 0.8571f; // back sensor
         sensor_angle[5] = CRadians::PI / 0.6667f; //side sensor
         sensor_angle[6] = CRadians::PI / 0.5806f;
         sensor_angle[7] = CRadians::PI / 0.5247f;

         CRadians cAngle;
         CVector3 cOff, cDir, c_center = CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION);
         for(UInt32 i = 0; i < 8; ++i)
         {
            cAngle = sensor_angle[i];
            cAngle.SignedNormalize();
            cOff.Set(PROXIMITY_SENSOR_RING_RADIUS, 0.0f, 0.0f);
            cOff.RotateZ(cAngle);
            cOff += c_center;
            cDir.Set(PROXIMITY_SENSOR_RING_RANGE, 0.0f, 0.0f);
            cDir.RotateZ(cAngle);
            m_pcProximitySensorEquippedEntity->AddSensor(cOff, cDir, PROXIMITY_SENSOR_RING_RANGE, m_pcEmbodiedEntity->GetOriginAnchor());
         }



         /* Light sensor equipped entity */
         m_pcLightSensorEquippedEntity =
            new CLightSensorEquippedEntity(this,
                                           "light_0");
         AddComponent(*m_pcLightSensorEquippedEntity);
         m_pcLightSensorEquippedEntity->AddSensorRing(
            CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
            PROXIMITY_SENSOR_RING_RADIUS,
            PROXIMITY_SENSOR_RING_START_ANGLE,
            PROXIMITY_SENSOR_RING_RANGE,
            8,
            m_pcEmbodiedEntity->GetOriginAnchor());
         /* Ground sensor equipped entity */
         m_pcGroundSensorEquippedEntity =
            new CGroundSensorEquippedEntity(this,
                                            "ground_0");
         AddComponent(*m_pcGroundSensorEquippedEntity);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f, -0.009f),
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE,
                                                   m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f,  0.0f),
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE,
                                                   m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.03f,  0.009f),
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE,
                                                   m_pcEmbodiedEntity->GetOriginAnchor());
         /* RAB equipped entity */
         m_pcRABEquippedEntity = new CRABEquippedEntity(this,
                                                        "rab_0",
                                                        RAB_DEFAULT_MSG_SIZE,
                                                        RAB_DEFAULT_RANGE,
                                                        m_pcEmbodiedEntity->GetOriginAnchor(),
                                                        *m_pcEmbodiedEntity,
                                                        CVector3(0.0f, 0.0f, RAB_DEFAULT_ELEVATION),
                                                        RAB_DEFAULT_ROT_OFFSET);
         if(NodeExists(t_tree, "range_and_bearing")) {
           m_pcRABEquippedEntity->Init(GetNode(t_tree, "range_and_bearing"));
         }
         AddComponent(*m_pcRABEquippedEntity);
         /* Battery equipped entity */
         m_pcBatteryEquippedEntity = new CBatteryEquippedEntity(this, "battery_0");
         if(NodeExists(t_tree, "battery"))
            m_pcBatteryEquippedEntity->Init(GetNode(t_tree, "battery"));
         AddComponent(*m_pcBatteryEquippedEntity);
         /* Controllable entity
            It must be the last one, for actuators/sensors to link to composing entities correctly */
         m_pcControllableEntity = new CControllableEntity(this);
         AddComponent(*m_pcControllableEntity);
         m_pcControllableEntity->Init(GetNode(t_tree, "controller"));
         /* Update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckEntity::Reset() {
      /* Reset all components */
      CComposableEntity::Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CEPuckEntity::Destroy() {
      CComposableEntity::Destroy();
   }

   /****************************************/
   /****************************************/

#define UPDATE(COMPONENT) if(COMPONENT->IsEnabled()) COMPONENT->Update();

   void CEPuckEntity::UpdateComponents() {
      UPDATE(m_pcRABEquippedEntity);
      UPDATE(m_pcLEDEquippedEntity);
      UPDATE(m_pcBatteryEquippedEntity);
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CEPuckEntity,
                   "e-puck",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The e-puck robot.",
                   "The e-puck is a open-hardware, extensible robot intended for education. In its\n"
                   "simplest form, it is a two-wheeled robot equipped with proximity sensors,\n"
                   "ground sensors, light sensors, a microphone, a frontal camera, and a ring of\n"
                   "red LEDs. More information is available at http://www.epuck.org\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <e-puck id=\"eb0\">\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,90,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "    </e-puck>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'body/position' attribute specifies the position of the pucktom point of the\n"
                   "e-puck in the arena. When the robot is untranslated and unrotated, the\n"
                   "pucktom point is in the origin and it is defined as the middle point between\n"
                   "the two wheels on the XY plane and the lowest point of the robot on the Z\n"
                   "axis, that is the point where the wheels touch the floor. The attribute values\n"
                   "are in the X,Y,Z order.\n"
                   "The 'body/orientation' attribute specifies the orientation of the e-puck. All\n"
                   "rotations are performed with respect to the pucktom point. The order of the\n"
                   "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                   "around the Z axis, the second around Y and the last around X. This reflects\n"
                   "the internal convention used in ARGoS, in which rotations are performed in\n"
                   "that order. Angles are expressed in degrees. When the robot is unrotated, it\n"
                   "is oriented along the X axis.\n"
                   "The 'controller/config' attribute is used to assign a controller to the\n"
                   "e-puck. The value of the attribute must be set to the id of a previously\n"
                   "defined controller. Controllers are defined in the <controllers> XML subtree.\n\n"

                   "OPTIONAL XML CONFIGURATION\n\n"

                   "----------------------------------------\n"
                   "Range And Bearing (RAB) Configuration\n"
                   "----------------------------------------\n" +

                   CRABEquippedEntity::GetQueryDocumentation({
                       .strEntityName = "e-puck",
                       .fRangeDefault = CEPuckEntity::RAB_DEFAULT_RANGE,
                       .fMsgSizeDefault = CEPuckEntity::RAB_DEFAULT_MSG_SIZE,
                       .cPosOffsetDefault = CVector3(0.0f, 0.0f, CEPuckEntity::RAB_DEFAULT_ELEVATION),
                       .cRotOffsetDefault = CEPuckEntity::RAB_DEFAULT_ROT_OFFSET}) +


                   "----------------------------------------\n"
                   "Battery Configuration\n"
                   "----------------------------------------\n" +

                   CBatteryEquippedEntity::GetQueryDocumentation({
                       .strEntityName = "e-puck"}),
                   "Under development"
      );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CEPuckEntity);

   /****************************************/
   /****************************************/

}
