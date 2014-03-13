/**
 * @file <argos3/plugins/simulator/entities/quadrotor_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "quadrotor_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CQuadRotorEntity::CQuadRotorEntity(CComposableEntity* pc_parent,
                                      Real f_arm_length) :
      CEntity(pc_parent),
      m_fArmLength(f_arm_length),
      m_eControlMethod(NO_CONTROL) {
   }

   /****************************************/
   /****************************************/

   CQuadRotorEntity::CQuadRotorEntity(CComposableEntity* pc_parent,
                                      const std::string& str_id,
                                      Real f_arm_length) :
      CEntity(pc_parent, str_id),
      m_fArmLength(f_arm_length),
      m_eControlMethod(NO_CONTROL) {
   }

   /****************************************/
   /****************************************/

   void CQuadRotorEntity::Reset() {
      m_sPositionControlData.Position = CVector3();
      m_sPositionControlData.Yaw = CRadians::ZERO;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CQuadRotorEntity);

   /****************************************/
   /****************************************/

}
