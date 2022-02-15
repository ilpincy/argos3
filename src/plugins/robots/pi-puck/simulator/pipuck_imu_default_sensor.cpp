/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_imu_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_imu_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>

namespace argos {
   
   /****************************************/
   /****************************************/

   void CPiPuckImuDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcBody = &c_entity.GetComponent<CEmbodiedEntity>("body");
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckImuDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckImuSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the PiPuck IMU sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CPiPuckImuDefaultSensor::Reset() {
      m_cAccelerometer = CVector3::ZERO;
      m_cGryoscope = CVector3::ZERO;
      m_cMagnetometer = CVector3::ZERO;
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckImuDefaultSensor::Update() {
      /* calculate a reading for the accelerometer */
      // TODO
      /* calculate a reading for the gyroscope */
      // TODO
      /* calculate a reading for the magnetometer */
      //const SAnchor& sOriginAnchor = m_pcBody->GetOriginAnchor();
      //const CQuaternion& cOrientation = sOriginAnchor.Orientation;
      //m_cMagnetometer.Set(1,2,3); // X, Y, Z
   }

   /****************************************/
   /****************************************/
   
   REGISTER_SENSOR(CPiPuckImuDefaultSensor,
                   "pipuck_imu", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The pipuck IMU sensor.",
                   "This sensor provides readings from the IMU on the E-Puck2.",
                   "Incomplete"
   );

   /****************************************/
   /****************************************/
   
}

   
