/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_camera_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CI_CAMERAS_SENSOR_H
#define CI_CAMERAS_SENSOR_H

namespace argos {
	class CCI_CameraSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_camera_sensor_algorithm.h>

namespace argos {

   class CCI_CameraSensor: virtual public CCI_Sensor {

   public:

      struct SInterface {
         SInterface(const std::string& str_id,
                    const std::vector<CCI_CameraSensorAlgorithm*>& vec_algorithms) :
            Id(str_id),
            Algorithms(vec_algorithms) {}
         std::string Id;
         std::vector<CCI_CameraSensorAlgorithm*> Algorithms;
         typedef std::vector<SInterface> TVector;
      };
      
   public:
    
      /**
       * Constructor
       */
      CCI_CameraSensor() {}
      
      /**
       * Destructor
       */
      virtual ~CCI_CameraSensor() {}

   public:

      /**
       * Returns a const reference to the camera interfaces.
       * @return A const reference to the camera interfaces.
       */
      const SInterface::TVector& GetInterfaces() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
      
      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SInterface::TVector m_vecInterfaces;

   };
   
}

#endif
