/**
 * @file <argos3/plugins/simulator/entities/cylinder_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "cylinder_entity.h"
#include <argos3/core/utility/math/matrix/rotationmatrix3.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/media/led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CCylinderEntity::CCylinderEntity() :
      CComposableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcLEDEquippedEntity(NULL),
      m_fMass(1.0f),
      m_pcLEDMedium(NULL) {
   }

   /****************************************/
   /****************************************/

   CCylinderEntity::CCylinderEntity(const std::string& str_id,
                                    const CVector3& c_position,
                                    const CQuaternion& c_orientation,
                                    bool b_movable,
                                    Real f_radius,
                                    Real f_height,
                                    Real f_mass) :
      CComposableEntity(NULL, str_id),
      m_pcEmbodiedEntity(
         new CEmbodiedEntity(this,
                             "body_0",
                             c_position,
                             c_orientation,
                             b_movable)),
      m_pcLEDEquippedEntity(
         new CLEDEquippedEntity(this, "leds_0")),
      m_fRadius(f_radius),
      m_fHeight(f_height),
      m_fMass(f_mass) {
      AddComponent(*m_pcEmbodiedEntity);
      AddComponent(*m_pcLEDEquippedEntity);
   }

   /****************************************/
   /****************************************/

   void CCylinderEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CComposableEntity::Init(t_tree);
         /* Parse XML to get the radius */
         GetNodeAttribute(t_tree, "radius", m_fRadius);
         /* Parse XML to get the height */
         GetNodeAttribute(t_tree, "height", m_fHeight);
         /* Parse XML to get the movable attribute */
         bool bMovable;
         GetNodeAttribute(t_tree, "movable", bMovable);
         if(bMovable) {
            /* Parse XML to get the mass */
            GetNodeAttribute(t_tree, "mass", m_fMass);
         }
         else {
            m_fMass = 0.0f;
         }
         /* Create embodied entity using parsed data */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         m_pcEmbodiedEntity->SetMovable(bMovable);
         /* Init LED equipped entity component */
         m_pcLEDEquippedEntity = new CLEDEquippedEntity(this);
         AddComponent(*m_pcLEDEquippedEntity);
         if(NodeExists(t_tree, "leds")) {
            /* Create LED equipped entity
             * NOTE: the LEDs are not added to the medium yet
             */
            m_pcLEDEquippedEntity->Init(GetNode(t_tree, "leds"));
            /* Add the LEDs to the medium */
            std::string strMedium;
            GetNodeAttribute(GetNode(t_tree, "leds"), "medium", strMedium);
            m_pcLEDMedium = &CSimulator::GetInstance().GetMedium<CLEDMedium>(strMedium);
            m_pcLEDEquippedEntity->SetMedium(*m_pcLEDMedium);
            m_pcLEDEquippedEntity->Enable();
         }
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize the cylinder entity.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CCylinderEntity::Reset() {
      /* Reset all components */
      m_pcEmbodiedEntity->Reset();
      m_pcLEDEquippedEntity->Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CCylinderEntity::EnableLEDs(CLEDMedium& c_medium) {
      m_pcLEDMedium = &c_medium;
      m_pcLEDEquippedEntity->SetMedium(*m_pcLEDMedium);
      m_pcLEDEquippedEntity->Enable();
   }

   /****************************************/
   /****************************************/

   void CCylinderEntity::DisableLEDs() {
      m_pcLEDEquippedEntity->Disable();
   }
   
   /****************************************/
   /****************************************/

   void CCylinderEntity::AddLED(const CVector3& c_offset,
                                const CColor& c_color) {
      m_pcLEDEquippedEntity->AddLED(c_offset,
                                    GetEmbodiedEntity().GetOriginAnchor(),
                                    c_color);
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CCylinderEntity,
                   "cylinder",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "A stretchable cylinder.",
                   "The cylinder entity can be used to model obstacles or cylinder-shaped\n"
                   "grippable objects. The cylinder has a red LED on the center of one\n"
                   "of the circular surfaces, that allows perception using the cameras.\n"
                   "The cylinder can be movable or not. A movable object can be pushed\n"
                   "and gripped. An unmovable object is pretty much like a column.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "To declare an unmovable object (i.e., a column) you need the following:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <cylinder id=\"cyl1\" radius=\"0.8\" height=\"0.5\" movable=\"false\">\n"
                   "      <body position=\"0.4,2.3,0\" orientation=\"45,0,0\" />\n"
                   "    </cylinder>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "To declare a movable object you need the following:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <cylinder id=\"cyl1\" radius=\"0.8\" height=\"0.5\"\n"
                   "              movable=\"true\" mass=\"2.5\">\n"
                   "      <body position=\"0.4,2.3,0\" orientation=\"45,0,0\" />\n"
                   "    </cylinder>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'radius' and 'height' attributes specify the size of the cylinder. When\n"
                   "you add a cylinder, imagine it initially unrotated and centered in the origin.\n"
                   "The base of the cylinder, then, is parallel to the XY plane and its height\n"
                   "goes with the Z axis.\n"
                   "The 'movable' attribute specifies whether or not the object is movable. When\n"
                   "set to 'false', the object is unmovable: if another object pushes against it,\n"
                   "the cylinder won't move. When the attribute is set to 'true', the cylinder is\n"
                   "movable upon pushing or gripping. When an object is movable, the 'mass'\n"
                   "attribute is required.\n"
                   "The 'mass' attribute quantifies the mass of the cylinder in kg.\n"
                   "The 'body/position' attribute specifies the position of the base of the\n"
                   "cylinder in the arena. The three values are in the X,Y,Z order.\n"
                   "The 'body/orientation' attribute specifies the orientation of the cylinder. All\n"
                   "rotations are performed with respect to the center of mass. The order of the\n"
                   "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                   "around the Z axis, the second around Y and the last around X. This reflects\n"
                   "the internal convention used in ARGoS, in which rotations are performed in\n"
                   "that order. Angles are expressed in degrees.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to add any number of colored LEDs to the cylinder. In this way,\n"
                   "the cylinder is visible with a robot camera. The position and color of the\n"
                   "LEDs is specified with the following syntax:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <cylinder id=\"cyl1\" radius=\"0.8\" height=\"0.5\"\n"
                   "              movable=\"true\" mass=\"2.5\">\n"
                   "      <body position=\"0.4,2.3,0\" orientation=\"45,0,0\" />\n"
                   "      <leds medium=\"id_of_led_medium\">\n"
                   "        <led offset=\" 0.15, 0.15,0.15\" anchor=\"origin\" color=\"white\" />\n"
                   "        <led offset=\"-0.15, 0.15,0\"    anchor=\"origin\" color=\"red\"   />\n"
                   "        <led offset=\" 0.15, 0.15,0\"    anchor=\"origin\" color=\"blue\"  />\n"
                   "        <led offset=\" 0.15,-0.15,0\"    anchor=\"origin\" color=\"green\" />\n"
                   "      </leds>\n"
                   "    </cylinder>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "In the example, four LEDs are added around the cylinder. The LEDs have\n"
                   "different colors and are located around the cylinder. The LEDs are\n"
                   "managed by the LED medium declared in the <media> section of the\n"
                   "configuration file with id \"id_of_led_medium\"",
                   "Usable"
      );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CCylinderEntity);

   /****************************************/
   /****************************************/

}
