/**
 * @file <argos3/plugins/robots/mini-quadrotor/simulator/miniquadrotor_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef MINIQUADROTOR_ENTITY_H
#define MINIQUADROTOR_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CEmbodiedEntity;
   class CRotorEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/robots/mini-quadrotor/control_interface/ci_miniquadrotor_trajectory_actuator.h>

namespace argos {

   class CMiniQuadrotorEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CMiniQuadrotorEntity();

      CMiniQuadrotorEntity(const std::string& str_id,
                     const std::string& str_controller_id,
                     const CVector3& c_position = CVector3(),
                     const CQuaternion& c_orientation = CQuaternion());
      
      virtual void Init(TConfigurationNode& t_tree);

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }
      
      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CRotorEquippedEntity& GetRotorEquippedEntity() {
         return *m_pcRotorEquippedEntity;
      }
      
      virtual std::string GetTypeDescription() const {
         return "mini-bot";
      }

      inline const CCI_MiniQuadrotorTrajectoryActuator::SWaypoint& GetDesiredWaypoint() const {
         return m_sDesiredWaypoint;
      }

      inline void SetDesiredWaypoint(CCI_MiniQuadrotorTrajectoryActuator::SWaypoint& s_waypoint) {
         m_sDesiredWaypoint = s_waypoint;
      }

   private:

      CControllableEntity*                           m_pcControllableEntity;
      CEmbodiedEntity*                               m_pcEmbodiedEntity;
      CRotorEquippedEntity*                          m_pcRotorEquippedEntity;
      CCI_MiniQuadrotorTrajectoryActuator::SWaypoint m_sDesiredWaypoint;
   };

}

#endif
