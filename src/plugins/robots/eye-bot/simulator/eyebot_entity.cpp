/**
 * @file <argos3/plugins/robots/eye-bot/simulator/eyebot_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "eyebot_entity.h"

#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/light_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/proximity_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/quadrotor_entity.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real BODY_HEIGHT              = 0.566f;
   static const Real BODY_RADIUS              = 0.25f;
   static const Real LED_RING_RADIUS          = BODY_RADIUS + 0.005;
   static const Real LED_LOWER_RING_ELEVATION = 0.1535f;
   static const Real LED_UPPER_RING_ELEVATION = 0.1635f;
   static const CRadians LED_ANGLE_SLICE      = CRadians(3.14159265358979323846264338327950288 / 8.0);
   static const CRadians HALF_LED_ANGLE_SLICE = LED_ANGLE_SLICE * 0.5f;
   static const Real PROXIMITY_SENSOR_RING_ELEVATION       = LED_UPPER_RING_ELEVATION;
   static const Real PROXIMITY_SENSOR_RING_RADIUS          = LED_RING_RADIUS;
   static const CRadians PROXIMITY_SENSOR_RING_START_ANGLE = CRadians((ARGOS_PI / 12.0f) * 0.5f);
   static const Real PROXIMITY_SENSOR_RING_RANGE           = 3.0f;

   /****************************************/
   /****************************************/

   CEyeBotEntity::CEyeBotEntity() :
      CComposableEntity(NULL),
      m_pcControllableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcLEDEquippedEntity(NULL),
      m_pcLightSensorEquippedEntity(NULL),
      m_pcProximitySensorEquippedEntity(NULL),
      m_pcQuadRotorEntity(NULL),
      m_pcRABEquippedEntity(NULL) {
   }

   /****************************************/
   /****************************************/

   CEyeBotEntity::CEyeBotEntity(const std::string& str_id,
                                const std::string& str_controller_id,
                                const CVector3& c_position,
                                const CQuaternion& c_orientation,
                                Real f_rab_range,
                                size_t un_rab_data_size) :
      CComposableEntity(NULL, str_id),
      m_pcControllableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcLEDEquippedEntity(NULL),
      m_pcLightSensorEquippedEntity(NULL),
      m_pcProximitySensorEquippedEntity(NULL),
      m_pcQuadRotorEntity(NULL),
      m_pcRABEquippedEntity(NULL) {
      try {
         /*
          * Create and init components
          */
         /*
          * Embodied entity
          * Better to put this first, because many other entities need this one
          */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this, "body_0", c_position, c_orientation);
         AddComponent(*m_pcEmbodiedEntity);
         /* Quadrotor entity */
         m_pcQuadRotorEntity = new CQuadRotorEntity(this, "quadrotor_0");
         AddComponent(*m_pcQuadRotorEntity);
         /* LED equipped entity, with LEDs [0-11] and beacon [12] */
         m_pcLEDEquippedEntity = new CLEDEquippedEntity(this, "leds_0");
         m_pcLEDEquippedEntity->AddLEDRing(
            CVector3(0.0f, 0.0f, LED_LOWER_RING_ELEVATION),
            LED_RING_RADIUS,
            HALF_LED_ANGLE_SLICE,
            16,
            m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcLEDEquippedEntity->AddLEDRing(
            CVector3(0.0f, 0.0f, LED_UPPER_RING_ELEVATION),
            LED_RING_RADIUS,
            HALF_LED_ANGLE_SLICE,
            16,
            m_pcEmbodiedEntity->GetOriginAnchor());
         CVector3 cLEDPos(LED_RING_RADIUS * 0.7f,
                          0.0f,
                          LED_LOWER_RING_ELEVATION - 0.01f);
         cLEDPos.RotateZ(3.0f * CRadians::PI_OVER_FOUR);
         m_pcLEDEquippedEntity->AddLED(
            cLEDPos,
            m_pcEmbodiedEntity->GetOriginAnchor());
         AddComponent(*m_pcLEDEquippedEntity);
         /* Proximity sensor equipped entity */
         m_pcProximitySensorEquippedEntity =
            new CProximitySensorEquippedEntity(this,
                                               "proximity_0");
         AddComponent(*m_pcProximitySensorEquippedEntity);
         m_pcProximitySensorEquippedEntity->AddSensorRing(
            CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
            PROXIMITY_SENSOR_RING_RADIUS,
            PROXIMITY_SENSOR_RING_START_ANGLE,
            PROXIMITY_SENSOR_RING_RANGE,
            24,
            m_pcEmbodiedEntity->GetOriginAnchor());
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
            24,
            m_pcEmbodiedEntity->GetOriginAnchor());
         /* RAB equipped entity */
         m_pcRABEquippedEntity = new CRABEquippedEntity(
            this,
            "rab_0",
            un_rab_data_size,
            f_rab_range,
            m_pcEmbodiedEntity->GetOriginAnchor(),
            *m_pcEmbodiedEntity);
         AddComponent(*m_pcRABEquippedEntity);
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

   void CEyeBotEntity::Init(TConfigurationNode& t_tree) {
      try {
         /*
          * Init parent
          */
         CComposableEntity::Init(t_tree);
         /*
          * Create and init components
          */
         /*
          * Embodied entity
          * Better to put this first, because many other entities need this one
          */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         /* Quadrotor entity */
         m_pcQuadRotorEntity = new CQuadRotorEntity(this, "quadrotor_0");
         AddComponent(*m_pcQuadRotorEntity);
         /* LED equipped entity, with LEDs [0-11] and beacon [12] */
         m_pcLEDEquippedEntity = new CLEDEquippedEntity(this, "leds_0");
         m_pcLEDEquippedEntity->AddLEDRing(
            CVector3(0.0f, 0.0f, LED_LOWER_RING_ELEVATION),
            LED_RING_RADIUS,
            HALF_LED_ANGLE_SLICE,
            16,
            m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcLEDEquippedEntity->AddLEDRing(
            CVector3(0.0f, 0.0f, LED_UPPER_RING_ELEVATION),
            LED_RING_RADIUS,
            HALF_LED_ANGLE_SLICE,
            16,
            m_pcEmbodiedEntity->GetOriginAnchor());
         CVector3 cLEDPos(LED_RING_RADIUS * 0.7f,
                          0.0f,
                          LED_LOWER_RING_ELEVATION - 0.01f);
         cLEDPos.RotateZ(3.0f * CRadians::PI_OVER_FOUR);
         m_pcLEDEquippedEntity->AddLED(
            cLEDPos,
            m_pcEmbodiedEntity->GetOriginAnchor());
         AddComponent(*m_pcLEDEquippedEntity);
         /* Proximity sensor equipped entity */
         m_pcProximitySensorEquippedEntity =
            new CProximitySensorEquippedEntity(this,
                                               "proximity_0");
         AddComponent(*m_pcProximitySensorEquippedEntity);
         m_pcProximitySensorEquippedEntity->AddSensorRing(
            CVector3(0.0f, 0.0f, PROXIMITY_SENSOR_RING_ELEVATION),
            PROXIMITY_SENSOR_RING_RADIUS,
            PROXIMITY_SENSOR_RING_START_ANGLE,
            PROXIMITY_SENSOR_RING_RANGE,
            24,
            m_pcEmbodiedEntity->GetOriginAnchor());
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
            24,
            m_pcEmbodiedEntity->GetOriginAnchor());
         /* RAB equipped entity */
         Real fRange = 3.0f;
         GetNodeAttributeOrDefault(t_tree, "rab_range", fRange, fRange);
         UInt32 unDataSize = 10;
         GetNodeAttributeOrDefault(t_tree, "rab_data_size", unDataSize, unDataSize);
         m_pcRABEquippedEntity = new CRABEquippedEntity(
            this,
            "rab_0",
            unDataSize,
            fRange,
            m_pcEmbodiedEntity->GetOriginAnchor(),
            *m_pcEmbodiedEntity);
         AddComponent(*m_pcRABEquippedEntity);
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

   void CEyeBotEntity::Reset() {
      /* Reset all components */
      CComposableEntity::Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

#define UPDATE(COMPONENT) if(COMPONENT->IsEnabled()) COMPONENT->Update();

   void CEyeBotEntity::UpdateComponents() {
      UPDATE(m_pcRABEquippedEntity);
      UPDATE(m_pcLEDEquippedEntity);
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CEyeBotEntity,
                   "eye-bot",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The eye-bot robot, developed in the Swarmanoid project.",
                   "The eye-bot is a quad-rotor developed in the Swarmanoid Project. It is a\n"
                   "fully autonomous robot with a rich set of sensors and actuators. For more\n"
                   "information, refer to the dedicated web page\n"
                   "(http://www.swarmanoid.org/swarmanoid_hardware.php).\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <eye-bot id=\"eb0\">\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,0,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "    </eye-bot>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'body/position' attribute specifies the position of the bottom point of the\n"
                   "eye-bot in the arena. When the robot is untranslated and unrotated, the\n"
                   "bottom point is in the origin and it is defined as the middle point between\n"
                   "the two wheels on the XY plane and the lowest point of the robot on the Z\n"
                   "axis, that is the point where the wheels touch the floor. The attribute values\n"
                   "are in the X,Y,Z order.\n"
                   "The 'body/orientation' attribute specifies the orientation of the eye-bot. All\n"
                   "rotations are performed with respect to the bottom point. The order of the\n"
                   "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                   "around the Z axis, the second around Y and the last around X. This reflects\n"
                   "the internal convention used in ARGoS, in which rotations are performed in\n"
                   "that order. Angles are expressed in degrees. When the robot is unrotated, it\n"
                   "is oriented along the X axis.\n"
                   "The 'controller/config' attribute is used to assign a controller to the\n"
                   "eye-bot. The value of the attribute must be set to the id of a previously\n"
                   "defined controller. Controllers are defined in the <controllers> XML subtree.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "You can set the emission range of the range-and-bearing system. By default, a\n"
                   "message sent by an eye-bot can be received up to 3m. By using the 'rab_range'\n"
                   "attribute, you can change it to, i.e., 4m as follows:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <eye-bot id=\"eb0\" rab_range=\"4\">\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,0,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "    </eye-bot>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "You can also set the data sent at each time step through the range-and-bearing"
                   "system. By default, a message sent by a eye-bot is 10 bytes long. By using the"
                   "'rab_data_size' attribute, you can change it to, i.e., 20 bytes as follows:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <eye-bot id=\"eb0\" rab_data_size=\"20\">\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,0,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "    </eye-bot>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   ,
                   "Under development"
      );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CEyeBotEntity);

   /****************************************/
   /****************************************/

}
