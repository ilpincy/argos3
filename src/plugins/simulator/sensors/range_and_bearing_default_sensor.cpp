/**
 * @file <argos3/plugins/simulator/sensors/range_and_bearing_default_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "range_and_bearing_default_sensor.h"
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRange<CRadians> INCLINATION_RANGE(CRadians(0), CRadians(ARGOS_PI));

   /****************************************/
   /****************************************/

   CRangeAndBearingDefaultSensor::CRangeAndBearingDefaultSensor() :
      m_pcRangeAndBearingEquippedEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_fDistanceNoiseStdDev(0.0f),
      m_pcRNG(NULL),
      m_bShowRays(false),
      m_cSpace(CSimulator::GetInstance().GetSpace()),
      m_cEmbodiedEntityIndex(m_cSpace.GetEmbodiedEntityIndex()),
      m_cRABEquippedSpaceHash(m_cSpace.GetRABEquippedEntitiesSpaceHash()) {}

   /****************************************/
   /****************************************/

   void CRangeAndBearingDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      /* Assign RAB equipped entity to this sensor */
      m_pcRangeAndBearingEquippedEntity = &c_entity.GetComponent<CRABEquippedEntity>("rab");
      /* Enable the RAB equipped entity */
      m_pcRangeAndBearingEquippedEntity->SetCanBeEnabledIfDisabled(true);
      m_pcRangeAndBearingEquippedEntity->Enable();
      /* Get reference to embodied entity */
      m_pcEmbodiedEntity = &c_entity.GetComponent<CEmbodiedEntity>("body");
      /* Ignore the body of the parent composable in occlusion check */
      m_tIgnoreMe.insert(m_pcEmbodiedEntity);
      /* Get reference to controllable entity */
      m_pcControllableEntity = &c_entity.GetComponent<CControllableEntity>("controller");
   }

   /****************************************/
   /****************************************/

   void CRangeAndBearingDefaultSensor::Init(TConfigurationNode& t_tree) {
      /* Parent class init */
      CCI_RangeAndBearingSensor::Init(t_tree);
      /* Parse noise */
      GetNodeAttributeOrDefault(t_tree, "noise_std_dev", m_fDistanceNoiseStdDev, m_fDistanceNoiseStdDev);
      if(m_fDistanceNoiseStdDev > 0.0f) {
         m_pcRNG = CRandom::CreateRNG("argos");
      }
      /* Show rays? */
      GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
   }

   /****************************************/
   /****************************************/

   void CRangeAndBearingDefaultSensor::Update() {
      m_tReadings.clear();
      /* Buffer for calculating the message--robot distance */
      CVector3 cVectorRobotToMessage;
      Real fMessageDistance;
      /* Buffer for the received packet */
      CCI_RangeAndBearingSensor::SPacket sPacket;
      /* Initialize the occlusion check ray start to the position of the robot */
      CRay3 cOcclusionCheckRay;
      cOcclusionCheckRay.SetStart(m_pcRangeAndBearingEquippedEntity->GetPosition());
      /* Buffer to store the intersection data */
      CSpace::SEntityIntersectionItem<CEmbodiedEntity> sIntersectionData;
      /* Buffer to store the list of RAB entitities to check */
      CRABEquippedEntity::TSet tRABEntities;
      /*
       * 1. Go through the RAB entities in the buckets of the space hash covered by the bounding box of the robot
       * 2. Then, for each of them:
       *    a) check that the RAB entity is compatible with that of this robot
       *    b) Check if there is an occlusion
       *    c) If there isn't, get the info and set reading for that robot
       */
      const SBoundingBox& sBBox = m_pcEmbodiedEntity->GetBoundingBox();
      m_cRABEquippedSpaceHash.SpaceToHashTable(m_nBBoxMinI, m_nBBoxMinJ, m_nBBoxMinK, sBBox.MinCorner);
      m_cRABEquippedSpaceHash.SpaceToHashTable(m_nBBoxMaxI, m_nBBoxMaxJ, m_nBBoxMaxK, sBBox.MaxCorner);
      for(SInt32 i = m_nBBoxMinI; i <= m_nBBoxMaxI; ++i) {         
         for(SInt32 j = m_nBBoxMinJ; j <= m_nBBoxMaxJ; ++j) {         
            for(SInt32 k = m_nBBoxMinK; k <= m_nBBoxMaxK; ++k) {
               m_cRABEquippedSpaceHash.CheckCell(i, j, k, tRABEntities);
            }
         }
      }
      /* Go through the items */
      for(CRABEquippedEntity::TSet::iterator it = tRABEntities.begin();
          it != tRABEntities.end();
          ++it) {
         /* Create a reference to the RAB entity to check */
         CRABEquippedEntity& cRABEntity = **it;
         /* Check the RAB equipped entity is not this robot (avoid self-messaging) */
         if((&cRABEntity != m_pcRangeAndBearingEquippedEntity) &&
            (cRABEntity.GetMsgSize() == m_pcRangeAndBearingEquippedEntity->GetMsgSize())) {
            /*
             * The message is considered received if:
             * 1. the source of the message is not beyond the range;
             * 2. there are no occlusions between the message source and this RAB entity.
             */
            cVectorRobotToMessage = cRABEntity.GetPosition();
            cVectorRobotToMessage -= m_pcRangeAndBearingEquippedEntity->GetPosition();
            /* If noise was setup, add it */
            if(m_fDistanceNoiseStdDev > 0.0f) {
               cVectorRobotToMessage += CVector3(
                  m_pcRNG->Gaussian(m_fDistanceNoiseStdDev),
                  m_pcRNG->Uniform(INCLINATION_RANGE),
                  m_pcRNG->Uniform(CRadians::UNSIGNED_RANGE));
            }
            /* Check that the distance is lower than the range */
            fMessageDistance = cVectorRobotToMessage.Length();
            if(fMessageDistance < cRABEntity.GetRange()) {
               /* Set the ray end */
               cOcclusionCheckRay.SetEnd(cRABEntity.GetPosition());
               /* Check occlusion between robot and message location */
               if(! m_cSpace.GetClosestEmbodiedEntityIntersectedByRay(sIntersectionData,
                                                                      cOcclusionCheckRay,
                                                                      m_tIgnoreMe) ||
                  (&cRABEntity.GetParent() == &sIntersectionData.IntersectedEntity->GetParent())) {
                  /* The message is not occluded */
                  if(m_bShowRays) m_pcControllableEntity->AddCheckedRay(false, cOcclusionCheckRay);
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
               else {
                  /* The message is occluded */
                  if(m_bShowRays) {
                     m_pcControllableEntity->AddCheckedRay(true, cOcclusionCheckRay);
                     m_pcControllableEntity->AddIntersectionPoint(cOcclusionCheckRay, sIntersectionData.TOnRay);
                  }
               }
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   void CRangeAndBearingDefaultSensor::Reset() {
      m_tReadings.clear();
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CRangeAndBearingDefaultSensor,
                   "range_and_bearing", "default",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The range and bearing sensor",
                   "TODO",
                   "Usable");
   
}
