/**
 * @file <argos3/plugins/robots/spiri/simulator/spiri_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "spiri_entity.h"

#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/light_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/perspective_camera_equipped_entity.h>
#include <argos3/plugins/simulator/entities/proximity_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/quadrotor_entity.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>
#include <argos3/plugins/simulator/entities/battery_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real HEIGHT         = 0.090f;
   static const Real BODY_HEIGHT    = HEIGHT * 3.0f / 4.0f;
   static const Real BODY_SIDE      = 0.470f;
   static const Real BODY_DIAGONAL  = BODY_SIDE * ::sqrt(2);
   static const Real BODY_RADIUS    = BODY_DIAGONAL / 2.0f;
   static const Real BODY_ELEVATION = HEIGHT - BODY_HEIGHT;
   static const Real RAB_ELEVATION  = (BODY_ELEVATION + BODY_HEIGHT) / 2.0f;

   /****************************************/
   /****************************************/

   CSpiriEntity::CSpiriEntity() :
      CComposableEntity(NULL),
      m_pcControllableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcQuadRotorEntity(NULL),
      m_pcRABEquippedEntity(NULL),
      m_pcPerspectiveCameraEquippedEntity(NULL),
      m_pcBatteryEquippedEntity(NULL) {
   }

   /****************************************/
   /****************************************/

   CSpiriEntity::CSpiriEntity(const std::string& str_id,
                              const std::string& str_controller_id,
                              const CVector3& c_position,
                              const CQuaternion& c_orientation,
                              Real f_rab_range,
                              size_t un_rab_data_size,
                              const std::string& str_bat_model,
                              const CRadians& c_cam_aperture,
                              Real f_cam_range) :
      CComposableEntity(NULL, str_id),
      m_pcControllableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcQuadRotorEntity(NULL),
      m_pcRABEquippedEntity(NULL),
      m_pcPerspectiveCameraEquippedEntity(NULL),
      m_pcBatteryEquippedEntity(NULL) {
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
         /* RAB equipped entity */
         SAnchor& cRABAnchor = m_pcEmbodiedEntity->AddAnchor(
            "rab",
            CVector3(0.0f, 0.0f, RAB_ELEVATION));
         m_pcRABEquippedEntity = new CRABEquippedEntity(
            this,
            "rab_0",
            un_rab_data_size,
            f_rab_range,
            cRABAnchor,
            *m_pcEmbodiedEntity);
         AddComponent(*m_pcRABEquippedEntity);
         m_pcEmbodiedEntity->EnableAnchor("rab");
         /* Perspective camera */
         SAnchor& cCameraAnchor =
            m_pcEmbodiedEntity->AddAnchor(
               "camera",
               CVector3(BODY_RADIUS, 0.0f, 0.0f));
         m_pcPerspectiveCameraEquippedEntity = new CPerspectiveCameraEquippedEntity(
            this,
            "perspective_camera_0",
            c_cam_aperture,
            0.035f,
            f_cam_range,
            640, 480,
            cCameraAnchor);
         AddComponent(*m_pcPerspectiveCameraEquippedEntity);
         m_pcEmbodiedEntity->EnableAnchor("camera");
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

   void CSpiriEntity::Init(TConfigurationNode& t_tree) {
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
         /* RAB equipped entity */
         Real fRABRange = 3.0f;
         GetNodeAttributeOrDefault(t_tree, "rab_range", fRABRange, fRABRange);
         UInt32 unRABDataSize = 10;
         GetNodeAttributeOrDefault(t_tree, "rab_data_size", unRABDataSize, unRABDataSize);
         SAnchor& cRABAnchor = m_pcEmbodiedEntity->AddAnchor("rab", CVector3(0.0f, 0.0f, RAB_ELEVATION));
         m_pcRABEquippedEntity = new CRABEquippedEntity(
            this,
            "rab_0",
            unRABDataSize,
            fRABRange,
            cRABAnchor,
            *m_pcEmbodiedEntity);
         AddComponent(*m_pcRABEquippedEntity);
         m_pcEmbodiedEntity->EnableAnchor("rab");
         /* Perspective camera */
         CDegrees cCameraAperture(30.0f);
         GetNodeAttributeOrDefault(t_tree, "camera_aperture", cCameraAperture, cCameraAperture);
         Real fCameraRange = 10.0f;
         GetNodeAttributeOrDefault(t_tree, "camera_range", fCameraRange, fCameraRange);
         SAnchor& cCameraAnchor =
            m_pcEmbodiedEntity->AddAnchor(
               "camera",
               CVector3(BODY_RADIUS, 0.0f, 0.0f));
         m_pcPerspectiveCameraEquippedEntity = new CPerspectiveCameraEquippedEntity(
            this,
            "perspective_camera_0",
            ToRadians(cCameraAperture),
            0.035f,
            fCameraRange,
            640, 480,
            cCameraAnchor);
         AddComponent(*m_pcPerspectiveCameraEquippedEntity);
         m_pcEmbodiedEntity->EnableAnchor("camera");
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

   void CSpiriEntity::Reset() {
      /* Reset all components */
      CComposableEntity::Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CSpiriEntity,
                   "spiri",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The spiri robot, developed by Pleiades Robotics.",
                   "The spiri is a quad-rotor developed by Pleiades Robotics. It is a\n"
                   "fully autonomous robot with a rich set of sensors and actuators. For more\n"
                   "information, refer to the dedicated web page (http://www.pleaides.ca/).\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <spiri id=\"sp0\">\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,0,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "    </spiri>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'body/position' attribute specifies the position of the bottom point of the\n"
                   "spiri in the arena. When the robot is untranslated and unrotated, the\n"
                   "bottom point is in the origin and it is defined as the middle point between\n"
                   "the two wheels on the XY plane and the lowest point of the robot on the Z\n"
                   "axis, that is the point where the wheels touch the floor. The attribute values\n"
                   "are in the X,Y,Z order.\n"
                   "The 'body/orientation' attribute specifies the orientation of the spiri. All\n"
                   "rotations are performed with respect to the bottom point. The order of the\n"
                   "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                   "around the Z axis, the second around Y and the last around X. This reflects\n"
                   "the internal convention used in ARGoS, in which rotations are performed in\n"
                   "that order. Angles are expressed in degrees. When the robot is unrotated, it\n"
                   "is oriented along the X axis.\n"
                   "The 'controller/config' attribute is used to assign a controller to the\n"
                   "spiri. The value of the attribute must be set to the id of a previously\n"
                   "defined controller. Controllers are defined in the <controllers> XML subtree.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "You can set the emission range of the range-and-bearing system. By default, a\n"
                   "message sent by an spiri can be received up to 3m. By using the 'rab_range'\n"
                   "attribute, you can change it to, i.e., 4m as follows:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <spiri id=\"sp0\" rab_range=\"4\">\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,0,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "    </spiri>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "You can also set the data sent at each time step through the range-and-bearing\n"
                   "system. By default, a message sent by a spiri is 10 bytes long. By using the\n"
                   "'rab_data_size' attribute, you can change it to, i.e., 20 bytes as follows:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <spiri id=\"sp0\" rab_data_size=\"20\">\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,0,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "    </spiri>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "You can also configure the battery of the robot. By default, the battery never\n"
                   "depletes. You can choose among several battery discharge models, such as\n"
                   "- time: the battery depletes by a fixed amount at each time step\n"
                   "- motion: the battery depletes according to how the robot moves\n"
                   "- time_motion: a combination of the above models.\n"
                   "You can define your own models too. Follow the examples in the file\n"
                   "argos3/src/plugins/simulator/entities/battery_equipped_entity.cpp.\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <spiri id=\"sp0\"\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,0,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "      <battery model=\"time\" factor=\"1e-5\"/>\n"
                   "    </spiri>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <spiri id=\"sp0\"\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,0,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "      <battery model=\"motion\" pos_factor=\"1e-3\"\n"
                   "                              orient_factor=\"1e-3\"/>\n"
                   "    </spiri>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <spiri id=\"sp0\"\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,0,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "      <battery model=\"time_motion\" time_factor=\"1e-5\"\n"
                   "                                   pos_factor=\"1e-3\"\n"
                   "                                   orient_factor=\"1e-3\"/>\n"
                   "    </spiri>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   ,
                   "Under development"
      );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CSpiriEntity);

   /****************************************/
   /****************************************/

}
