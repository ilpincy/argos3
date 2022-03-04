/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_rangefinders_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_rangefinders_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/physics_engine/physics_model.h>
#include <argos3/plugins/simulator/entities/led_entity.h>
#include <argos3/plugins/simulator/media/led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotRangefindersDefaultSensor::SSimulatedInterface::SSimulatedInterface(const std::string& str_label,
                                                                                  const SAnchor& s_anchor,
                                                                                  CEmbodiedEntity& c_embodied_entity,
                                                                                  CControllableEntity& c_controllable_entity,
                                                                                  Real f_range,
                                                                                  const bool& b_show_rays) :
      SInterface(str_label),
      Anchor(s_anchor),
      EmbodiedEntity(c_embodied_entity),
      ControllableEntity(c_controllable_entity),
      Range(f_range),
      ShowRays(b_show_rays) {}

   /****************************************/
   /****************************************/

   CBuilderBotRangefindersDefaultSensor::CBuilderBotRangefindersDefaultSensor() :
      m_pcEmbodiedEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcLightIndex(nullptr),
      m_bShowRays(false) {
      m_vecSimulatedInterfaces.reserve(16);
      m_vecInterfaces.reserve(16);
   }

   /****************************************/
   /****************************************/

   void CBuilderBotRangefindersDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      /* get components */
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
      /* get anchors */
      SAnchor& sOriginAnchor = m_pcEmbodiedEntity->GetOriginAnchor();
      SAnchor& sEndEffectorAnchor = m_pcEmbodiedEntity->GetAnchor("end_effector");
      /* instantiate sensors */
      m_vecSimulatedInterfaces.emplace_back("1", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("2", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("3", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("4", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("5", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("6", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("7", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("8", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("9", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("10", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("11", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("12", sOriginAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("left", sEndEffectorAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("right", sEndEffectorAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("underneath", sEndEffectorAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      m_vecSimulatedInterfaces.emplace_back("front", sEndEffectorAnchor, *m_pcEmbodiedEntity, *m_pcControllableEntity, 0.05, m_bShowRays);
      /* copy pointers to the the base class */
      for(SSimulatedInterface& s_simulated_interface : m_vecSimulatedInterfaces) {
         m_vecInterfaces.push_back(&s_simulated_interface);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotRangefindersDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotRangefindersSensor::Init(t_tree);
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         std::string strLightMedium;
         GetNodeAttributeOrDefault(t_tree, "light_medium", strLightMedium, strLightMedium);
         if(!strLightMedium.empty()) {
            m_pcLightIndex = &CSimulator::GetInstance().GetMedium<CLEDMedium>(strLightMedium).GetIndex();
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot rangefinders sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotRangefindersDefaultSensor::Update() {
      class CRangefinderOperation : public CPositionalIndex<CLEDEntity>::COperation {
      public:
         CRangefinderOperation(SSimulatedInterface& s_interface) :
            m_sInterface(s_interface),
            m_fTotalLight(0.0) {
            m_cSensorPosition = m_sInterface.PositionOffset;
            m_cSensorPosition.Rotate(m_sInterface.Anchor.Orientation);
            m_cSensorPosition += m_sInterface.Anchor.Position;
            m_cSensorDirection = CVector3::Z * m_sInterface.Range;
            m_cSensorDirection.Rotate(m_sInterface.OrientationOffset);
            m_cSensorDirection.Rotate(m_sInterface.Anchor.Orientation);
            m_cSensorRay.Set(m_cSensorPosition, m_cSensorPosition + m_cSensorDirection);
            /* Compute reading */
            /* Get the closest intersection */
            if(GetClosestEmbodiedEntityIntersectedByRay(m_sIntersection, m_cSensorRay)) {
               /* There is an intersection */
               if(m_sInterface.ShowRays) {
                  m_sInterface.ControllableEntity.AddIntersectionPoint(m_cSensorRay, m_sIntersection.TOnRay);
                  m_sInterface.ControllableEntity.AddCheckedRay(true, m_cSensorRay);
               }
               Real fDistance = m_cSensorRay.GetDistance(m_sIntersection.TOnRay);
               m_sInterface.Proximity =
                  CBuilderBotRangefindersDefaultSensor::ConvertToMeters(fDistance);
            }
            else {
               /* No intersection */
               m_sInterface.Proximity = 0.05;
               if(m_sInterface.ShowRays) {
                  m_sInterface.ControllableEntity.AddCheckedRay(false, m_cSensorRay);
               }
            }
         }
         /* destructor writes back the total illuminance */
         ~CRangefinderOperation() {
            m_fTotalLight = std::min(m_fTotalLight, static_cast<Real>(UINT8_MAX));
            m_sInterface.Illuminance = m_fTotalLight / static_cast<Real>(UINT8_MAX);
         }
         /* operator() accumulates the light measurement from different sources */
         virtual bool operator()(CLEDEntity& c_light) {
            Real fBrightness = c_light.GetColor().ToGrayScale();
            /* decrease fBrightness with respect to inverse square law */
            fBrightness *= 1.0 / (1.0 + SquareDistance(c_light.GetPosition(), m_cSensorPosition));
            /* decrease fBrightness with respect to the angle between the light source and the sensor */
            CVector3 cSensorToLight(c_light.GetPosition() - m_cSensorPosition);
            Real fCosine =
               (m_cSensorDirection.DotProduct(cSensorToLight) /
                  (m_cSensorDirection.Length() * cSensorToLight.Length()));
            fBrightness *= (fCosine + 1.0) / 2.0;
            /* the following lines were written to emulate the reduction in light due to the shadows
               cast by emodied entities in the simulation, uncomment at your own risk */
            //m_cSensorRay.SetEnd(c_light.GetPosition());
            //if(GetClosestEmbodiedEntityIntersectedByRay(m_sIntersection, m_cSensorRay, m_sInterface.EmbodiedEntity)) {
            //   fBrightness *= Abs(0.5 - m_sIntersection.TOnRay);
            //}
            m_fTotalLight += fBrightness;
            return true;
         }
      private:
         /* local data */
         SSimulatedInterface& m_sInterface;
         Real m_fTotalLight;
         CRay3 m_cSensorRay;
         CVector3 m_cSensorPosition, m_cSensorDirection;
         SEmbodiedEntityIntersectionItem m_sIntersection;
      };
      /* for each sensor */
      for(SSimulatedInterface& s_simulated_interface : m_vecSimulatedInterfaces) {
         /* COperation::COperation performs the proximity measurement */
         CRangefinderOperation cRangefinderOperation(s_simulated_interface);
         /* COperation::operator() performs the light measurement */
         if(m_pcLightIndex != nullptr) {
            m_pcLightIndex->ForAllEntities(cRangefinderOperation);
         }
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotRangefindersDefaultSensor::Reset() {
      for(SSimulatedInterface& s_simulated_interface : m_vecSimulatedInterfaces) {
         s_simulated_interface.Proximity = 0.05;
      }
   }
   
   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBuilderBotRangefindersDefaultSensor,
                  "builderbot_rangefinders", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot rangefinders sensor.",
                  "The documentation for this sensor is \e]8;;https://github.com/ilpincy/argos3/blob/master/src/plugins/robots/builderbot/README.md#builderbot_rangefinders\aavailable on Github\e]8;;\a.",
                  "Usable"
   );
   
}

   
