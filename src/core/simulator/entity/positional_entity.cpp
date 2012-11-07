#include "positional_entity.h"
#include "composable_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/string_utilities.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CPositionalEntity::Init(TConfigurationNode& t_tree)
   {
      try {
         /* Initialize base entity */
         CEntity::Init(t_tree);
         /* Get the position of the entity */
         CVector3 cPosition;
         GetNodeAttribute(t_tree, "position", cPosition);
         /* Get the orientation of the entity */
         std::string strOrientationEuler;
         GetNodeAttribute(t_tree, "orientation", strOrientationEuler);
         CDegrees pcOrientationEuler[3];
         ParseValues(strOrientationEuler, 3, pcOrientationEuler, ',');
         /* If this entity is has no parent, the given position is absolute
          * otherwise is relative to the parent
          */
         if(HasParent()) {
            /* Initialize offset from XML attribute data */
            m_cInitOffsetPosition = cPosition;
            m_cOffsetPosition = m_cInitOffsetPosition;
            m_cInitOffsetOrientation.FromEulerAngles(ToRadians(pcOrientationEuler[0]),
                                                     ToRadians(pcOrientationEuler[1]),
                                                     ToRadians(pcOrientationEuler[2]));
            m_cOffsetOrientation = m_cInitOffsetOrientation;
            /* Get parent's positional component */
            CPositionalEntity* pcParentPosEntity;
            CComposableEntity* pcComposableParent = &dynamic_cast<CComposableEntity&>(GetParent());
            if(pcComposableParent != NULL) {
               if(pcComposableParent->HasComponent("positional_entity")) {
                  pcParentPosEntity = &dynamic_cast<CPositionalEntity&>(pcComposableParent->GetComponent("positional_entity"));
               }
               else if(pcComposableParent->HasComponent("embodied_entity")) {
                  pcParentPosEntity = &dynamic_cast<CPositionalEntity&>(pcComposableParent->GetComponent("embodied_entity"));
               }
               else {
                  THROW_ARGOSEXCEPTION("Parent entity of \"" << GetId() << "\" does not have a positional/embodied component.");
               }
            }
            /* Initialize position and orientation */
            m_cInitPosition = pcParentPosEntity->GetPosition();
            m_cInitPosition += m_cOffsetPosition;
            m_cPosition = m_cInitPosition;
            m_cInitOrientation = pcParentPosEntity->GetOrientation();
            m_cInitOrientation *= m_cInitOffsetOrientation;
            m_cOrientation *= m_cInitOrientation;
         }
         else {
            m_cInitPosition = m_cPosition;
            m_cOrientation.FromEulerAngles(ToRadians(pcOrientationEuler[0]),
                                           ToRadians(pcOrientationEuler[1]),
                                           ToRadians(pcOrientationEuler[2]));
            m_cInitOrientation = m_cOrientation;
         }
         
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CPositionalEntity::Reset() {
      /* Reset the entity's initial position and orientation */
      SetPosition(m_cInitPosition);
      SetOrientation(m_cInitOrientation);
      SetOffsetPosition(m_cInitOffsetPosition);
      SetOffsetOrientation(m_cInitOffsetOrientation);
   }

   /****************************************/
   /****************************************/

   bool CPositionalEntity::MoveTo(const CVector3& c_position,
                                  const CQuaternion& c_orientation,
                                  bool b_check_only) {
      if(! b_check_only) {
         SetPosition(c_position);
         SetOrientation(c_orientation);
      }
      return true;
   }

   /****************************************/
   /****************************************/

}
