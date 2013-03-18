/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_distance_scanner_default_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_DISTANCE_SCANNER_DEFAULT_ACTUATOR_H
#define FOOTBOT_DISTANCE_SCANNER_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CFootBotDistanceScannerDefaultActuator;
}

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_distance_scanner_actuator.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_distance_scanner_equipped_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos {

   class CFootBotDistanceScannerDefaultActuator : public CSimulatedActuator,
                                                  public CCI_FootBotDistanceScannerActuator {

   public:

  	  static const Real RPM_TO_RADIANS_PER_SEC;

      CFootBotDistanceScannerDefaultActuator();
      virtual ~CFootBotDistanceScannerDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void SetAngle(const CRadians& c_angle);
      virtual void SetRPM(Real f_rpm);

      virtual void Enable();
      virtual void Disable();

      virtual void Update();
      virtual void Reset();

   private:

      CFootBotDistanceScannerEquippedEntity* m_pcDistanceScannerEquippedEntity;

      CRadians m_cDesiredRotation;
      Real     m_fDesiredRotationSpeed;
      UInt8    m_unDesiredMode;

   };

}

#endif
