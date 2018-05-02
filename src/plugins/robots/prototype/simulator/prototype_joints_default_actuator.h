/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_joints_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PROTOTYPE_JOINTS_DEFAULT_ACTUATOR_H
#define PROTOTYPE_JOINTS_DEFAULT_ACTUATOR_H

namespace argos {
   class CPrototypeJointsDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/prototype/control_interface/ci_prototype_joints_actuator.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_entity.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_joint_equipped_entity.h>

namespace argos {

   class CPrototypeJointsDefaultActuator : public CSimulatedActuator,
                                           public CCI_PrototypeJointsActuator {

   public:

      struct SSimulatedActuator : SActuator {
         SSimulatedActuator(const std::string& str_id,
                            Real f_target,
                            CPrototypeJointEntity::SActuator& s_actuator) :
            SActuator(str_id, f_target),
            Instance(s_actuator) {}
         CPrototypeJointEntity::SActuator& Instance;
      };
      
   public:

      CPrototypeJointsDefaultActuator();

      virtual ~CPrototypeJointsDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      std::vector<SSimulatedActuator> m_vecSimulatedActuators;
      
      CPrototypeJointEquippedEntity* m_pcJointEquippedEntity;

   };
}

#endif
