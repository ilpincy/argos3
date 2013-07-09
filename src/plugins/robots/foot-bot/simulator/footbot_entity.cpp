/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "footbot_entity.h"

#include <argos3/core/utility/math/matrix/rotationmatrix3.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>
#include <argos3/plugins/simulator/entities/gripper_equipped_entity.h>
#include <argos3/plugins/simulator/entities/ground_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/light_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/proximity_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/wifi_equipped_entity.h>
#include "footbot_distance_scanner_equipped_entity.h"
#include "footbot_turret_entity.h"

namespace argos {

   /****************************************/
   /****************************************/

   static const Real BODY_RADIUS                = 0.085036758f;
   static const Real BODY_HEIGHT                = 0.146899733f;

   static const Real LED_RING_RADIUS            = BODY_RADIUS + 0.005;

   static const Real INTERWHEEL_DISTANCE        = 0.14f;
   static const Real HALF_INTERWHEEL_DISTANCE   = INTERWHEEL_DISTANCE * 0.5f;
   static const Real WHEEL_RADIUS               = 0.029112741f;

   static const Real PROXIMITY_SENSOR_RING_ELEVATION       = 0.06f;
   static const Real PROXIMITY_SENSOR_RING_RADIUS          = BODY_RADIUS;
   static const CRadians PROXIMITY_SENSOR_RING_START_ANGLE = CRadians((ARGOS_PI / 12.0f) * 0.5f);
   static const Real PROXIMITY_SENSOR_RING_RANGE           = 0.1f;

   static const Real LED_RING_ELEVATION         = 0.085f;
   static const Real RAB_ELEVATION              = 0.1f;
   static const Real BEACON_ELEVATION           = 0.174249733f;

   static const Real GRIPPER_ELEVATION          = LED_RING_ELEVATION;

   static const CRadians LED_ANGLE_SLICE        = CRadians(ARGOS_PI / 6.0);
   static const CRadians HALF_LED_ANGLE_SLICE   = LED_ANGLE_SLICE * 0.5f;

   /****************************************/
   /****************************************/

   CFootBotEntity::CFootBotEntity() :
      CComposableEntity(NULL),
      m_pcControllableEntity(NULL),
      m_pcDistanceScannerEquippedEntity(NULL),
      m_pcTurretEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcGripperEquippedEntity(NULL),
      m_pcGroundSensorEquippedEntity(NULL),
      m_pcLEDEquippedEntity(NULL),
      m_pcLightSensorEquippedEntity(NULL),
      m_pcProximitySensorEquippedEntity(NULL),
      m_pcRABEquippedEntity(NULL),
      m_pcWheeledEntity(NULL),
      m_pcWiFiEquippedEntity(NULL) {
   }

   /****************************************/
   /****************************************/

   void CFootBotEntity::Init(TConfigurationNode& t_tree) {
      try {
         /*
          * Init parent
          */
         CComposableEntity::Init(t_tree);
         /*
          * Create and init components
          */
         /* Wheeled entity and wheel positions (left, right) */
         m_pcWheeledEntity = new CWheeledEntity(this, 2);
         AddComponent(*m_pcWheeledEntity);
         m_pcWheeledEntity->SetWheel(0, CVector3(0.0f,  HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
         m_pcWheeledEntity->SetWheel(1, CVector3(0.0f, -HALF_INTERWHEEL_DISTANCE, 0.0f), WHEEL_RADIUS);
         m_pcWheeledEntity->Init(t_tree);
         /* LED equipped entity, with LEDs [0-11] and beacon [12] */
         m_pcLEDEquippedEntity = new CLEDEquippedEntity(this,
                                                        "leds",
                                                        m_pcEmbodiedEntity);
         AddComponent(*m_pcLEDEquippedEntity);
         for(UInt32 i = 0; i < 13; ++i) {
            m_pcLEDEquippedEntity->AddLED(CVector3());
         }
         /* Proximity sensor equipped entity */
         m_pcProximitySensorEquippedEntity =
            new CProximitySensorEquippedEntity(this,
                                               "proximity");
         AddComponent(*m_pcProximitySensorEquippedEntity);
         m_pcProximitySensorEquippedEntity->AddSensorRing(
            CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
            PROXIMITY_SENSOR_RING_RADIUS,
            PROXIMITY_SENSOR_RING_START_ANGLE,
            PROXIMITY_SENSOR_RING_RANGE,
            24);
         /* Light sensor equipped entity */
         m_pcLightSensorEquippedEntity =
            new CLightSensorEquippedEntity(this,
                                           "light");
         AddComponent(*m_pcLightSensorEquippedEntity);
         m_pcLightSensorEquippedEntity->AddSensorRing(
            CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
            PROXIMITY_SENSOR_RING_RADIUS,
            PROXIMITY_SENSOR_RING_START_ANGLE,
            PROXIMITY_SENSOR_RING_RANGE,
            24);
         /* Gripper equipped entity */
         m_pcGripperEquippedEntity =
            new CGripperEquippedEntity(this,
                                       "gripper",
                                       CVector3(BODY_RADIUS, 0.0f, GRIPPER_ELEVATION),
                                       CVector3::X);
         AddComponent(*m_pcGripperEquippedEntity);
         /* Ground sensor equipped entity */
         m_pcGroundSensorEquippedEntity =
            new CGroundSensorEquippedEntity(this,
                                            "ground");
         AddComponent(*m_pcGroundSensorEquippedEntity);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.063, 0.0116),
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.063, -0.0116),
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(-0.063, -0.0116),
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(-0.063, 0.0116),
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.08, 0.0),
                                                   CGroundSensorEquippedEntity::TYPE_BLACK_WHITE);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.042, 0.065),
                                                   CGroundSensorEquippedEntity::TYPE_BLACK_WHITE);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.0, 0.08),
                                                   CGroundSensorEquippedEntity::TYPE_BLACK_WHITE);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(-0.042, 0.065),
                                                   CGroundSensorEquippedEntity::TYPE_BLACK_WHITE);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(-0.08, 0.0),
                                                   CGroundSensorEquippedEntity::TYPE_BLACK_WHITE);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(-0.042, -0.065),
                                                   CGroundSensorEquippedEntity::TYPE_BLACK_WHITE);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.0, -0.08),
                                                   CGroundSensorEquippedEntity::TYPE_BLACK_WHITE);
         m_pcGroundSensorEquippedEntity->AddSensor(CVector2(0.042, -0.065),
                                                   CGroundSensorEquippedEntity::TYPE_BLACK_WHITE);
         /* Distance scanner */
         m_pcDistanceScannerEquippedEntity = new CFootBotDistanceScannerEquippedEntity(this,
                                                                                       "distance_scanner");
         AddComponent(*m_pcDistanceScannerEquippedEntity);
         /* Embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         /* RAB equipped entity */
         Real fRange = 3.0f;
         GetNodeAttributeOrDefault(t_tree, "rab_range", fRange, fRange);
         m_pcRABEquippedEntity = new CRABEquippedEntity(this,
                                                        "rab",
                                                        10,
                                                        fRange,
                                                        *m_pcEmbodiedEntity,
                                                        CVector3(0.0f, 0.0f, RAB_ELEVATION));
         AddComponent(*m_pcRABEquippedEntity);
         /* Turret equipped entity */
         m_pcTurretEntity = new CFootBotTurretEntity(this, "turret");
         AddComponent(*m_pcTurretEntity);
         /* WiFi equipped entity */
         m_pcWiFiEquippedEntity = new CWiFiEquippedEntity(this, "wifi");
         AddComponent(*m_pcWiFiEquippedEntity);
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

   void CFootBotEntity::Reset() {
      /* Reset all components */
      CComposableEntity::Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CFootBotEntity::Destroy() {
      CComposableEntity::Destroy();
   }

   /****************************************/
   /****************************************/

#define UPDATE(COMPONENT) if(COMPONENT->IsEnabled()) COMPONENT->Update();

   void CFootBotEntity::UpdateComponents() {
      UPDATE(m_pcDistanceScannerEquippedEntity);
      UPDATE(m_pcTurretEntity);
      UPDATE(m_pcGripperEquippedEntity);
      UPDATE(m_pcRABEquippedEntity);
      if(m_pcLEDEquippedEntity->IsEnabled()) SetLEDPosition();
   }

   /****************************************/
   /****************************************/
   
#define SET_RING_LED_POSITION(IDX)                                      \
   cLEDPosition.Set(LED_RING_RADIUS, 0.0f, LED_RING_ELEVATION);         \
   cLEDAngle = cLEDAnglePhase;                                          \
   cLEDAngle += LED_ANGLE_SLICE * IDX;                                  \
   cLEDPosition.RotateZ(cLEDAngle);                                     \
   cLEDPosition.Rotate(m_pcEmbodiedEntity->GetOrientation());           \
   cLEDPosition += cEntityPosition;                                     \
   m_pcLEDEquippedEntity->SetLEDPosition(IDX, cLEDPosition);
   
   void CFootBotEntity::SetLEDPosition() {
      /* Set LED positions */
      const CVector3& cEntityPosition = GetEmbodiedEntity().GetPosition();
      CVector3 cLEDPosition;
      CRadians cLEDAnglePhase = HALF_LED_ANGLE_SLICE + m_pcTurretEntity->GetRotation();
      CRadians cLEDAngle;
      SET_RING_LED_POSITION(0);
      SET_RING_LED_POSITION(1);
      SET_RING_LED_POSITION(2);
      SET_RING_LED_POSITION(3);
      SET_RING_LED_POSITION(4);
      SET_RING_LED_POSITION(5);
      SET_RING_LED_POSITION(6);
      SET_RING_LED_POSITION(7);
      SET_RING_LED_POSITION(8);
      SET_RING_LED_POSITION(9);
      SET_RING_LED_POSITION(10);
      SET_RING_LED_POSITION(11);
      /* Set beacon position */
      cLEDPosition.Set(0.0f, 0.0f, BEACON_ELEVATION);
      cLEDPosition.Rotate(m_pcEmbodiedEntity->GetOrientation());
      cLEDPosition += cEntityPosition;
      m_pcLEDEquippedEntity->SetLEDPosition(12, cLEDPosition);
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CFootBotEntity,
                   "foot-bot",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The foot-bot robot, developed in the Swarmanoid project.",
                   "The foot-bot is a wheeled robot developed in the Swarmanoid Project. It is a\n"
                   "modular robot with a rich set of sensors and actuators. For more information,\n"
                   "refer to the dedicated web page\n"
                   "(http://www.swarmanoid.org/swarmanoid_hardware.php).\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <foot-bot id=\"fb0\">\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,90,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "    </foot-bot>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'body/position' attribute specifies the position of the bottom point of the\n"
                   "foot-bot in the arena. When the robot is untranslated and unrotated, the\n"
                   "bottom point is in the origin and it is defined as the middle point between\n"
                   "the two wheels on the XY plane and the lowest point of the robot on the Z\n"
                   "axis, that is the point where the wheels touch the floor. The attribute values\n"
                   "are in the X,Y,Z order.\n"
                   "The 'body/orientation' attribute specifies the orientation of the foot-bot. All\n"
                   "rotations are performed with respect to the bottom point. The order of the\n"
                   "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                   "around the Z axis, the second around Y and the last around X. This reflects\n"
                   "the internal convention used in ARGoS, in which rotations are performed in\n"
                   "that order. Angles are expressed in degrees. When the robot is unrotated, it\n"
                   "is oriented along the X axis.\n"
                   "The 'controller/config' attribute is used to assign a controller to the\n"
                   "foot-bot. The value of the attribute must be set to the id of a previously\n"
                   "defined controller. Controllers are defined in the <controllers> XML subtree.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being.\n",
                   "Under development"
      );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CFootBotEntity);

   /****************************************/
   /****************************************/

}
