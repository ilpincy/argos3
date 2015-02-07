
/**
 * @file <argos3/core/simulator/entity/positional_entity.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "positional_entity.h"
#include "composable_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/string_utilities.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPositionalEntity::CPositionalEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent) {}

   /****************************************/
   /****************************************/

   CPositionalEntity::CPositionalEntity(CComposableEntity* pc_parent,
                                        const std::string& str_id,
                                        const CVector3& c_position,
                                        const CQuaternion& c_orientation) :
      CEntity(pc_parent, str_id),
      m_cPosition(c_position),
      m_cInitPosition(c_position),
      m_cOrientation(c_orientation),
      m_cInitOrientation(c_orientation) {}

   /****************************************/
   /****************************************/

   void CPositionalEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Initialize base entity */
         CEntity::Init(t_tree);
         /* Get the position of the entity */
         GetNodeAttributeOrDefault(t_tree, "position", m_cPosition, CVector3());
         /* Get the orientation of the entity */
         GetNodeAttributeOrDefault(t_tree, "orientation", m_cOrientation, CQuaternion());
         m_cInitPosition = m_cPosition;
         m_cInitOrientation = m_cOrientation;
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize positional entity \"" << GetId() << "\".", ex);
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

   void CPositionalEntity::MoveTo(const CVector3& c_position,
                                  const CQuaternion& c_orientation) {
      SetPosition(c_position);
      SetOrientation(c_orientation);
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CPositionalEntity);

   /****************************************/
   /****************************************/

}
