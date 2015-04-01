/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_distance_scanner_sensor.h>
 *
 * @brief This file provides the definition of the distance scanner sensor.
 *
 * This file provides the definition of the distance scanner sensor. This sensor
 * is located right below the iMX board. It is intended to return quite
 * precise (1cm accuracy) distance measurements of the objects around the
 * robot. As such, it can be seen as a better proximity sensor, in that,
 * besides accuracy, it also provides higher range.
 *
 * The distance scanner is composed of to pairs of sensors:
 * - short range sensors, that can measure distances in the range [4cm;30cm]
 *   and correspond to readings 0 and 2;
 * - long range sensors, that can measure distances in the range [20cm;150cm]
 *   and correspond to readings 1 and 3.
 *
 * Furthermore, the sensor can rotate. It can be rotated to a specific angle,
 * or it can keep rotating at a desired constant speed. Refer to the distance
 * scanner actuator to control this behavior. When the scanner is still, its
 * accuracy is maximum; when the scanner rotates, the accuracy degrades as the
 * speed increases.
 * The sensor rotates clockwise
 *
 * The sensors has a frequency of 60hz, which means that in theory you should receive
 * 6 new readings per sensor per control step, for a total of 24 readings. However,
 * this is not always the case - sometimes the device sends more readings, sometimes
 * less. To normalize the results for control purposes, the sensor returns always the
 * last 6 readings. This means that:
 * (i)  when the device sends >6 readings, the oldest among them are ignored;
 * (ii) when the device sends <6 readings, some old ones from the previous set are
 *      returned again.
 *
 * The sensors, when the device is rotated at angle 0, are placed as follows
 * (seeing the robot from TOP, battery socket is the BACK):
 *
 *      front
 *
 *        0
 *
 * left 1   3 right
 *
 *        2
 *
 *      back
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Manuele Brambilla - <mbrambilla@iridia.ulb.ac.be>
 */

#ifndef CCI_FOOTBOT_DISTANCE_SCANNER_SENSOR_H
#define CCI_FOOTBOT_DISTANCE_SCANNER_SENSOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_FootBotDistanceScannerSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/angles.h>
#include <vector>

namespace argos {

   class CCI_FootBotDistanceScannerSensor : public CCI_Sensor {

   public:

	   /**
	    * The DTO of the distance scanner,
	    * it contains the measured distance and its angle
	    */
      struct SReading {
         CRadians Angle;
         Real Distance; // in cm from the center of the robot (about 10cm from the border)

         SReading() :
            Angle(0.0f),
            Distance(0.0f) {}

         SReading(const CRadians& c_angle,
                  Real f_distance) :
            Angle(c_angle),
            Distance(f_distance) {
         }
      };

      /* These structs are for internal use only */
      typedef std::vector<SReading> TReadings;
      typedef std::vector<TReadings> TLastReadings;
      typedef std::pair<CRadians,Real> TMapPair;
      typedef std::map<CRadians,Real> TReadingsMap;

      /**
       * Constructor
       */
      CCI_FootBotDistanceScannerSensor() :
         m_tReadings(4){
      }

      /**
       * Destructor
       */
      virtual ~CCI_FootBotDistanceScannerSensor() {}

      /**
       *
       * @brief Return a map of the readings of the ds
       * A map of the readings of the ds in the following format:
       * angle - value
       *
       * The value is the distance wrt to a perceived obstacle.
       * If the value is -1 it means that the sensor is saturated (the obstacle is too close)
       * If the value is -2 it means that the sensor is empy (obstacle too far / no obstacle)
       * The "too far / too close" have different meanings for the short sensors and long sensors.
       * This map stores both long and short sensor readings.
       * It is suggested to use the short/long maps instead.
       *
       */
      const TReadingsMap& GetReadingsMap();

      /**
       *
       * @brief Return a map of the readings of the ds, short sensor readings
       * A map of the readings of the ds short sensor readings in the following format:
       * angle - value
       *
       * The value is the distance wrt to a perceived obstacle.
       * If the value is -1 it means that the sensor is saturated (the obstacle is too close, i.e. < 4cm from the robot border)
       * If the value is -2 it means that the sensor is empy (obstacle too far / no obstacle, i.e. > 30cm from the robot border)
       *
       */
      const TReadingsMap& GetShortReadingsMap();

      /**
       *
       * @brief Return a map of the readings of the ds, long sensor readings
       * A map of the readings of the ds long sensor readings in the following format:
       * angle - value
       *
       * The value is the distance wrt to a perceived obstacle.
       * If the value is -1 it means that the sensor is saturated (the obstacle is too close, i.e. < 20cm from the robot border)
       * If the value is -2 it means that the sensor is empy (obstacle too far / no obstacle, i.e. > 150cm from the robot border)
       *
       */
      const TReadingsMap& GetLongReadingsMap();

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      /** A vector of sReadings */
      TReadings m_tReadings;

      /** Vector storing the last received packets. */
      TLastReadings m_tLastReadings;

      /** Map storing all the last received packets. */
      TReadingsMap m_tReadingsMap;

      /** Map storing the last received packets from the short distance sensors. */
      TReadingsMap m_tShortReadingsMap;

      /** Map storing the last received packets from the long distance sensors. */
      TReadingsMap m_tLongReadingsMap;

   };

}

#endif
