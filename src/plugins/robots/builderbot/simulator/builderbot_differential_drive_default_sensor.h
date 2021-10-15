/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_differential_drive_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_DIFFERENTIAL_DRIVE_DEFAULT_SENSOR_H
#define BUILDERBOT_DIFFERENTIAL_DRIVE_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotDifferentialDriveDefaultSensor;
   class CBuilderBotDifferentialDriveEntity;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_differential_drive_sensor.h>

namespace argos {

   class CBuilderBotDifferentialDriveDefaultSensor : public CSimulatedSensor,
                                                     public CCI_BuilderBotDifferentialDriveSensor {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotDifferentialDriveDefaultSensor();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotDifferentialDriveDefaultSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual Real GetLeftVelocity() {
         return m_fVelocityLeft;
      }

      virtual Real GetRightVelocity() {
         return m_fVelocityRight;
      }

   private:
      /*
      Real ConvertToMetersPerSecond(SInt16 n_raw) {
         static const Real fConversionFactor = 1.0;
         return (fConversionFactor * n_raw);
      }
      */

      CBuilderBotDifferentialDriveEntity* m_pcDifferentialDriveEntity;

      Real m_fVelocityLeft;
      Real m_fVelocityRight;
   };
}

#endif
