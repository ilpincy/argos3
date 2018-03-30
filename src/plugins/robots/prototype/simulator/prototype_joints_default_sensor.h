/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_joints_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PROTOTYPE_JOINTS_DEFAULT_SENSOR_H
#define PROTOTYPE_JOINTS_DEFAULT_SENSOR_H

namespace argos {
   class CPrototypeJointsDefaultSensor;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/prototype/control_interface/ci_prototype_joints_sensor.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_entity.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_joint_equipped_entity.h>

namespace argos {

   class CPrototypeJointsDefaultSensor : public CSimulatedSensor,
                                         public CCI_PrototypeJointsSensor {

   public:
      struct SSimulatedSensor : public SSensor {
         SSimulatedSensor(const std::string& str_id,
                          CPrototypeJointEntity::SSensor& s_sensor) :
            SSensor(str_id),
            Instance(s_sensor) {}
            CPrototypeJointEntity::SSensor& Instance;
      };
      
   public:
      CPrototypeJointsDefaultSensor();

      virtual ~CPrototypeJointsDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      std::vector<SSimulatedSensor> m_vecSimulatedSensors;

      CPrototypeJointEquippedEntity* m_pcJointEquippedEntity;

   };
}

#endif
