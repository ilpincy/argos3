/**
 * @file <argos3/plugins/simulator/entities/gripper_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "gripper_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {
   
   /****************************************/
   /****************************************/

   CRange<Real> UNIT(0.0f, 1.0f);

   /****************************************/
   /****************************************/

   CGripperEquippedEntity::CGripperEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent),
      m_fLockState(0.0f),
      m_fLockThreshold(0.5f),
      m_pcGrippedEntity(NULL) {
      Disable();
   }
         
   /****************************************/
   /****************************************/
         
   CGripperEquippedEntity::CGripperEquippedEntity(CComposableEntity* pc_parent,
                                                  const std::string& str_id,
                                                  const CVector3& c_offset,
                                                  const CVector3& c_direction,
                                                  Real f_lock_threshold) :
      CEntity(pc_parent,
              str_id),
      m_cOffset(c_offset),
      m_cInitOffset(c_offset),
      m_cDirection(c_direction),
      m_cInitDirection(c_direction),
      m_fLockState(0.0f),
      m_fLockThreshold(f_lock_threshold),
      m_pcGrippedEntity(NULL) {
      Disable();
   }
         
   /****************************************/
   /****************************************/

   void CGripperEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Parse id */
         CEntity::Init(t_tree);
         /* Parse gripper attributes */
         GetNodeAttribute(t_tree, "offset", m_cOffset);
         m_cInitOffset = m_cOffset;
         GetNodeAttribute(t_tree, "direction", m_cDirection);
         m_cInitDirection = m_cDirection;
         GetNodeAttributeOrDefault(t_tree, "lock_threshold", m_fLockThreshold, m_fLockThreshold);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while initializing the gripper entity \"" << GetId() << "\"", ex);
      }
   }
         
   /****************************************/
   /****************************************/

   void CGripperEquippedEntity::Reset() {
      m_fLockState = 0.0f;
      m_cOffset = m_cInitOffset;
      m_cDirection = m_cInitDirection;
      ClearGrippedEntity();
   }

   /****************************************/
   /****************************************/
         
   void CGripperEquippedEntity::SetLockState(Real f_lock_state) {
      UNIT.TruncValue(f_lock_state);
      m_fLockState = f_lock_state;
   }

   /****************************************/
   /****************************************/
         
   void CGripperEquippedEntity::SetLockThreshold(Real f_lock_threshold) {
      UNIT.TruncValue(f_lock_threshold);
      m_fLockThreshold = f_lock_threshold;
   }

   /****************************************/
   /****************************************/
         
   CEmbodiedEntity& CGripperEquippedEntity::GetGrippedEntity() {
      if(m_pcGrippedEntity != NULL) {
         return *m_pcGrippedEntity;
      }
      else {
         THROW_ARGOSEXCEPTION("Entity \"" << GetId() << "\" is not gripping anything.");
      }
   }

   /****************************************/
   /****************************************/
         
   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CGripperEquippedEntity);

   /****************************************/
   /****************************************/

}
