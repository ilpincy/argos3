/**
 * @file <argos3/plugins/robots/prototype/control_interface/ci_prototype_joints_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_PROTOTYPE_JOINTS_ACTUATOR_H
#define CCI_PROTOTYPE_JOINTS_ACTUATOR_H

namespace argos {
   class CCI_PrototypeJointsActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/math/angles.h>

namespace argos {

   class CCI_PrototypeJointsActuator : virtual public CCI_Actuator {

   public:
      struct SActuator {
         using TVector = std::vector<SActuator*>;
         SActuator(const std::string& str_id, Real f_target) :
            Id(str_id),
            Target(f_target) {}
         std::string Id;
         Real Target;
      };

   public:
      /**
       * Destructor.
       */
      virtual ~CCI_PrototypeJointsActuator() {}

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State*);
#endif

   protected: 
      SActuator::TVector m_vecActuators;
   };
}

#endif
