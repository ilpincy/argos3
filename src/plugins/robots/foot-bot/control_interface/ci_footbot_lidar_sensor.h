/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_lidar_sensor.h>
 *
 * @brief This file provides the definition of the footbot lidar sensor.
 *
 * This file provides the definition of the footbot lidar sensor.
 * The sensors are evenly spaced on a ring around the base of the robot.
 * Therefore, they do not turn with the turret. The readings return the
 * distance to an obstacle or the maximum range of the sensor if no
 * obstacle is hit.
 *
 * The readings are in the following order (seeing the robot from TOP,
 * battery socket is the BACK):
 *
 *              front
 *
 *               0 512
 *
 *
 *                               r
 * l                             i
 * e                             g
 * f  128                    384 h
 * t                             t
 *
 *
 *
 *               256
 *
 *              back
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 * @author Andreas Pasternak <andreas.pasternak@gmx.ch>
 */

#ifndef CCI_FOOTBOT_LIDAR_SENSOR_H
#define CCI_FOOTBOT_LIDAR_SENSOR_H

namespace argos {
   class CCI_FootBotLidarSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/angles.h>

namespace argos {

   class CCI_FootBotLidarSensor : public CCI_Sensor {

   public:

      struct SReading {
         Real Value;
         CRadians Angle;

         SReading() :
            Value(0.0f) {}

         SReading(Real f_value,
                  const CRadians& c_angle) :
            Value(f_value),
            Angle(c_angle) {}
      };

      typedef std::vector<SReading> TReadings;

   public:

      /**
       * Class constructor
       */
      CCI_FootBotLidarSensor();

      /**
       * Class destructor
       */
      virtual ~CCI_FootBotLidarSensor() {}

      /**
       * Returns the readings of this sensor
       */
      const TReadings& GetReadings() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      TReadings m_tReadings;

   };

   std::ostream& operator<<(std::ostream& c_os, const CCI_FootBotLidarSensor::SReading& s_reading);
   std::ostream& operator<<(std::ostream& c_os, const CCI_FootBotLidarSensor::TReadings& t_readings);

}

#endif
