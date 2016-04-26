/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_turret_default_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_TURRET_DEFAULT_ACTUATOR_H
#define FOOTBOT_TURRET_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CFootBotTurretDefaultActuator;
}

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_turret_actuator.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_turret_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos {

   class CFootBotTurretDefaultActuator : public CSimulatedActuator,
                                         public CCI_FootBotTurretActuator {

   public:

      CFootBotTurretDefaultActuator();
      virtual ~CFootBotTurretDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void SetRotation(const CRadians& c_angle);
      virtual void SetRotationSpeed(SInt32 n_speed_pulses);
      virtual void SetMode(ETurretModes e_mode);

      virtual void Update();
      virtual void Reset();

   private:

      CFootBotTurretEntity* m_pcTurretEntity;
      UInt32 m_unDesiredMode;

   };

}

#endif
