/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "prototype_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_link_equipped_entity.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_joint_equipped_entity.h>
#include <argos3/plugins/simulator/entities/proximity_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/magnet_equipped_entity.h>
#include <argos3/plugins/simulator/entities/radio_equipped_entity.h>
#include <argos3/plugins/simulator/entities/tag_equipped_entity.h>
#include <argos3/plugins/simulator/media/directional_led_medium.h>
#include <argos3/plugins/simulator/media/led_medium.h>
#include <argos3/plugins/simulator/media/radio_medium.h>
#include <argos3/plugins/simulator/media/tag_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPrototypeEntity::CPrototypeEntity() :
      CComposableEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcLinkEquippedEntity(nullptr),
      m_pcJointEquippedEntity(nullptr),
      m_pcDirectionalLEDEquippedEntity(nullptr),
      m_pcLEDEquippedEntity(nullptr),
      m_pcTagEquippedEntity(nullptr),
      m_pcRadioEquippedEntity(nullptr),
      m_pcMagnetEquippedEntity(nullptr),
      m_pcProximitySensorEquippedEntity(nullptr) {}

   /****************************************/
   /****************************************/

   void CPrototypeEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Initialize the parent */
         CComposableEntity::Init(t_tree);
         /* Check if the model is movable */
         bool bMovable = true;
         GetNodeAttributeOrDefault(t_tree, "movable", bMovable, bMovable);
         /* Initialize the body */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));
         m_pcEmbodiedEntity->SetMovable(bMovable);
         /* Initialize the links */
         m_pcLinkEquippedEntity = new CPrototypeLinkEquippedEntity(this);
         m_pcLinkEquippedEntity->Init(GetNode(t_tree, "links"));
         AddComponent(*m_pcLinkEquippedEntity);
         /* Initialize the joints */
         m_pcJointEquippedEntity = new CPrototypeJointEquippedEntity(this);
         AddComponent(*m_pcJointEquippedEntity);
         /* create an empty joints configuration node if one isn't provided */
         if(!NodeExists(t_tree, "joints")) {
            TConfigurationNode tJointsNode("joints");
            AddChildNode(t_tree, tJointsNode);
         }
         m_pcJointEquippedEntity->Init(GetNode(t_tree, "joints"));
         /* Initialize the devices */
         if(NodeExists(t_tree, "devices")) {
            TConfigurationNodeIterator itDevice;
            for(itDevice = itDevice.begin(&GetNode(t_tree, "devices"));
                itDevice != itDevice.end();
                ++itDevice) {
               if(itDevice->Value() == "directional_leds" ) {
                  m_pcDirectionalLEDEquippedEntity = 
                     new CDirectionalLEDEquippedEntity(this);
                  m_pcDirectionalLEDEquippedEntity->Init(*itDevice);
                  AddComponent(*m_pcDirectionalLEDEquippedEntity);
                  /* Add the directional LEDs to the medium */
                  std::string strMedium;
                  GetNodeAttribute(*itDevice, "medium", strMedium);
                  CDirectionalLEDMedium& cDirectionalLEDMedium = 
                     CSimulator::GetInstance().GetMedium<CDirectionalLEDMedium>(strMedium);
                  m_pcDirectionalLEDEquippedEntity->SetMedium(cDirectionalLEDMedium);
                  m_pcDirectionalLEDEquippedEntity->Enable();
               }
               else if(itDevice->Value() == "leds" ) {
                  m_pcLEDEquippedEntity = new CLEDEquippedEntity(this);
                  m_pcLEDEquippedEntity->Init(*itDevice);
                  AddComponent(*m_pcLEDEquippedEntity);
                  /* Add the LEDs to the medium */
                  std::string strMedium;
                  GetNodeAttribute(*itDevice, "medium", strMedium);
                  CLEDMedium& cLEDMedium = 
                     CSimulator::GetInstance().GetMedium<CLEDMedium>(strMedium);
                  m_pcLEDEquippedEntity->SetMedium(cLEDMedium);
                  m_pcLEDEquippedEntity->Enable();
               }
               else if(itDevice->Value() == "proximity_sensors") {
                  m_pcProximitySensorEquippedEntity = new CProximitySensorEquippedEntity(this);
                  m_pcProximitySensorEquippedEntity->Init(*itDevice);
                  AddComponent(*m_pcProximitySensorEquippedEntity);
                  m_pcProximitySensorEquippedEntity->Enable();
               }
               else if(itDevice->Value() == "tags" ) {
                  m_pcTagEquippedEntity = new CTagEquippedEntity(this);
                  m_pcTagEquippedEntity->Init(*itDevice);
                  AddComponent(*m_pcTagEquippedEntity);
                  /* Add the tags to the medium */
                  std::string strMedium;
                  GetNodeAttribute(*itDevice, "medium", strMedium);
                  CTagMedium& cTagMedium = 
                     CSimulator::GetInstance().GetMedium<CTagMedium>(strMedium);
                  m_pcTagEquippedEntity->SetMedium(cTagMedium);
                  m_pcTagEquippedEntity->Enable();
               }
               else if(itDevice->Value() == "radios" ) {
                  CRadioEquippedEntity* m_pcRadioEquippedEntity =
                     new CRadioEquippedEntity(this);
                  m_pcRadioEquippedEntity->Init(*itDevice);
                  AddComponent(*m_pcRadioEquippedEntity);
                  /* Add the radios to the medium */
                  std::string strMedium;
                  GetNodeAttribute(*itDevice, "medium", strMedium);
                  CRadioMedium& cRadioMedium = 
                     CSimulator::GetInstance().GetMedium<CRadioMedium>(strMedium);
                  m_pcRadioEquippedEntity->SetMedium(cRadioMedium);
                  m_pcRadioEquippedEntity->Enable();
               }
               else if(itDevice->Value() == "magnets" ) {
                  m_pcMagnetEquippedEntity = new CMagnetEquippedEntity(this);
                  m_pcMagnetEquippedEntity->Init(*itDevice);
                  AddComponent(*m_pcMagnetEquippedEntity);
                  m_pcMagnetEquippedEntity->Enable();
               }
               else {
                  THROW_ARGOSEXCEPTION("Device type \"" << itDevice->Value() << "\" not implemented");
               }
            }
         }
         /* Initialize the controllable entity, this is done last so that the sensors and
            actuators correctly link to the entity */
         if(NodeExists(t_tree, "controller")) {
            m_pcControllableEntity = new CControllableEntity(this);
            AddComponent(*m_pcControllableEntity);
            m_pcControllableEntity->Init(GetNode(t_tree, "controller"));
         }
         /* Update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CPrototypeEntity,
                   "prototype",
                   "1.0",
                   "Michael Allwright [allsey87@gmail.com]",
                   "A generic and configurable entity",
                   "The prototyping entity allows users to quickly implement new entities using an\n"
                   "experiment's configuration file. A prototype entity is predominantly described\n"
                   "as a set of links connected through joints. The joints can be fixed, prismatic,\n"
                   "revolute, spherical, or planar. Sensors and actuators for prismatic and revolute\n"
                   "joints are provided. Since each link defines an anchor in the embodied entity,\n"
                   "it is also possible to attach the following entities to any link in the model:\n"
                   "the directional LED entity, the LED entity, the magnet entity, the tag entity,\n"
                   "the radio entity, and the proximity sensor equipped entity.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <prototype id=\"pt0\" movable=\"true\">\n"
                   "      <body position=\"0.1,0.1,0.0\" orientation=\"45,0,0\" />\n"
                   "      <links ref=\"base\">\n"
                   "        <link id=\"base\" geometry=\"box\" size=\"0.1,0.1,0.1\"\n"
                   "              mass=\"1\" position=\"0,0,0\" orientation=\"0,0,0\" />\n"
                   "    </prototype>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id the initialization aborts and the simulator will\n"
                   "terminate. The movable attribute defines whether the reference link of a robot\n"
                   "is fixed or not. This attribute is useful, for example, if you want to model a\n"
                   "robotic arm whose links are movable, but where there is one link (i.e. the\n"
                   "reference link) that is fixed to the ground, a wall, or the ceiling.\n"
                   "The body tag defines the position and orientation of the prototype entity in the\n"
                   "global coordinate system. The most simple entity contains a single link and no\n"
                   "joints. An important detail to note is the 'ref' attribute of the links node.\n"
                   "This attribute specifies which link is to be used to refer to the prototype\n"
                   "entity's location. The example in the required configuration defines a single\n"
                   "link with a box geometry. Three geometries for links are currently supported:\n"
                   "boxes, cylinders, and spheres. The size of a box is defined using the size\n"
                   "attribute while cylinders and spheres are defined using a radius attribute and a\n"
                   "height attribute in the case of the cylinder. The mass of each link must be\n"
                   "defined and specified in kilograms. The position and orientation of each link\n"
                   "refer to the offset of the link in the entity's local coordinate system.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "In the following example, additional links and joints are added to a prototype\n"
                   "entity to form a small four-wheeled vehicle.\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <prototype id=\"vehicle\" movable=\"true\">\n"
                   "      <body position=\"0,0,0\" orientation=\"0,0,0\" />\n"
                   "      <links ref=\"base\">\n"
                   "        <link id=\"base\" geometry=\"box\" size=\"0.1,0.05,0.02\" mass=\"1\"\n"
                   "              position=\"0,0,0.01\" orientation=\"0,0,0\" />\n"
                   "        <link id=\"wheel_fl\" geometry=\"cylinder\" radius=\"0.02\" height=\".005\"\n"
                   "              mass=\".05\" position=\".025,.025,0.02\" orientation=\"0,0,-90\" />\n"
                   "        <link id=\"wheel_fr\" geometry=\"cylinder\" radius=\"0.02\" height=\".005\"\n"
                   "              mass=\".05\" position=\".025,-.025,0.02\" orientation=\"0,0,90\" />\n"
                   "        <link id=\"wheel_br\" geometry=\"cylinder\" radius=\"0.02\" height=\".005\"\n"
                   "              mass=\".05\" position=\"-.025,-.025,0.02\" orientation=\"0,0,90\" />\n"
                   "        <link id=\"wheel_bl\" geometry=\"cylinder\" radius=\"0.02\" height=\".005\"\n"
                   "              mass=\".05\" position=\"-.025,.025,0.02\" orientation=\"0,0,-90\" />\n"
                   "      </links>\n"
                   "      <joints>\n"
                   "        <joint id=\"base_wheel_fr\" type=\"revolute\" axis=\"0,0,1\">\n"
                   "          <parent link=\"base\" position=\".025,-.025,.01\" orientation=\"0,0,90\"/>\n"
                   "          <child link=\"wheel_fr\" position=\"0,0,0\" orientation=\"0,0,0\"/>\n"
                   "        </joint>\n"
                   "        <joint id=\"base_wheel_fl\" type=\"revolute\" axis=\"0,0,1\">\n"
                   "          <parent link=\"base\" position=\".025,.025,.01\" orientation=\"0,0,-90\"/>\n"
                   "          <child link=\"wheel_fl\" position=\"0,0,0\" orientation=\"0,0,0\"/>\n"
                   "        </joint>\n"
                   "        <joint id=\"base_wheel_br\" type=\"revolute\" axis=\"0,0,1\">\n"
                   "          <parent link=\"base\" position=\"-.025,-.025,.01\" orientation=\"0,0,90\"/>\n"
                   "          <child link=\"wheel_br\" position=\"0,0,0\" orientation=\"0,0,0\"/>\n"
                   "        </joint>\n"
                   "        <joint id=\"base_wheel_bl\" type=\"revolute\" axis=\"0,0,1\">\n"
                   "          <parent link=\"base\" position=\"-.025,.025,.01\" orientation=\"0,0,-90\"/>\n"
                   "          <child link=\"wheel_bl\" position=\"0,0,0\" orientation=\"0,0,0\"/>\n"
                   "        </joint>\n"
                   "      </joints>\n"
                   "    </prototype>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "In this example, there are five links: the base of the vehicle and four wheels.\n"
                   "The wheels are attached to the base using revolute joints. The joint node\n"
                   "requires the joint type attribute and in the case of prismatic and revolute\n"
                   "joints an axis which defines the joint's degree of freedom. In addition to the\n"
                   "axis attribute, prismatic and revolute joints can also be limited using the\n"
                   "limit attribute. In the case of prismatic joints, this value is interpreted as a\n"
                   "range in meters and in the case of revolute joints, this value is interpreted as\n"
                   "a range in degrees. The maximum and minimum values must be separated with the\n"
                   "colon character. The joint node requires two child nodes, namely the parent and\n"
                   "child nodes which describe the links involved in the joint. Each of these nodes\n"
                   "also specifies a position and an orientation, which defines the offset from the\n"
                   "link's local coordinate system to the joint's reference frame. Note that the\n"
                   "prototype entity is consistent with ARGoS conventions and the origin of all\n"
                   "coordinate systems is at the base of an entity and not at its center of mass.\n"
                   "Prismatic and revolute joints support sensors and actuators which can control\n"
                   "and monitor the position and velocity of a joint. Refer to the documentation on\n"
                   "the joints sensor and joints actuator by querying the joints plugin. It is also\n"
                   "possible to attach any number of devices to the links in a prototype entity:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <prototype id=\"block\" movable=\"true\">\n"
                   "    <body position=\"0,0,0\" orientation=\"0,0,0\" />\n"
                   "      <links ref=\"base\">\n"
                   "        <link id=\"base\" geometry=\"box\" size=\".015,.015,.015\" mass=\"0.1\"\n"
                   "              position=\"0,0,0\" orientation=\"0,0,0\" />\n"
                   "      </links>\n"
                   "      <devices>\n"
                   "        <directional_leds medium=\"leds\">\n"
                   "          <directional_led anchor=\"base\" color=\"red\" observable_angle=\"180\"\n"
                   "                           position=\"0,0,.0155\" orientation=\"0,0,0\"/>\n"
                   "        </directional_leds>\n"
                   "        <tags medium=\"tags\">\n"
                   "          <tag position=\"0,0,.0155\" orientation=\"0,0,0\" payload=\"0\"\n"
                   "               side_length=\".01\" observable_angle=\"180\" anchor=\"base\"/>\n"
                   "        </tags>\n"
                   "        <radios medium=\"nfc\">\n"
                   "          <radio anchor=\"base\" duplex_mode=\"half\" range=\"1\"\n"
                   "                 position=\"0,0,0.0075\"/>\n"
                   "        </radios>\n"
                   "      </devices>\n"
                   "    </prototype>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "Each of these devices has three attributes in common: an anchor, a position, and\n"
                   "an orientation. Using these attributes you can attach any number of these\n"
                   "entities to any link in the prototype entity since each link creates an anchor.\n"
                   "In addition to these attributes, the entities have some attributes that define\n"
                   "their characteristics. For example, tags have a side length and payload, while\n"
                   "the directional LED has a color. The three devices in the example all require\n"
                   "mediums. These mediums index the entities and allow them to be detected by\n"
                   "sensors.",
                   "Usable"
   );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CPrototypeEntity);

   /****************************************/
   /****************************************/
}
