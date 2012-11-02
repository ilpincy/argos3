#include "positional_entity.h"
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
         GetNodeAttribute(t_tree, "position", m_cPosition);
         m_cInitPosition = m_cPosition;
         /* Get the orientation of the entity */
         std::string strOrientationEuler;
         GetNodeAttribute(t_tree, "orientation", strOrientationEuler);
         CDegrees pcOrientationEuler[3];
         ParseValues(strOrientationEuler, 3, pcOrientationEuler, ',');
         m_cOrientation.FromEulerAngles(ToRadians(pcOrientationEuler[0]),
                                        ToRadians(pcOrientationEuler[1]),
                                        ToRadians(pcOrientationEuler[2]));
         m_cInitOrientation = m_cOrientation;
         
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
