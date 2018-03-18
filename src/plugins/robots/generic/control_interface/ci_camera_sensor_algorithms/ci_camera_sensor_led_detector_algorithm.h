/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_camera_sensor_algorithms/ci_camera_sensor_led_detector_algorithm.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CI_CAMERAS_SENSOR_LED_DETECTOR_ALGORITHM_H
#define CI_CAMERAS_SENSOR_LED_DETECTOR_ALGORITHM_H

namespace argos {
	class CCI_CameraSensorLEDDetectorAlgorithm;
}

#include <argos3/plugins/robots/generic/control_interface/ci_camera_sensor_algorithm.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/datatypes/datatypes.h>

#ifdef ARGOS_WITH_LUA
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#endif


namespace argos {
   
   class CCI_CameraSensorLEDDetectorAlgorithm : virtual public CCI_CameraSensorAlgorithm {
      
   public:

      struct SReading {
         /* Color */
         CColor Color;
         /* Coordinates in image */
         CVector2 Center;
         /**
          * Constructor
          * @param c_color Observation color
          * @param c_center Image coordinates of the observation
          */
         SReading(const CColor& c_color,
                  const CVector2& c_center) :
            Color(c_color),
            Center(c_center) {}
      };

   public:
      
      /**
       * Constructor
       */
      CCI_CameraSensorLEDDetectorAlgorithm() {}
      
      /**
       * Destructor
       */
      virtual ~CCI_CameraSensorLEDDetectorAlgorithm() {}
      
      const std::vector<SReading>& GetReadings() const {
         return m_vecReadings;
      }
      
#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
      
      virtual void ReadingsToLuaState(lua_State* pt_lua_state);

      virtual const std::string& GetId() {
         static std::string strId("led_detector");
         return strId;
      }
#endif

   protected:

      std::vector<SReading> m_vecReadings;
      
   };
   
}

#endif
