/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "footbot_entity.h"

#include <argos3/core/simulator/entity/gripper_equipped_entity.h>
#include <argos3/core/simulator/entity/rab_equipped_entity.h>
#include "distance_scanner_equipped_entity.h"
#include <argos3/core/simulator/entity/led_equipped_entity.h>
#include <argos3/core/simulator/entity/wifi_equipped_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/utility/math/matrix/rotationmatrix3.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real FOOTBOT_RADIUS                   = 0.085036758f;
   static const Real FOOTBOT_HEIGHT                   = 0.146899733f;

   static const Real FOOTBOT_LED_RING_RADIUS          = FOOTBOT_RADIUS + 0.005;

   static const Real FOOTBOT_INTERWHEEL_DISTANCE      = 0.14f;
   static const Real FOOTBOT_HALF_INTERWHEEL_DISTANCE = FOOTBOT_INTERWHEEL_DISTANCE * 0.5f;

   static const Real FOOTBOT_LED_RING_ELEVATION       = 0.085f;
   static const Real FOOTBOT_BEACON_ELEVATION         = 0.174249733f;

   static const Real FOOTBOT_GRIPPER_ELEVATION        = FOOTBOT_LED_RING_ELEVATION;

   /* We can't use CRadians::PI and the likes here because of the 'static initialization order fiasco' */
   static const CRadians FOOTBOT_LED_ANGLE_SLICE      = CRadians(3.14159265358979323846264338327950288 / 6.0);
   static const CRadians FOOTBOT_HALF_LED_ANGLE_SLICE = FOOTBOT_LED_ANGLE_SLICE * 0.5f;

   /****************************************/
   /****************************************/

   class CFootBotEmbodiedEntity : public CEmbodiedEntity {

   public:

      CFootBotEmbodiedEntity(CFootBotEntity* pc_parent) :
         CEmbodiedEntity(pc_parent) {
         m_cHalfSize.SetX(FOOTBOT_RADIUS);
         m_cHalfSize.SetY(FOOTBOT_RADIUS);
         m_cHalfSize.SetZ(FOOTBOT_HEIGHT * 0.5f);
      }

   protected:

      virtual void CalculateBoundingBox() {
         m_cCenterPos = GetPosition();
         m_cCenterPos.SetZ(m_cCenterPos.GetZ() + m_cHalfSize.GetZ());
         m_cOrientationMatrix.SetFromQuaternion(GetOrientation());
         CalculateBoundingBoxFromHalfSize(GetBoundingBox(),
                                          m_cHalfSize,
                                          m_cCenterPos,
                                          m_cOrientationMatrix);
      }

   private:

      CVector3 m_cHalfSize;
      CVector3 m_cCenterPos;
      CRotationMatrix3 m_cOrientationMatrix;

   };

   /****************************************/
   /****************************************/

   CFootBotEntity::CFootBotEntity() :
      CComposableEntity(NULL),
      m_pcEmbodiedEntity(new CFootBotEmbodiedEntity(this)),
      m_pcControllableEntity(new CControllableEntity(this)),
      m_pcWheeledEntity(new CWheeledEntity<2>(this)),
      m_pcLEDEquippedEntity(new CLedEquippedEntity(this)),
      m_pcGripperEquippedEntity(new CGripperEquippedEntity(this)),
      m_pcDistanceScannerEquippedEntity(new CDistanceScannerEquippedEntity(this)),
      m_pcRABEquippedEntity(new CRABEquippedEntity(this, 10)),
      m_pcWiFiEquippedEntity(new CWiFiEquippedEntity(this)),
      m_fTurretRotationSpeed(0.0f),
      m_unTurretMode(0) {
      /* Left wheel position */
      m_pcWheeledEntity->SetWheelPosition(0, CVector3(0.0f,  FOOTBOT_HALF_INTERWHEEL_DISTANCE, 0.0f));
      /* Right wheel position */
      m_pcWheeledEntity->SetWheelPosition(1, CVector3(0.0f, -FOOTBOT_HALF_INTERWHEEL_DISTANCE, 0.0f));
      /* Add LEDs [0-11] and beacon [12] */
      for(UInt32 i = 0; i < 13; ++i) {
         m_pcLEDEquippedEntity->AddLed(CVector3());
      }
      /* Gripper position */
      m_pcGripperEquippedEntity->SetPosition(CVector3(FOOTBOT_RADIUS, 0.0f, FOOTBOT_GRIPPER_ELEVATION));
   }

   /****************************************/
   /****************************************/

   CFootBotEntity::~CFootBotEntity() {
      delete m_pcEmbodiedEntity;
      delete m_pcControllableEntity;
      delete m_pcWheeledEntity;
      delete m_pcLEDEquippedEntity;
      delete m_pcGripperEquippedEntity;
      delete m_pcDistanceScannerEquippedEntity;
      delete m_pcRABEquippedEntity;
      delete m_pcWiFiEquippedEntity;
   }

   /****************************************/
   /****************************************/

   void CFootBotEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CEntity::Init(t_tree);
         /* Init components */
         m_pcEmbodiedEntity->Init(t_tree);
         m_pcControllableEntity->Init(t_tree);
         m_pcWheeledEntity->Init(t_tree);
         m_pcLEDEquippedEntity->Init(t_tree);
         m_pcGripperEquippedEntity->Init(t_tree);
         m_pcDistanceScannerEquippedEntity->Init(t_tree);
         m_pcRABEquippedEntity->Init(t_tree);
         m_pcWiFiEquippedEntity->Init(t_tree);
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
      m_pcEmbodiedEntity->Reset();
      m_pcControllableEntity->Reset();
      m_pcWheeledEntity->Reset();
      m_pcLEDEquippedEntity->Reset();
      m_pcGripperEquippedEntity->Reset();
      m_pcDistanceScannerEquippedEntity->Reset();
      m_pcRABEquippedEntity->Reset();
      m_pcWiFiEquippedEntity->Reset();
      m_cTurretRotation = CRadians::ZERO;
      m_fTurretRotationSpeed = 0.0f;
      m_unTurretMode = 0;
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CFootBotEntity::Destroy() {
      m_pcEmbodiedEntity->Destroy();
      m_pcControllableEntity->Destroy();
      m_pcWheeledEntity->Destroy();
      m_pcLEDEquippedEntity->Destroy();
      m_pcGripperEquippedEntity->Destroy();
      m_pcDistanceScannerEquippedEntity->Destroy();
      m_pcRABEquippedEntity->Destroy(); 
      m_pcWiFiEquippedEntity->Destroy(); 
      m_cTurretRotation = CRadians::ZERO;
      m_fTurretRotationSpeed = 0.0f;
      m_unTurretMode = 0;
   }

   /****************************************/
   /****************************************/

   CEntity& CFootBotEntity::GetComponent(const std::string& str_component) {
      if(str_component == "embodied_entity") {
         return *m_pcEmbodiedEntity;
      }
      else if(str_component == "controllable_entity") {
         return *m_pcControllableEntity;
      }
      else if(str_component == "wheeled_entity<2>") {
         return *m_pcWheeledEntity;
      }
      else if(str_component == "led_equipped_entity") {
         return *m_pcLEDEquippedEntity;
      }
      else if(str_component == "gripper_equipped_entity") {
         return *m_pcGripperEquippedEntity;
      }
      else if(str_component == "distance_scanner_equipped_entity") {
         return *m_pcDistanceScannerEquippedEntity;
      }
      else if(str_component == "rab_equipped_entity") {
         return *m_pcRABEquippedEntity;
      }
      else if(str_component == "wifi_equipped_entity") {
         return *m_pcWiFiEquippedEntity;
      }
      else {
         THROW_ARGOSEXCEPTION("A foot-bot does not have a component of type \"" << str_component << "\"");
      }
   }

   /****************************************/
   /****************************************/

   bool CFootBotEntity::HasComponent(const std::string& str_component) {
      return (str_component == "embodied_entity"                  ||
              str_component == "controllable_entity"              ||
              str_component == "wheeled_entity<2>"                ||
              str_component == "led_equipped_entity"              ||
              str_component == "gripper_equipped_entity"          ||
              str_component == "distance_scanner_equipped_entity" ||
              str_component == "rab_equipped_entity" ||
              str_component == "wifi_equipped_entity");
   }

   /****************************************/
   /****************************************/

   void CFootBotEntity::UpdateComponents() {
      SetLedPosition();
      m_pcDistanceScannerEquippedEntity->UpdateRotation();
      m_pcEmbodiedEntity->UpdateBoundingBox();
      m_pcRABEquippedEntity->SetPosition(m_pcEmbodiedEntity->GetPosition());
   }

   /****************************************/
   /****************************************/
   
#define SET_RING_LED_POSITION(IDX)                                              \
   cLEDPosition.Set(FOOTBOT_LED_RING_RADIUS, 0.0f, FOOTBOT_LED_RING_ELEVATION); \
   cLEDAngle = cLEDAnglePhase;                                                 \
   cLEDAngle += FOOTBOT_LED_ANGLE_SLICE * IDX;                                  \
   cLEDPosition.RotateZ(cLEDAngle);                                             \
   cLEDPosition.Rotate(m_pcEmbodiedEntity->GetOrientation());                   \
   cLEDPosition += cEntityPosition;                                             \
   m_pcLEDEquippedEntity->SetLedPosition(IDX, cLEDPosition);
   
   void CFootBotEntity::SetLedPosition() {
      /* Set LED positions */
      const CVector3& cEntityPosition = GetEmbodiedEntity().GetPosition();
      CVector3 cLEDPosition;
      CRadians cLEDAnglePhase = FOOTBOT_HALF_LED_ANGLE_SLICE + m_cTurretRotation;
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
      cLEDPosition.Set(0.0f, 0.0f, FOOTBOT_BEACON_ELEVATION);
      cLEDPosition.Rotate(m_pcEmbodiedEntity->GetOrientation());
      cLEDPosition += cEntityPosition;
      m_pcLEDEquippedEntity->SetLedPosition(12, cLEDPosition);
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CFootBotEntity,
                   "foot-bot",
                   "1.0",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "The foot-bot robot, developed in the Swarmanoid project.",
                   "The foot-bot is a wheeled robot developed in the Swarmanoid Project. It is a\n"
                   "modular robot with a rich set of sensors and actuators. For more information,\n"
                   "refer to the dedicated web page\n"
                   "(http://www.swarmanoid.org/swarmanoid_hardware.php).\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <foot-bot id=\"fb0\"\n"
                   "              position=\"0.4,2.3,0.25\"\n"
                   "              orientation=\"45,90,0\"\n"
                   "              controller=\"mycntrl\" />\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'position' attribute specifies the position of the bottom point of the\n"
                   "foot-bot in the arena. When the robot is untranslated and unrotated, the\n"
                   "bottom point is in the origin and it is defined as the middle point between\n"
                   "the two wheels on the XY plane and the lowest point of the robot on the Z\n"
                   "axis, that is the point where the wheels touch the floor. The attribute values\n"
                   "are in the X,Y,Z order.\n"
                   "The 'orientation' attribute specifies the orientation of the foot-bot. All\n"
                   "rotations are performed with respect to the bottom point. The order of the\n"
                   "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                   "around the Z axis, the second around Y and the last around X. This reflects\n"
                   "the internal convention used in ARGoS, in which rotations are performed in\n"
                   "that order. Angles are expressed in degrees. When the robot is unrotated, it\n"
                   "is oriented along the X axis.\n"
                   "The 'controller' attribute is used to assign a controller to the foot-bot. The\n"
                   "value of the attribute must be set to the id of a previously defined\n"
                   "controller. Controllers are defined in the <controllers> XML subtree.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being.\n",
                   "Under development"
      );

}
