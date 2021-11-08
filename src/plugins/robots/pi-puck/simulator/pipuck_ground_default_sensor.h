/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_ground_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_GROUND_DEFAULT_SENSOR_H
#define PIPUCK_GROUND_DEFAULT_SENSOR_H

namespace argos {
   class CEmbodiedEntity;
   class CFloorEntity;
}

#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_ground_sensor.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CPiPuckGroundDefaultSensor : public CSimulatedSensor,
                                      public CCI_PiPuckGroundSensor {

   public:

      struct SSimulatedInterface : SInterface {
         /* constructor */
         SSimulatedInterface(UInt8 un_label,
                             const SAnchor& s_anchor) :
            SInterface(un_label),
            Anchor(s_anchor) {}
         /* members */
         const SAnchor& Anchor;
      };

   public:

      CPiPuckGroundDefaultSensor();

      virtual ~CPiPuckGroundDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void Visit(std::function<void(const SInterface&)> fn_visitor);

   private:

      bool m_bShowRays;
      CControllableEntity* m_pcControllableEntity;
      CFloorEntity& m_cFloorEntity;
      std::vector<SSimulatedInterface> m_vecSimulatedInterfaces;
      static const CPlane m_cFloor;

   };

}

#endif
