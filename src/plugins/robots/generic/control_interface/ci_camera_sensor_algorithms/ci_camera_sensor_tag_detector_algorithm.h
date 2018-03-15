/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_camera_sensor_algorithms/ci_camera_sensor_tag_detector_algorithm.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CI_CAMERAS_SENSOR_TAG_DETECTOR_ALGORITHM_H
#define CI_CAMERAS_SENSOR_TAG_DETECTOR_ALGORITHM_H

namespace argos {
	class CCI_CameraSensorTagDetectorAlgorithm;
}

#include <argos3/plugins/robots/generic/control_interface/ci_camera_sensor_algorithm.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <array>

#ifdef ARGOS_WITH_LUA
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#endif


namespace argos {
   
   class CCI_CameraSensorTagDetectorAlgorithm : virtual public CCI_CameraSensorAlgorithm {
      
   public:

      struct SReading {
         /* Payload of the tag */
         std::string Payload;
         /* Coordinates of the center in image */
         CVector2 Center;
         /* Coordinates of the corners in image */
         std::array<CVector2, 4> Corners;
         /**
          * Constructor
          * @param str_payload Tag payload
          * @param c_center Center pixel of the tag
          * @param c_corners Corner pixels of the tag
          */
         SReading(const std::string& str_payload,
                  const CVector2& c_center,
                  const std::array<CVector2, 4>& c_corners) :
            Payload(str_payload),
            Center(c_center),
            Corners(c_corners) {}
      };

   public:
      
      /**
       * Constructor
       */
      CCI_CameraSensorTagDetectorAlgorithm() {}
      
      /**
       * Destructor
       */
      virtual ~CCI_CameraSensorTagDetectorAlgorithm() {}
      
      const std::vector<SReading>& GetReadings() const {
         return m_vecReadings;
      }
      
#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
      
      virtual void ReadingsToLuaState(lua_State* pt_lua_state);

      virtual const std::string& GetId() {
         static std::string strId("tag_detector");
         return strId;
      }
#endif

   protected:

      std::vector<SReading> m_vecReadings;
      
   };
   
}

#endif
