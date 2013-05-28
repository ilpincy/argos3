/**
 * @file <argos3/plugins/simulator/sensors/range_and_bearing_medium_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "range_and_bearing_medium_sensor.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>
#include <argos3/plugins/simulator/media/rab_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRange<CRadians> INCLINATION_RANGE(CRadians(0), CRadians(ARGOS_PI));

   /****************************************/
   /****************************************/

   CRangeAndBearingMediumSensor::CRangeAndBearingMediumSensor() :
      m_pcRangeAndBearingEquippedEntity(NULL),
      m_fDistanceNoiseStdDev(0.0f),
      m_pcRNG(NULL),
      m_cSpace(CSimulator::GetInstance().GetSpace()),
      m_bShowRays(false) {}

   /****************************************/
   /****************************************/

   void CRangeAndBearingMediumSensor::SetRobot(CComposableEntity& c_entity) {
      /* Assign RAB equipped entity to this sensor */
      m_pcRangeAndBearingEquippedEntity = &c_entity.GetComponent<CRABEquippedEntity>("rab");
      /* Enable the RAB equipped entity */
      m_pcRangeAndBearingEquippedEntity->SetCanBeEnabledIfDisabled(true);
      m_pcRangeAndBearingEquippedEntity->Enable();
      /* Get reference to controllable entity */
      m_pcControllableEntity = &c_entity.GetComponent<CControllableEntity>("controller");
   }

   /****************************************/
   /****************************************/

   void CRangeAndBearingMediumSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_RangeAndBearingSensor::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Parse noise */
         GetNodeAttributeOrDefault(t_tree, "noise_std_dev", m_fDistanceNoiseStdDev, m_fDistanceNoiseStdDev);
         if(m_fDistanceNoiseStdDev > 0.0f) {
            m_pcRNG = CRandom::CreateRNG("argos");
         }
         /* Get RAB medium from id specified in the XML */
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcRangeAndBearingMedium = &(CSimulator::GetInstance().GetMedium<CRABMedium>(strMedium));
         /* Assign RAB entity to the medium */
         m_pcRangeAndBearingMedium->AddEntity(*m_pcRangeAndBearingEquippedEntity);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the range and bearing medium sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRangeAndBearingMediumSensor::Update() {
      /** TODO: there's a more efficient way to implement this */
      /* Delete old readings */
      m_tReadings.clear();
      /* Get list of communicating RABs */
      const CSet<CRABEquippedEntity*>& setRABs = m_pcRangeAndBearingMedium->GetRABsCommunicatingWith(*m_pcRangeAndBearingEquippedEntity);
      /* Buffer for calculating the message--robot distance */
      CVector3 cVectorRobotToMessage;
      /* Buffer for the received packet */
      CCI_RangeAndBearingSensor::SPacket sPacket;
      /* Go through communicating RABs and create packets */
      for(CSet<CRABEquippedEntity*>::iterator it = setRABs.begin();
          it != setRABs.end(); ++it) {
         /* Create a reference to the RAB entity to process */
         CRABEquippedEntity& cRABEntity = **it;
         /* Add ray if requested */
         if(m_bShowRays) {
            m_pcControllableEntity->AddCheckedRay(false,
                                                  CRay3(cRABEntity.GetPosition(),
                                                        m_pcRangeAndBearingEquippedEntity->GetPosition()));
         }
         /* Calculate vector to entity */
         cVectorRobotToMessage = cRABEntity.GetPosition();
         cVectorRobotToMessage -= m_pcRangeAndBearingEquippedEntity->GetPosition();
         /* If noise was setup, add it */
         if(m_fDistanceNoiseStdDev > 0.0f) {
            cVectorRobotToMessage += CVector3(
               m_pcRNG->Gaussian(m_fDistanceNoiseStdDev),
               m_pcRNG->Uniform(INCLINATION_RANGE),
               m_pcRNG->Uniform(CRadians::UNSIGNED_RANGE));
         }
         /*
          * Set range and bearing from cVectorRobotToMessage
          * First, we must rotate the cVectorRobotToMessage so that
          * it is local to the robot coordinate system. To do this,
          * it enough to rotate cVectorRobotToMessage by the inverse
          * of the robot orientation.
          */
         cVectorRobotToMessage.Rotate(m_pcRangeAndBearingEquippedEntity->GetOrientation().Inverse());
         cVectorRobotToMessage.ToSphericalCoords(sPacket.Range,
                                                 sPacket.VerticalBearing,
                                                 sPacket.HorizontalBearing);
         /* Convert range to cm */
         sPacket.Range *= 100.0f;
         /* Normalize horizontal bearing between [-pi,pi] */
         sPacket.HorizontalBearing.SignedNormalize();
         /*
          * The vertical bearing is defined as the angle between the local
          * robot XY plane and the message source position, i.e., the elevation
          * in math jargon. However, cVectorRobotToMessage.ToSphericalCoords()
          * sets sPacket.VerticalBearing to the inclination, which is the angle
          * between the azimuth vector (robot local Z axis) and
          * cVectorRobotToMessage. Elevation = 90 degrees - Inclination.
          */
         sPacket.VerticalBearing.Negate();
         sPacket.VerticalBearing += CRadians::PI_OVER_TWO;
         sPacket.VerticalBearing.SignedNormalize();
         /* Set message data */
         sPacket.Data = cRABEntity.GetData();
         /* Add message to the list */
         m_tReadings.push_back(sPacket);
      }
   }

   /****************************************/
   /****************************************/

   void CRangeAndBearingMediumSensor::Reset() {
      m_tReadings.clear();
   }

   /****************************************/
   /****************************************/

   void CRangeAndBearingMediumSensor::Destroy() {
      m_pcRangeAndBearingMedium->RemoveEntity(*m_pcRangeAndBearingEquippedEntity);
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CRangeAndBearingMediumSensor,
                   "range_and_bearing", "medium",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The range and bearing sensor",
                   "TODO",
                   "Usable");
   
}
