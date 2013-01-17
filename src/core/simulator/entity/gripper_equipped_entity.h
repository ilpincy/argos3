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

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {
   class CGripperEquippedEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      CGripperEquippedEntity(CEntity* pc_parent) :
         CEntity(pc_parent),
         m_fLogicLockState(0.0f),
         m_fNativeLockState(0.0f),
         m_pcGrippedEntity(NULL) {}

      virtual ~CGripperEquippedEntity() {}

      virtual void Reset() {
         m_fLogicLockState = 0.0f;
      }

      virtual void Update() {}

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

      inline virtual bool IsLocked() const {
         return m_fLogicLockState > 0.5f;
      }

      inline virtual bool IsUnlocked() const {
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

      inline const CVector3& GetPosition() const {
         return m_cPosition;
      }

      inline void SetPosition(const CVector3& c_position) {
         m_cPosition = c_position;
      }

      inline const CVector3& GetDirection() const {
         return m_cDirection;
      }

      inline void SetDirection(const CVector3& c_direction) {
         m_cDirection = c_direction;
      }

      inline virtual std::string GetTypeDescription() const {
         return "gripper_equipped_entity";
      }

   private:

      Real m_fLogicLockState;
      Real m_fNativeLockState;
      CEmbodiedEntity* m_pcGrippedEntity;
      CVector3 m_cPosition;
      CVector3 m_cDirection;
      
   };
}

#endif
