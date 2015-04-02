/**
 * @file <argos3/plugins/simulator/actuators/range_and_bearing_default_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef RANGE_AND_BEARING_DEFAULT_ACTUATOR_H
#define RANGE_AND_BEARING_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CRangeAndBearingDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>

namespace argos {

   class CRangeAndBearingDefaultActuator : public CSimulatedActuator,
                                           public CCI_RangeAndBearingActuator {

   public:

      CRangeAndBearingDefaultActuator() {}
      virtual ~CRangeAndBearingDefaultActuator() {}
      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Update();
      virtual void Reset();

   private:

      CRABEquippedEntity* m_pcRangeAndBearingEquippedEntity;

   };

}

#endif
