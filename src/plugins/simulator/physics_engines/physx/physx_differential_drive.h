#ifndef PHYSX_DIFFERENTIAL_DRIVE_H
#define PHYSX_DIFFERENTIAL_DRIVE_H

namespace argos {
   class CPhysXDifferentialDrive;
   class CPhysXMultiBodyObjectModel;
}

#include <argos3/plugins/simulator/physics_engines/physx/physx_engine.h>

namespace argos {

   /**
    * Creates a collection of actors that act as a simple, two-wheeled vehicle.
    * This class manages three dynamic actors - two wheels and a body located in
    * the middle. You can set the speed of the wheels independently, and attach
    * actors to the body to build robots on top of this component.
    *
    * The wheels are placed along the Y axis. The X axis is directed as
    * the nose of the robot. Following the right-hand rule, and taking
    * the X axis as the thumb and the index finger as the Y axis, the left
    * wheel is located on the positive Y axis, while the right wheel on the
    * negative one.
    */
   class CPhysXDifferentialDrive {

   public:

      /**
       * Class constructor.
       * Creates the differential drive component.
       * @param c_model The multi-body model that contains this component.
       * @param c_physx_engine The PhysX engine that manages this component.
       * @param f_interwheel_distance The distance between the wheel centers.
       * @param f_wheel_radius The radius of the wheels.
       * @param f_wheel_thickness The thickness of the wheels.
       * @param c_body_size The size of the main body.
       */
      CPhysXDifferentialDrive(CPhysXMultiBodyObjectModel& c_model,
                              CPhysXEngine& c_physx_engine,
                              physx::PxReal f_interwheel_distance,
                              physx::PxReal f_wheel_radius,
                              physx::PxReal f_wheel_thickness,
                              physx::PxReal f_wheel_mass,
                              const physx::PxVec3& c_body_size,
                              physx::PxReal f_body_elevation,
                              physx::PxReal f_body_mass);

      ~CPhysXDifferentialDrive();

      /**
       * Sets the global pose of this component.
       * The global pose is calculated with respect to the local origin
       * of this component. The local origin in situated at the center
       * of the base.
       */
      void SetGlobalPose(const physx::PxTransform& c_pose);

      inline void GetCurrentWheelAngles(physx::PxReal& f_left_angle,
                                        physx::PxReal& f_right_angle) const {
         f_left_angle = m_pcLeftWheelJoint->getAngle();
         f_right_angle = m_pcRightWheelJoint->getAngle();
      }

      inline void GetCurrentWheelLinearVelocity(physx::PxReal& f_left_velocity,
                                                physx::PxReal& f_right_velocity) const {
         f_left_velocity = m_pcLeftWheelJoint->getVelocity() * m_fWheelRadius;
         f_right_velocity = m_pcRightWheelJoint->getVelocity() * m_fWheelRadius;
      }

      inline void GetTargetWheelLinearVelocity(physx::PxReal& f_left_velocity,
                                               physx::PxReal& f_right_velocity) const {
         f_left_velocity = m_pcLeftWheelJoint->getDriveVelocity() * m_fWheelRadius;
         f_right_velocity = m_pcRightWheelJoint->getDriveVelocity() * m_fWheelRadius;
      }

      inline void SetTargetWheelLinearVelocity(physx::PxReal f_left_velocity,
                                               physx::PxReal f_right_velocity) {
         m_pcLeftWheelJoint->setDriveVelocity(f_left_velocity / m_fWheelRadius);
         m_pcRightWheelJoint->setDriveVelocity(f_right_velocity / m_fWheelRadius);
      }

      inline physx::PxReal GetWheelRadius() const {
         return m_fWheelRadius;
      }

      inline physx::PxReal GetWheelThickness() const {
         return m_fWheelThickness;
      }

      inline const physx::PxVec3& GetMainBodySize() const {
         return m_cMainBodySize;
      }

      inline const physx::PxTransform& GetMainBodyOffset() const {
         return m_cMainBodyOffset;
      }
 
      inline const physx::PxTransform& GetLeftWheelOffset() const {
         return m_cLeftWheelOffset;
      }
 
      inline const physx::PxTransform& GetRightWheelOffset() const {
         return m_cRightWheelOffset;
      }

      inline physx::PxRigidDynamic& GetMainBodyActor() {
         return *m_pcMainBodyActor;
      }
 
      inline const physx::PxRigidDynamic& GetMainBodyActor() const {
         return *m_pcMainBodyActor;
      }
 
      inline physx::PxRigidDynamic& GetLeftWheelActor() {
         return *m_pcLeftWheelActor;
      }
 
      inline const physx::PxRigidDynamic& GetLeftWheelActor() const {
         return *m_pcLeftWheelActor;
      }
 
      inline physx::PxRigidDynamic& GetRightWheelActor() {
         return *m_pcRightWheelActor;
      }
 
      inline const physx::PxRigidDynamic& GetRightWheelActor() const {
         return *m_pcRightWheelActor;
      }
 
      inline physx::PxShape& GetMainBodyShape() {
         return *m_pcMainBodyShape;
      }
 
      inline const physx::PxShape& GetMainBodyShape() const {
         return *m_pcMainBodyShape;
      }
 
      inline physx::PxShape& GetLeftWheelShape() {
         return *m_pcLeftWheelShape;
      }
 
      inline const physx::PxShape& GetLeftWheelShape() const {
         return *m_pcLeftWheelShape;
      }
 
      inline physx::PxShape& GetRightWheelShape() {
         return *m_pcRightWheelShape;
      }
 
      inline const physx::PxShape& GetRightWheelShape() const {
         return *m_pcRightWheelShape;
      }
 
  private:

      void CreateJoints();
      void DestroyJoints();

  private:

      /** The PhysX engine managing this component */
      CPhysXEngine& m_cPhysXEngine;
      /** The radius of the wheels */
      physx::PxReal m_fWheelRadius;
      /** The thickness of the wheels */
      physx::PxReal m_fWheelThickness;
      /** Main body size */
      physx::PxVec3 m_cMainBodySize;
      /** The offset of the main body */
      physx::PxTransform m_cMainBodyOffset;
      /** The offset of the left wheel */
      physx::PxTransform m_cLeftWheelOffset;
      /** The offset of the right wheel */
      physx::PxTransform m_cRightWheelOffset;
      /** The main body actor */
      physx::PxRigidDynamic* m_pcMainBodyActor;
      /** The left wheel actor */
      physx::PxRigidDynamic* m_pcLeftWheelActor;
      /** The right wheel actor */
      physx::PxRigidDynamic* m_pcRightWheelActor;
      /** The main body shape */
      physx::PxShape* m_pcMainBodyShape;
      /** The left wheel shape */
      physx::PxShape* m_pcLeftWheelShape;
      /** The right wheel shape */
      physx::PxShape* m_pcRightWheelShape;
      /** The joint for the left wheel */
      physx::PxRevoluteJoint* m_pcLeftWheelJoint;
      /** The joint for the right wheel */
      physx::PxRevoluteJoint* m_pcRightWheelJoint;
      /** The distance between the body center and the joints center */
      physx::PxReal m_fBodyJointDistance;
      /** The distance between the wheel center and its joint center */
      physx::PxReal m_fWheelJointDistance;
   };

}

#endif
