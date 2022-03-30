/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_ground_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include <argos3/core/utility/math/plane.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/floor_entity.h>

#include "pipuck_ground_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   CPiPuckGroundDefaultSensor::CPiPuckGroundDefaultSensor() :
      m_bShowRays(false),
      m_pcControllableEntity(nullptr),
      m_cFloorEntity(CSimulator::GetInstance().GetSpace().GetFloorEntity()) {}

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
      /* allocate memory for the sensor interfaces */
      m_vecSimulatedInterfaces.reserve(MAP_SENSOR_CONFIG.size());
      /* get the anchors for the sensor interfaces from m_mapSensorConfig */
      for(const std::pair<const UInt8, TConfiguration>& t_config : MAP_SENSOR_CONFIG) {
         const std::string& strAnchor = std::get<std::string>(t_config.second);
         SAnchor& sAnchor =
            c_entity.GetComponent<CEmbodiedEntity>("body").GetAnchor(strAnchor);
         m_vecSimulatedInterfaces.emplace_back(t_config.first, sAnchor);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckGroundSensor::Init(t_tree);
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in Pi-Puck ground sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::Update() {
      CVector3 cRayStart, cRayEnd, cIntersection;
      CRay3 cSensorRay;
      /* go through the sensors */
      for(SSimulatedInterface& s_interface : m_vecSimulatedInterfaces) {
         cRayStart = std::get<CVector3>(s_interface.Configuration);
         cRayStart.Rotate(s_interface.Anchor.Orientation);
         cRayStart += s_interface.Anchor.Position;
         cRayEnd = CVector3::Z * std::get<Real>(s_interface.Configuration);
         cRayEnd.Rotate(std::get<CQuaternion>(s_interface.Configuration));
         cRayEnd.Rotate(s_interface.Anchor.Orientation);
         cRayEnd += cRayStart;
         cSensorRay.Set(cRayStart, cRayEnd);
         bool bIntersection = cSensorRay.Intersects(m_cFloor, cIntersection);
         if(m_bShowRays) {
            m_pcControllableEntity->GetCheckedRays().emplace_back(bIntersection, cSensorRay);
         }
         if(bIntersection) {
            /* get the color */
            const CColor& cColor =
               m_cFloorEntity.GetColorAtPoint(cIntersection.GetX(), cIntersection.GetY());
            /* set reading */
            s_interface.Reflected = cColor.ToGrayScale() / 255.0;
         }
         else {
            s_interface.Reflected = 1.0;
         }
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::Reset() {
      for(SSimulatedInterface& s_interface : m_vecSimulatedInterfaces) {
         s_interface.Reflected = 0.0;
         s_interface.Background = 0.0;
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckGroundDefaultSensor::Visit(std::function<void(const SInterface&)> fn_visitor) {
      for(const SSimulatedInterface& s_interface : m_vecSimulatedInterfaces) {
         fn_visitor(s_interface);
      }
   }

   /****************************************/
   /****************************************/

   const CPlane CPiPuckGroundDefaultSensor::m_cFloor(CVector3::ZERO, CVector3::Z);

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CPiPuckGroundDefaultSensor,
                   "pipuck_ground", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The Pi-Puck ground sensor.",
                   "The documentation for this sensor is \e]8;;https://github.com/ilpincy/argos3/blob/master/src/plugins/robots/pi-puck/README.md#pipuck_ground\aavailable on Github\e]8;;\a.",
                   "Usable"
   );

   /****************************************/
   /****************************************/

}
