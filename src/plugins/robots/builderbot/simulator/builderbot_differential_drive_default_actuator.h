/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_differential_drive_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_DIFFERENTIAL_DRIVE_DEFAULT_ACTUATOR_H
#define BUILDERBOT_DIFFERENTIAL_DRIVE_DEFAULT_ACTUATOR_H

namespace argos {
   class CBuilderBotDifferentialDriveDefaultActuator;
   class CBuilderBotDifferentialDriveEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_differential_drive_actuator.h>

namespace argos {

   class CBuilderBotDifferentialDriveDefaultActuator : public CSimulatedActuator,
                                                       public CCI_BuilderBotDifferentialDriveActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotDifferentialDriveDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotDifferentialDriveDefaultActuator();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void SetTargetVelocityLeft(Real f_target_velocity_left);

      virtual void SetTargetVelocityRight(Real f_target_velocity_right);

   private:

      /*
      SInt16 ConvertToRaw(Real f_metres_per_second) {
         static const Real fConversionFactor = 1.0;
         return std::round(f_metres_per_second * fConversionFactor);
      }
      */

      CBuilderBotDifferentialDriveEntity* m_pcDifferentialDriveEntity;

      Real m_fTargetVelocityLeft;
      Real m_fTargetVelocityRight;


   };
}

#endif
