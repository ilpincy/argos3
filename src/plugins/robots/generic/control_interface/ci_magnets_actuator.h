/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_magnets_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CI_MAGNETS_ACTUATOR_H
#define CI_MAGNETS_ACTUATOR_H

namespace argos {
   class CCI_MagnetsActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_MagnetsActuator : virtual public CCI_Actuator {

   public:

      struct SInterface {
         SInterface(const std::string& str_id,
                    Real f_current = 0.0f) :
            Id(str_id),
            Current(f_current) {}
         std::string Id;
         Real Current;
         using TVector = std::vector<SInterface>;
      };

   public:

      /**
       * Constructor
       */
      CCI_MagnetsActuator() {}
      /**
       * Destructor.
       */
      virtual ~CCI_MagnetsActuator() {}

   public:

      /**
       * Returns a reference to the magnet interfaces.
       * @return A reference to the magnet interfaces.
       */        
      SInterface::TVector& GetInterfaces();

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SInterface::TVector m_vecInterfaces;
         
   };
}

#endif
