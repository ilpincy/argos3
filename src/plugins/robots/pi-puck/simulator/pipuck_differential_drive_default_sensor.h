/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_differential_drive_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_DIFFERENTIAL_DRIVE_DEFAULT_SENSOR_H
#define PIPUCK_DIFFERENTIAL_DRIVE_DEFAULT_SENSOR_H

namespace argos {
   class CPiPuckDifferentialDriveDefaultSensor;
   class CPiPuckDifferentialDriveEntity;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_differential_drive_sensor.h>

namespace argos {

   class CPiPuckDifferentialDriveDefaultSensor : public CSimulatedSensor,
                                                 public CCI_PiPuckDifferentialDriveSensor {

   public:

      /**
       * @brief Constructor.
       */
      CPiPuckDifferentialDriveDefaultSensor();

      /**
       * @brief Destructor.
       */
      virtual ~CPiPuckDifferentialDriveDefaultSensor();

      void SetRobot(CComposableEntity& c_entity) override;

      void Init(TConfigurationNode& t_tree) override;

      void Update() override;

      void Reset() override;

      Real GetLeftVelocity() override {
         return m_fVelocityLeft;
      }

      Real GetRightVelocity() override {
         return m_fVelocityRight;
      }

   private:
      /*
      Real ConvertToMetersPerSecond(SInt16 n_raw) {
         static const Real fConversionFactor = 1.0;
         return (fConversionFactor * n_raw);
      }
      */

      CPiPuckDifferentialDriveEntity* m_pcDifferentialDriveEntity;

      Real m_fVelocityLeft;
      Real m_fVelocityRight;
   };
}

#endif
