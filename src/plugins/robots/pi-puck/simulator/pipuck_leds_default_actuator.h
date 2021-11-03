/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_leds_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_LEDS_DEFAULT_ACTUATOR_H
#define PIPUCK_LEDS_DEFAULT_ACTUATOR_H

namespace argos {
   class CPiPuckLEDsDefaultActuator;
   class CDirectionalLEDEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_leds_actuator.h>

#include <array>

namespace argos {

   class CPiPuckLEDsDefaultActuator : public CSimulatedActuator,
                                      public CCI_PiPuckLEDsActuator {

   public:

      /**
       * @brief Constructor.
       */
      CPiPuckLEDsDefaultActuator() {}

      /**
       * @brief Destructor.
       */
      virtual ~CPiPuckLEDsDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void SetRingLEDIndex(UInt32 un_index, bool b_on);

      virtual void SetRingLEDs(bool b_on);

      virtual void SetFrontLED(bool b_on);

      virtual void SetBodyLED(bool b_on);

   private:

      std::array<std::pair<CDirectionalLEDEntity*, bool>, 
                 CCI_PiPuckLEDsActuator::NUM_LEDS> m_arrLEDs;

   };
}

#endif
