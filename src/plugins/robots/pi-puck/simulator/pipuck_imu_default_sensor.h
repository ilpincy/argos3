/**
 * @file <argos3/plugins/robots/pipuck/simulator/pipuck_imu_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_IMU_DEFAULT_SENSOR_H
#define PIPUCK_IMU_DEFAULT_SENSOR_H

namespace argos {
   class CPiPuckImuDefaultSensor;
   class CEmbodiedEntity;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_imu_sensor.h>

namespace argos {

   class CPiPuckImuDefaultSensor : public CSimulatedSensor,
                                   public CCI_PiPuckImuSensor {

   public:

      /**
       * @brief Constructor.
       */
      CPiPuckImuDefaultSensor() : 
         m_pcBody(nullptr) {}

      /**
       * @brief Destructor.
       */
      virtual ~CPiPuckImuDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Update();

   private:

      CEmbodiedEntity* m_pcBody;

      std::array<CVector3, 2> m_arrPrevPositions;
   
   };


}

#endif
