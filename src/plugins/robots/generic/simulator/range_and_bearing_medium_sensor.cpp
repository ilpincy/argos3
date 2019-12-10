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
      m_fPacketDropProb(0.0f),
      m_cSpace(CSimulator::GetInstance().GetSpace()),
      m_bShowRays(false),
      m_cPacketDropNoiseInjector() {}

   /****************************************/
   /****************************************/

   void CRangeAndBearingMediumSensor::SetRobot(CComposableEntity& c_entity) {
      /* Assign RAB equipped entity to this sensor */
      m_pcRangeAndBearingEquippedEntity = &c_entity.GetComponent<CRABEquippedEntity>("rab");
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
         /* Parse noise injection */
         if(NodeExists(t_tree, "distance_noise")) {
           TConfigurationNode& tNode = GetNode(t_tree, "distance_noise");
           m_cDistanceNoiseInjector.Init(tNode);
         }
         if(NodeExists(t_tree, "packet_drop_noise")) {
           TConfigurationNode& tNode = GetNode(t_tree, "packet_drop_noise");
           m_cPacketDropNoiseInjector.Init(tNode);
         }

         /* Get RAB medium from id specified in the XML */
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcRangeAndBearingMedium = &(CSimulator::GetInstance().GetMedium<CRABMedium>(strMedium));
         /* Assign RAB entity to the medium */
         m_pcRangeAndBearingEquippedEntity->SetMedium(*m_pcRangeAndBearingMedium);
         /* Enable the RAB equipped entity */
         m_pcRangeAndBearingEquippedEntity->Enable();
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
      const CSet<CRABEquippedEntity*,SEntityComparator>& setRABs = m_pcRangeAndBearingMedium->GetRABsCommunicatingWith(*m_pcRangeAndBearingEquippedEntity);
      /* Buffer for calculating the message--robot distance */
      CVector3 cVectorRobotToMessage;
      /* Buffer for the received packet */
      CCI_RangeAndBearingSensor::SPacket sPacket;
      /* Go through communicating RABs and create packets */
      for(CSet<CRABEquippedEntity*>::iterator it = setRABs.begin();
          it != setRABs.end(); ++it) {
        /* Should we drop this packet? */
        if(m_cPacketDropNoiseInjector.Enabled() &&
           !m_cPacketDropNoiseInjector.BernoulliEvent()) {
          continue;
        }
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
        if(m_cDistanceNoiseInjector.Enabled()) {
          cVectorRobotToMessage += CVector3(m_cDistanceNoiseInjector.InjectNoise(),
                                            CRadians(m_cInclinationNoiseInjector.InjectNoise()),
                                            CRadians(m_cAzimuthNoiseInjector.InjectNoise()));
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
                   "The range-and-bearing sensor.",

                   "This sensor allows robots to perform situated communication, i.e., a form of\n"
                   "wireless communication whereby the receiver also knows the location of the\n"
                   "sender with respect to its own frame of reference.\n"
                   "This implementation of the range-and-bearing sensor is associated to the\n"
                   "range-and-bearing medium. To be able to use this sensor, you must add a\n"
                   "range-and-bearing medium to the <media> section.\n"
                   "This sensor allows a robot to receive messages. To send messages, you need the\n"
                   "range-and-bearing actuator.\n"
                   "To use this sensor, in controllers you must include the\n"
                   "ci_range_and_bearing_sensor.h header.\n\n"

                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <range_and_bearing implementation=\"medium\"\n"
                   "                           medium=\"rab\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "The 'medium' attribute must be set to the id of the range-and-bearing medium\n"
                   "declared in the <media> section.\n\n"

                   "OPTIONAL XML CONFIGURATION\n\n"

                   "It is possible to draw the rays shot by the range-and-bearing sensor in the\n"
                   "OpenGL visualization. This can be useful for sensor debugging but also to\n"
                   "understand what's wrong in your controller. In OpenGL, the rays are drawn in\n"
                   "cyan when two robots are communicating.\n"
                   "To turn this functionality on, add the attribute \"show_rays\" as in this\n"
                   "example:\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <range_and_bearing implementation=\"medium\"\n"
                   "                           medium=\"rab\"\n"
                   "                           show_rays=\"true\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "----------------------------------------\n"
                   "Noise Injection\n"
                   "----------------------------------------\n" +

                   CNoiseInjector::GetQueryDocumentation({
                       .strDocName = "RAB sensor",
                           .strXMLParent = "range_and_bearing",
                           .strXMLTag = "distance_drop_noise",
                           .strSAAType = "sensor",
                           .bShowExamples = true}) +

                   "Each timestep distance noise injection for the RAB sensor is enabled, a vector\n"
                   "of randomly generated noise {'model', Uniform(0, PI), Uniform(0, 2PI)} is\n"
                   "added to the (distance, inclination, azimuth) position readings for each entity.\n"
                   "within range. That is, the model of noise for the distance measure for the reading,\n"
                   "is configurable, and the model for the (inclination, azimuth) measures for the reading\n"
                   "are always Uniform().\n\n" +

                   CNoiseInjector::GetQueryDocumentation({
                       .strDocName = "RAB sensor",
                           .strXMLParent = "range_and_bearing",
                           .strXMLTag = "packet_drop_noise",
                           .strSAAType = "sensor",
                           .bShowExamples = true}) +

                   "Each timestep packet drop noise injection for the RAB sensor is enabled, the injected\n"
                   "noise (n) for each packet is used to compute Bernoulli(n) to determine if the packet\n"
                   "should be dropped or not.\n\n",

                   "Usable");

}
