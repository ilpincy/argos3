/**
 * @file <argos3/plugins/robots/foot-bot/simulator/dynamics2d_footbot_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_FOOTBOT_ENTITY_H
#define DYNAMICS2D_FOOTBOT_ENTITY_H

namespace argos {
   class SDynamics2DEngineGripperData;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_entity.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>

namespace argos {

   class CDynamics2DFootBotEntity : public CDynamics2DEntity {

   public:

      CDynamics2DFootBotEntity(CDynamics2DEngine& c_engine,
                               CFootBotEntity& c_entity);
      virtual ~CDynamics2DFootBotEntity();
      
      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus();
      virtual void UpdateFromEntityStatus();

   private:

      void TurretPassiveToActive();
      void TurretActiveToPassive();

   private:

      CFootBotEntity& m_cFootBotEntity;
      CWheeledEntity& m_cWheeledEntity;
      CGripperEquippedEntity& m_cGripperEntity;

      cpFloat  m_fMass;
      cpShape* m_ptBaseShape;
      cpShape* m_ptGripperShape;
      cpBody*  m_ptActualBaseBody;
      cpBody*  m_ptControlBaseBody;
      cpBody*  m_ptActualGripperBody;
      cpBody*  m_ptControlGripperBody;
      cpConstraint* m_ptBaseControlLinearMotion;
      cpConstraint* m_ptBaseControlAngularMotion;
      cpConstraint* m_ptGripperControlLinearMotion;
      cpConstraint* m_ptGripperControlAngularMotion;
      cpConstraint* m_ptBaseGripperLinearMotion;
      cpConstraint* m_ptBaseGripperAngularMotion;

      SDynamics2DEngineGripperData* m_psGripperData;

      Real m_fCurrentWheelVelocityFromSensor[2];
      UInt8 m_unLastTurretMode;

      Real m_fPreviousTurretAngleError;

   };

}

#endif
