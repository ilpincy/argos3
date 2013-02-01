
/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_base_ground_sensor.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @brief This file provides the implementation of the footbot base ground sensor.
 *
 * On the robot, there are two kinds of ground sensors.
 *
 * The BASE ground sensors are located on the main PCB, intertwined with the
 * proximity sensors and evenly spaced. They are 8 and are useful for detecting
 * holes.
 *
 * The MOTOR ground sensors are built on the motor PCB and located close to the
 * motors. They are 4 and useful to detect changes in color on the ground (i.e.,
 * for line following).
 *
 * This sensor provides access to the base sensors. The readings are in the following
 * order (seeing the robot from TOP, battery socket is the BACK):
 *
 *      front
 *
 *        0      r
 * l    1   7    i
 * e  2       6  g
 * f    3   5    h
 * t      4      t
 *
 *       back
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_footbot_base_ground_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   const UInt32 CCI_FootBotBaseGroundSensor::NUM_READINGS = 8;
   const CRange<Real> CCI_FootBotBaseGroundSensor::READING_RANGE(0.0f, 4096.0f);

   /****************************************/
   /****************************************/

   CCI_FootBotBaseGroundSensor::CCI_FootBotBaseGroundSensor() :
      m_tReadings(NUM_READINGS) {
      // Set the values for the base ground sensor offset (taken from the CAD model, in cm)
      m_tReadings[0].Offset.Set( 8.0, 0.0);
      m_tReadings[1].Offset.Set( 4.2, 6.5);
      m_tReadings[2].Offset.Set( 0.0, 8.0);
      m_tReadings[3].Offset.Set(-4.2, 6.5);
      m_tReadings[4].Offset.Set(-8.0, 0.0);
      m_tReadings[5].Offset.Set(-4.2,-6.5);
      m_tReadings[6].Offset.Set( 0.0,-8.0);
      m_tReadings[7].Offset.Set( 4.2,-6.5);
   }

  /****************************************/
  /****************************************/
  
   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotBaseGroundSensor::SReading& s_reading) {
      c_os << "Value=<" << s_reading.Value
           << ">, Offset=<" << s_reading.Offset << ">";
      return c_os;
   }

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotBaseGroundSensor::TReadings& t_readings) {
      if(! t_readings.empty()) {
         c_os << "{ " << t_readings[0].Value << " }";
         for(UInt32 i = 1; i < t_readings.size(); ++i) {
            c_os << " { " << t_readings[0].Value << " }";
         }
         c_os << std::endl;
      }
      return c_os;
   }

   /****************************************/
   /****************************************/

}
