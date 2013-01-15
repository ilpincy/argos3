/**
 * @file <argos3/plugins/simulator/entities/box_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "box_entity.h"
#include <argos3/core/utility/math/general.h>
#include <argos3/core/utility/math/matrix/rotationmatrix3.h>

namespace argos {

   class CBoxEmbodiedEntity : public CEmbodiedEntity {

   public:

    	CBoxEmbodiedEntity(CBoxEntity* pc_parent,
                         const CVector3& c_half_size) :
         CEmbodiedEntity(pc_parent),
         m_cHalfSize(c_half_size) {}

   protected:

      virtual void CalculateBoundingBox() {
         m_cOrientationMatrix.SetFromQuaternion(GetOrientation());
         CalculateBoundingBoxFromHalfSize(GetBoundingBox(),
                                          m_cHalfSize,
                                          GetPosition(),
                                          m_cOrientationMatrix);
      }

   private:

      CVector3 m_cHalfSize;
      CRotationMatrix3 m_cOrientationMatrix;

   };

   /****************************************/
   /****************************************/

   CBoxEntity::CBoxEntity():
      CComposableEntity(NULL),
      m_pcLEDEquippedEntity(new CLedEquippedEntity(this)),
      m_fMass(1.0f) {
   }

   /****************************************/
   /****************************************/

   CBoxEntity::~CBoxEntity() {
    	delete m_pcEmbodiedEntity;
    	delete m_pcLEDEquippedEntity;
   }

   /****************************************/
   /****************************************/

   void CBoxEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CEntity::Init(t_tree);
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
         /* Init LED equipped entity component */
         m_pcLEDEquippedEntity->Init(t_tree);
         if(NodeExists(t_tree, "leds")) {
            TConfigurationNode& tLEDs = GetNode(t_tree, "leds");
            /* Go through the led entries */
            CVector3 cPosition;
            CColor cColor;
            TConfigurationNodeIterator itLED("led");
            for(itLED = itLED.begin(&tLEDs);
                itLED != itLED.end();
                ++itLED) {
               GetNodeAttribute(*itLED, "position", cPosition);
               GetNodeAttribute(*itLED, "color", cColor);
               m_vecBaseLEDPositions.push_back(cPosition);
               m_pcLEDEquippedEntity->AddLed(cPosition, cColor);
            }
         }

         /* Create embodied entity using parsed data */
         m_pcEmbodiedEntity = new CBoxEmbodiedEntity(this, m_cSize * 0.5f);
         m_pcEmbodiedEntity->Init(t_tree);
         m_pcEmbodiedEntity->SetMovable(bMovable);

         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBoxEntity::Reset() {
      /* Reset all components */
      m_pcEmbodiedEntity->Reset();
      m_pcLEDEquippedEntity->Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CBoxEntity::Destroy() {
      m_pcEmbodiedEntity->Destroy();
      m_pcLEDEquippedEntity->Destroy();
   }

   /****************************************/
   /****************************************/

   CEntity& CBoxEntity::GetComponent(const std::string& str_component) {
      if(str_component == "embodied_entity") {
         return *m_pcEmbodiedEntity;
      }
      else if(str_component == "led_equipped_entity") {
         return *m_pcLEDEquippedEntity;
      }
      else {
         THROW_ARGOSEXCEPTION("A box does not have a component of type \"" << str_component << "\"");
      }
   }

   /****************************************/
   /****************************************/

   bool CBoxEntity::HasComponent(const std::string& str_component) {
      return (str_component == "embodied_entity"     ||
              str_component == "led_equipped_entity" );
   }

   /****************************************/
   /****************************************/

   void CBoxEntity::UpdateComponents() {
      /* Set LED position and update led equipped entity */
      CVector3 cLEDPosition;
      for(UInt32 i = 0; i < m_pcLEDEquippedEntity->GetAllLeds().size(); ++i) {
         cLEDPosition = m_vecBaseLEDPositions[i];
         cLEDPosition.Rotate(m_pcEmbodiedEntity->GetOrientation());
         cLEDPosition += GetEmbodiedEntity().GetPosition();
         m_pcLEDEquippedEntity->SetLedPosition(i, cLEDPosition);
      }
      m_pcEmbodiedEntity->UpdateBoundingBox();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CBoxEntity,
                   "box",
                   "1.0",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "A stretchable 3D box.",
                   "The box entity can be used to model walls, obstacles or box-shaped grippable\n"
                   "objects. It can be movable or not. A movable object can be pushed and gripped.\n"
                   "An unmovable object is pretty much like a wall.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "To declare an unmovable object (i.e., a wall) you need the following:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <box id=\"box1\"\n"
                   "         position=\"0.4,2.3,0.25\"\n"
                   "         orientation=\"45,90,0\"\n"
                   "         size=\"0.75,0.1,0.5\"\n"
                   "         movable=\"false\" />\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "To declare a movable object you need the following:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <box id=\"box1\"\n"
                   "         position=\"0.4,2.3,0.25\"\n"
                   "         orientation=\"45,90,0\"\n"
                   "         size=\"0.75,0.1,0.5\"\n"
                   "         movable=\"true\"\n"
                   "         mass=\"2.5\" />\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'position' attribute specifies the position of the center of mass of the\n"
                   "box in the arena. The three values are in the X,Y,Z order.\n"
                   "The 'orientation' attribute specifies the orientation of the 3D box. All\n"
                   "rotations are performed with respect to the center of mass. The order of the\n"
                   "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                   "around the Z axis, the second around Y and the last around X. This reflects\n"
                   "the internal convention used in ARGoS, in which rotations are performed in\n"
                   "that order. Angles are expressed in degrees.\n"
                   "The 'size' attribute specifies the size of the box along the three axes, in\n"
                   "the X,Y,Z order. When you add a box, imagine it initially unrotated and\n"
                   "centered in the origin. The size, then, corresponds to the extent along the X,\n"
                   "Y and Z axes.\n"
                   "The 'movable' attribute specifies whether or not the object is movable. When\n"
                   "set to 'false', the object is unmovable: if another object pushes against it,\n"
                   "the box won't move. When the attribute is set to 'true', the box is movable\n"
                   "upon pushing or gripping. When an object is movable, the 'mass' attribute is\n"
                   "required.\n"
                   "The 'mass' attribute quantifies the mass of the box in kg.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to add any number of colored LEDs to the box. In this way,\n"
                   "the box is visible with a robot camera. The position and color of the\n"
                   "LEDs is specified with the following syntax:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <box id=\"box1\"\n"
                   "         position=\"0.4,0.2,0.25\"\n"
                   "         orientation=\"45,90,0\"\n"
                   "         size=\"0.3,0.3,0.3\"\n"
                   "         movable=\"true\"\n"
                   "         mass=\"2.5\" \n"
                   "      <leds>\n"
                   "        <led position=\"0.15,0.15,0.15\" color=\"white\" />\n"
                   "        <led position=\"-0.15,0.15,0\" color=\"red\" />\n"
                   "        <led position=\"0.15,0.15,0\" color=\"blue\" />\n"
                   "        <led position=\"0.15,-0.15,0\" color=\"green\" />\n"
                   "      </leds>\n"
                   "    </box>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "In the example, four LEDs are added to the box. The LEDs have\n"
                   "different colors and are located one on the top and three\n"
                   "around the box.\n",
                   "Usable"
      );

}
