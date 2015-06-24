/**
 * @file <argos3/plugins/simulator/entities/box_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "box_entity.h"
#include <argos3/core/utility/math/matrix/rotationmatrix3.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/media/led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBoxEntity::CBoxEntity():
      CComposableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcLEDEquippedEntity(NULL),
      m_fMass(1.0f),
      m_pcLEDMedium(NULL) {}

   /****************************************/
   /****************************************/

   CBoxEntity::CBoxEntity(const std::string& str_id,
                          const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_movable,
                          const CVector3& c_size,
                          Real f_mass) :
      CComposableEntity(NULL, str_id),
      m_pcEmbodiedEntity(
         new CEmbodiedEntity(this,
                             "body_0",
                             c_position,
                             c_orientation,
                             b_movable)),
      m_pcLEDEquippedEntity(
         new CLEDEquippedEntity(this,
                                "leds_0")),
      m_cSize(c_size),
      m_fMass(f_mass) {
      AddComponent(*m_pcEmbodiedEntity);
      AddComponent(*m_pcLEDEquippedEntity);
   }

   /****************************************/
   /****************************************/

   void CBoxEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CComposableEntity::Init(t_tree);
         /* Parse XML to get the size */
         GetNodeAttribute(t_tree, "size", m_cSize);
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
            m_pcLEDEquippedEntity->AddToMedium(*m_pcLEDMedium);
         }
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize box entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBoxEntity::Reset() {
      /* Reset all components */
      CComposableEntity::Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CBoxEntity::EnableLEDs(CLEDMedium& c_medium) {
      m_pcLEDEquippedEntity->AddToMedium(c_medium);
   }

   /****************************************/
   /****************************************/

   void CBoxEntity::DisableLEDs() {
      m_pcLEDEquippedEntity->RemoveFromMedium();
   }
   
   /****************************************/
   /****************************************/

   void CBoxEntity::AddLED(const CVector3& c_offset,
                           const CColor& c_color) {
      m_pcLEDEquippedEntity->AddLED(c_offset,
                                    GetEmbodiedEntity().GetOriginAnchor(),
                                    c_color);
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CBoxEntity,
                   "box",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "A stretchable 3D box.",
                   "The box entity can be used to model walls, obstacles or box-shaped grippable\n"
                   "objects. It can be movable or not. A movable object can be pushed and gripped.\n"
                   "An unmovable object is pretty much like a wall.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "To declare an unmovable object (i.e., a wall) you need the following:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <box id=\"box1\" size=\"0.75,0.1,0.5\" movable=\"false\">\n"
                   "      <body position=\"0.4,2.3,0\" orientation=\"45,0,0\" />\n"
                   "    </box>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "To declare a movable object you need the following:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <box id=\"box1\" size=\"0.75,0.1,0.5\" movable=\"true\" mass=\"2.5\">\n"
                   "      <body position=\"0.4,2.3,0\" orientation=\"45,0,0\" />\n"
                   "    </box>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'size' attribute specifies the size of the box along the three axes, in\n"
                   "the X,Y,Z order. When you add a box, imagine it initially unrotated and\n"
                   "centered in the origin. The size, then, corresponds to the extent along the X,\n"
                   "Y and Z axes.\n"
                   "The 'movable' attribute specifies whether or not the object is movable. When\n"
                   "set to 'false', the object is unmovable: if another object pushes against it,\n"
                   "the box won't move. When the attribute is set to 'true', the box is movable\n"
                   "upon pushing or gripping. When an object is movable, the 'mass' attribute is\n"
                   "required.\n"
                   "The 'mass' attribute quantifies the mass of the box in kg.\n"
                   "The 'body/position' attribute specifies the position of the base of the box in\n"
                   "the arena. The three values are in the X,Y,Z order.\n"
                   "The 'body/orientation' attribute specifies the orientation of the 3D box. All\n"
                   "rotations are performed with respect to the center of mass. The order of the\n"
                   "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                   "around the Z axis, the second around Y and the last around X. This reflects\n"
                   "the internal convention used in ARGoS, in which rotations are performed in\n"
                   "that order. Angles are expressed in degrees.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to add any number of colored LEDs to the box. In this way,\n"
                   "the box is visible with a robot camera. The position and color of the\n"
                   "LEDs is specified with the following syntax:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <box id=\"box1\" size=\"0.75,0.1,0.5\" movable=\"true\" mass=\"2.5\">\n"
                   "      <body position=\"0.4,2.3,0\" orientation=\"45,0,0\" />\n"
                   "      <leds medium=\"id_of_led_medium\">\n"
                   "        <led offset=\" 0.15, 0.15,0.15\" anchor=\"origin\" color=\"white\" />\n"
                   "        <led offset=\"-0.15, 0.15,0\"    anchor=\"origin\" color=\"red\"   />\n"
                   "        <led offset=\" 0.15, 0.15,0\"    anchor=\"origin\" color=\"blue\"  />\n"
                   "        <led offset=\" 0.15,-0.15,0\"    anchor=\"origin\" color=\"green\" />\n"
                   "      </leds>\n"
                   "    </box>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "In the example, four LEDs are added to the box. The LEDs have\n"
                   "different colors and are located one on the top and three\n"
                   "around the box. The LEDs are managed by the LED medium declared in\n"
                   "the <media> section of the configuration file with id \"id_of_led_medium\"",
                   "Usable"
      );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CBoxEntity);

   /****************************************/
   /****************************************/

}
