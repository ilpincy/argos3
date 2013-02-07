/**
 * @file <argos3/core/simulator/entity/gripper_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "gripper_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {
   
   /****************************************/
   /****************************************/

   CGripperEquippedEntity::CGripperEquippedEntity(CComposableEntity* pc_parent) :
      CPositionalEntity(pc_parent),
      m_fLogicLockState(0.0f),
      m_fNativeLockState(0.0f),
      m_pcGrippedEntity(NULL) {}
         
   /****************************************/
   /****************************************/
         
   CGripperEquippedEntity::CGripperEquippedEntity(CComposableEntity* pc_parent,
                                                  const std::string& str_id,
                                                  const CVector3& c_position,
                                                  const CQuaternion& c_orientation) :
      CPositionalEntity(pc_parent,
                        str_id,
                        c_position,
                        c_orientation),
      m_fLogicLockState(0.0f),
      m_fNativeLockState(0.0f),
      m_pcGrippedEntity(NULL) {}
         
   /****************************************/
   /****************************************/
         
   void CGripperEquippedEntity::Reset() {
      m_fLogicLockState = 0.0f;
      m_fNativeLockState = 0.0f;
      ClearGrippedEntity();
   }

   /****************************************/
   /****************************************/
         
   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CGripperEquippedEntity);

   /****************************************/
   /****************************************/

}
