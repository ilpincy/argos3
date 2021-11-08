/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_rangefinders_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_RANGEFINDERS_DEFAULT_SENSOR_H
#define PIPUCK_RANGEFINDERS_DEFAULT_SENSOR_H

namespace argos {
   class CPiPuckRangefindersDefaultSensor;
   class CControllableEntity;
   class CEmbodiedEntity;
   struct SAnchor;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_rangefinders_sensor.h>

namespace argos {

   class CPiPuckRangefindersDefaultSensor : public CSimulatedSensor,
                                            public CCI_PiPuckRangefindersSensor {

   public:

      struct SSimulatedInterface : SInterface {
         /* constructor */
         SSimulatedInterface(const UInt8& un_label,
                             const SAnchor& s_anchor) :
            SInterface(un_label),
            Anchor(s_anchor) {}
         /* members */
         const SAnchor& Anchor;
      };

      CPiPuckRangefindersDefaultSensor():
         m_bShowRays(false),
         m_pcControllableEntity(nullptr) {}

      virtual ~CPiPuckRangefindersDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void Visit(std::function<void(const SInterface&)>);

   private:

      Real ConvertToMeters(Real f_raw) {
         static const Real fConversionFactor = Real(1.0);
         return (f_raw * fConversionFactor);
      }

      Real ConvertToLux(Real f_raw) {
         static const Real fConversionFactor = Real(1.0);
         return (f_raw * fConversionFactor);
      }

   private:

      bool m_bShowRays;
      CControllableEntity* m_pcControllableEntity;
      std::vector<SSimulatedInterface> m_vecSimulatedInterfaces;

   };
}

#endif
