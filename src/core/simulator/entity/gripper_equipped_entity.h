/**
 * @file <argos3/core/simulator/entity/gripper_equipped_entity.h>
 *
 * This file defines the class CGripperEquippedEntity. This class provides a
 * component that gripper-equipped robots can use manage for gripping other
 * objects.
 *
 * Internally, the class stores the gripper aperture in two ways. There is
 * a "logic" state, which says whether the gripper is locked or unlocked,
 * mainly used internally to manage object connection and disconnection.
 * In addition, there is a "native" state, which corresponds to the actual
 * value stored by the actuator and retrievable by the sensor when needed.
 * It is stored as a real.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef GRIPPER_EQUIPPED_ENTITY_H
#define GRIPPER_EQUIPPED_ENTITY_H

namespace argos {
   class CGripperEquippedEntity;
   class CEmbodiedEntity;
}

#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CGripperEquippedEntity : public CPositionalEntity {

   public:

      ENABLE_VTABLE();

   public:

      CGripperEquippedEntity(CComposableEntity* pc_parent);

      CGripperEquippedEntity(CComposableEntity* pc_parent,
                             const std::string& str_id,
                             const CVector3& c_position,
                             const CQuaternion& c_orientation);

      virtual void Reset();

      inline Real GetLogicLockState() const {
         return m_fLogicLockState;
      }

      inline void SetLogicLockState(Real f_logic_lock_state) {
         m_fLogicLockState = f_logic_lock_state;
      }

      inline Real GetNativeLockState() const {
         return m_fNativeLockState;
      }

      inline void SetNativeLockState(Real f_native_lock_state) {
         m_fNativeLockState = f_native_lock_state;
      }

      inline bool IsLocked() const {
         return m_fLogicLockState > 0.5f;
      }

      inline bool IsUnlocked() const {
         return !IsLocked();
      }

      inline CEmbodiedEntity& GetGrippedEntity() {
         return *m_pcGrippedEntity;
      }

      inline void SetGrippedEntity(CEmbodiedEntity& c_entity) {
         m_pcGrippedEntity = &c_entity;
      }

      inline void ClearGrippedEntity() {
         m_pcGrippedEntity = NULL;
      }

      inline bool IsGripping() const {
         return (m_pcGrippedEntity != NULL);
      }

      virtual std::string GetTypeDescription() const {
         return "gripper";
      }

   private:

      Real m_fLogicLockState;
      Real m_fNativeLockState;
      CEmbodiedEntity* m_pcGrippedEntity;
      
   };
}

#endif
