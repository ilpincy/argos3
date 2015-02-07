/**
 * @file <argos3/plugins/robots/mini-quadrotor/simulator/miniquadrotor_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "miniquadrotor_entity.h"

#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/rotor_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real ARM_LENGTH = 0.63f;

   /****************************************/
   /****************************************/

   CMiniQuadrotorEntity::CMiniQuadrotorEntity() :
      CComposableEntity(NULL),
      m_pcControllableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcRotorEquippedEntity(NULL) {
   }

   /****************************************/
   /****************************************/

   CMiniQuadrotorEntity::CMiniQuadrotorEntity(const std::string& str_id,
                                              const std::string& str_controller_id,
                                              const CVector3& c_position,
                                              const CQuaternion& c_orientation) :
      CComposableEntity(NULL, str_id),
      m_pcControllableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcRotorEquippedEntity(NULL) {
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
         /* Rotor equipped entity */
         m_pcRotorEquippedEntity = new CRotorEquippedEntity(this, "rotors_0", 4);
         m_pcRotorEquippedEntity->SetRotor(0, CVector3(ARM_LENGTH, 0.0f, 0.0f));
         m_pcRotorEquippedEntity->SetRotor(1, CVector3(0.0f, ARM_LENGTH, 0.0f));
         m_pcRotorEquippedEntity->SetRotor(2, CVector3(-ARM_LENGTH, 0.0f, 0.0f));
         m_pcRotorEquippedEntity->SetRotor(3, CVector3(0.0f, -ARM_LENGTH, 0.0f));
         AddComponent(*m_pcRotorEquippedEntity);
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

   void CMiniQuadrotorEntity::Init(TConfigurationNode& t_tree) {
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
         /* Rotor equipped entity */
         m_pcRotorEquippedEntity = new CRotorEquippedEntity(this, "rotors_0", 4);
         m_pcRotorEquippedEntity->SetRotor(0, CVector3(ARM_LENGTH, 0.0f, 0.0f));
         m_pcRotorEquippedEntity->SetRotor(1, CVector3(0.0f, ARM_LENGTH, 0.0f));
         m_pcRotorEquippedEntity->SetRotor(2, CVector3(-ARM_LENGTH, 0.0f, 0.0f));
         m_pcRotorEquippedEntity->SetRotor(3, CVector3(0.0f, -ARM_LENGTH, 0.0f));
         AddComponent(*m_pcRotorEquippedEntity);
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

   REGISTER_ENTITY(CMiniQuadrotorEntity,
                   "mini-quadrotor",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The mini-quadrotor robot, developed at UPenn.",
                   "The mini-quadrotor is a fluing robot developed at UPenn by Vijay Kumar's group.\n"
                   "It is a very agile robot, able to perform fast and precise movements.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <mini-quadrotor id=\"mq0\">\n"
                   "      <body position=\"0.4,2.3,2.0\" orientation=\"45,0,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "    </mini-quadrotor>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'body/position' attribute specifies the position of the bottom point of the\n"
                   "mini-quadrotor in the arena. When the robot is untranslated and unrotated, the\n"
                   "bottom point is in the origin and it is defined as the middle point between\n"
                   "the two wheels on the XY plane and the lowest point of the robot on the Z\n"
                   "axis, that is the point where the wheels touch the floor. The attribute values\n"
                   "are in the X,Y,Z order.\n"
                   "The 'body/orientation' attribute specifies the orientation of the\n"
                   "mini-quadrotor. All rotations are performed with respect to the bottom point.\n"
                   "The order of the angles is Z,Y,X, which means that the first number corresponds\n"
                   "to the rotation around the Z axis, the second around Y and the last around X.\n"
                   "This reflects the internal convention used in ARGoS, in which rotations are\n"
                   "performed in that order. Angles are expressed in degrees. When the robot is\n"
                   "unrotated, it is oriented along the X axis.\n"
                   "The 'controller/config' attribute is used to assign a controller to the\n"
                   "mini-quadrotor. The value of the attribute must be set to the id of a previously\n"
                   "defined controller. Controllers are defined in the <controllers> XML subtree.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None.\n\n"
                   ,
                   "Under development"
      );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CMiniQuadrotorEntity);

   /****************************************/
   /****************************************/

}
