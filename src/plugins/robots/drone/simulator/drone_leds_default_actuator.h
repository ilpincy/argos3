/**
 * @file <argos3/plugins/robots/drone/simulator/drone_leds_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_LEDS_DEFAULT_ACTUATOR_H
#define DRONE_LEDS_DEFAULT_ACTUATOR_H

namespace argos {
   class CDroneLEDsDefaultActuator;
   class CDirectionalLEDEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/plugins/robots/drone/control_interface/ci_drone_leds_actuator.h>

#include <array>

namespace argos {

   class CDroneLEDsDefaultActuator : public CSimulatedActuator,
                                     public CCI_DroneLEDsActuator {

   public:

      /**
       * @brief Constructor.
       */
      CDroneLEDsDefaultActuator() {}

      /**
       * @brief Destructor.
       */
      virtual ~CDroneLEDsDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void SetLEDIndex(UInt32 un_index, const CColor& c_color);

      virtual void SetLEDs(const CColor& c_color);

   private:

      std::array<std::pair<CDirectionalLEDEntity*, CColor>, 
         CCI_DroneLEDsActuator::NUM_LEDS> m_arrLEDs;

   };
}

#endif
