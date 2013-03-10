/**
 * @file <argos3/plugins/simulator/entities/gripper_equipped_entity.h>
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
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   /**
    * An entity that stores the state of a robot gripper.
    * <p>
    * This class provides a component that stores the state of a robot gripper.
    * The state is represented by a Real value between 0 and 1, where 0
    * means unlocked and 1 means locked. In-between values are also allowed. The
    * transition between locked and unlocked state is when the lock
    * state exceeds m_fLockThreshold, which is set to 0.5 by default.
    * </p>
    * <p>
    * The gripper can also be oriented, acting on its offset and direction
    * attributes. The offset refers to the position of the gripper with respect
    * to the reference point of the robot. The direction refers to where the gripper
    * is facing.
    * </p>
    */
   class CGripperEquippedEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      /**
       * Class constructor.
       * This constructor is meant to be used with the Init() method.
       * @param pc_parent The parent of this entity.
       */
      CGripperEquippedEntity(CComposableEntity* pc_parent);

      /**
       * Class constructor.
       * This constructor is meant to be standalone.
       * You should not call Init() after using this constructor, or
       * memory leaks are likely to happen.
       * @param pc_parent The parent of this entity.
       * @param str_id The id of this entity.
       * @param c_offset The positional offset of this gripper with respect to the robot reference point.
       * @param c_direction The facing direction of this gripper.
       * @param f_lock_threshold The lock value above which the gripper is considered locked.
       */
      CGripperEquippedEntity(CComposableEntity* pc_parent,
                             const std::string& str_id,
                             const CVector3& c_offset,
                             const CVector3& c_direction,
                             Real f_lock_threshold = 0.5f);

      /**
       * Initializes the state of the entity from the XML configuration tree.
       * @throws CARGoSException if a parse error occurs
       */
      virtual void Init(TConfigurationNode& t_tree);

      /**
       * Resets the state of the entity to whatever it was after Init() or one of the standalone constructors was called.
       */
      virtual void Reset();

      /**
       * Returns the offset of the gripper with respect to the reference point.
       * @return The offset of the gripper with respect to the reference point.
       */
      inline const CVector3& GetOffset() const {
         return m_cOffset;
      }

      /**
       * Returns the initial offset of the gripper with respect to the reference point.
       * @return The initial offset of the gripper with respect to the reference point.
       * @see Reset()
       */
      inline const CVector3& GetInitOffset() const {
         return m_cInitOffset;
      }

      /**
       * Sets the offset of the gripper with respect to the reference point.
       * @param c_offset The offset of the gripper with respect to the reference point.
       */
      inline void SetOffset(const CVector3& c_offset) {
         m_cOffset = c_offset;
      }

      /**
       * Sets the initial offset of the gripper with respect to the reference point.
       * @param c_offset The initial offset of the gripper with respect to the reference point.
       * @see Reset()
       */
      inline void SetInitOffset(const CVector3& c_offset) {
         m_cInitOffset = c_offset;
      }

      /**
       * Returns the facing direction of the gripper with respect to the local X-axis
       * @return The facing direction of the gripper with respect to the local X-axis
       */
      inline const CVector3& GetDirection() const {
         return m_cDirection;
      }

      /**
       * Returns the initial facing direction of the gripper with respect to the local X-axis
       * @return The initial facing direction of the gripper with respect to the local X-axis
       * @see Reset()
       */
      inline const CVector3& GetInitDirection() const {
         return m_cInitDirection;
      }

      /**
       * Sets the facing direction of the gripper with respect to the local X-axis
       * @param c_direction The facing direction of the gripper with respect to the local X-axis
       */
      inline void SetDirection(const CVector3& c_direction) {
         m_cDirection = c_direction;
      }

      /**
       * Sets the initial facing direction of the gripper with respect to the local X-axis
       * @param c_direction The initial facing direction of the gripper with respect to the local X-axis
       * @see Reset()
       */
      inline void SetInitDirection(const CVector3& c_direction) {
         m_cInitDirection = c_direction;
      }

      /**
       * Returns the lock state of the gripper.
       * The lock state is a Real in the range [0:1].
       * @return The lock state of the gripper.
       */
      inline Real GetLockState() const {
         return m_fLockState;
      }

      /**
       * Sets the lock state of the gripper.
       * The lock state is a Real in the range [0:1]. The passed value is clamped internally.
       * @return The lock state of the gripper.
       */
      void SetLockState(Real f_lock_state);

      /**
       * Returns the lock threshold of the gripper.
       * The lock threshold is a Real in the range [0:1].
       * @return The lock threshold of the gripper.
       */
      inline Real GetLockThreshold() const {
         return m_fLockThreshold;
      }

      /**
       * Sets the lock threshold of the gripper.
       * The lock threshold is a Real in the range [0:1]. The passed value is clamped internally.
       * @return The lock threshold of the gripper.
       */
      void SetLockThreshold(Real f_lock_threshold);

      /**
       * Returns <tt>true</tt> if the gripper is locked.
       * The gripper is considered locked if its state is greater than the lock threshold.
       * @return <tt>true</tt> if the gripper is locked.
       * @see GetLockState()
       * @see GetLockThreshold()
       */
      inline bool IsLocked() const {
         return m_fLockState > m_fLockThreshold;
      }

      /**
       * Returns <tt>true</tt> if this gripper is gripping something.
       * @return <tt>true</tt> if this gripper is gripping something.
       */
      inline bool IsGripping() const {
         return (m_pcGrippedEntity != NULL);
      }

      /**
       * Returns the embodied entity currently gripped by this gripper.
       * @return The embodied entity currently gripped by this gripper.
       * @throws CARGoSException if no entity is being gripped.
       */
      CEmbodiedEntity& GetGrippedEntity();

      /**
       * Sets the embodied entity currently gripped by this gripper.
       * @param c_entity The embodied entity currently gripped by this gripper.
       */
      inline void SetGrippedEntity(CEmbodiedEntity& c_entity) {
         m_pcGrippedEntity = &c_entity;
      }

      /**
       * Clears the reference to the embodied entity currently gripped by this gripper.
       */
      inline void ClearGrippedEntity() {
         m_pcGrippedEntity = NULL;
      }

      virtual std::string GetTypeDescription() const {
         return "gripper";
      }

   private:

      /** The positional offset of this gripper with respect to the robot reference point */
      CVector3 m_cOffset;

      /** The initial positional offset of this gripper with respect to the robot reference point */
      CVector3 m_cInitOffset;

      /** The facing direction of this gripper with respect to the robot's X-axis  */
      CVector3 m_cDirection;

      /** The initial facing direction of this gripper with respect to the robot's X-axis  */
      CVector3 m_cInitDirection;

      /** The current lock state of the gripper */
      Real m_fLockState;

      /** The value above which the gripper is considered locked */
      Real m_fLockThreshold;

      /** A pointer to the embodied entity being currently gripped */
      CEmbodiedEntity* m_pcGrippedEntity;
      
   };
}

#endif
