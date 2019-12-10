/**
 * @file <argos3/plugins/simulator/sensors/ground_rotzonly_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/floor_entity.h>
#include <argos3/plugins/simulator/entities/ground_sensor_equipped_entity.h>

#include "ground_rotzonly_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   static CRange<Real> UNIT(0.0f, 1.0f);

   /****************************************/
   /****************************************/

   CGroundRotZOnlySensor::CGroundRotZOnlySensor() :
      m_pcEmbodiedEntity(NULL),
      m_pcFloorEntity(NULL),
      m_pcGroundSensorEntity(NULL),
      m_cSpace(CSimulator::GetInstance().GetSpace()) {}

   /****************************************/
   /****************************************/

   void CGroundRotZOnlySensor::SetRobot(CComposableEntity& c_entity) {
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_pcGroundSensorEntity = &(c_entity.GetComponent<CGroundSensorEquippedEntity>("ground_sensors"));
      m_pcGroundSensorEntity->Enable();
      m_pcFloorEntity = &m_cSpace.GetFloorEntity();
   }

   /****************************************/
   /****************************************/

   void CGroundRotZOnlySensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_GroundSensor::Init(t_tree);
         /* Parse noise injection */
         if(NodeExists(t_tree, "noise")) {
           TConfigurationNode& tNode = GetNode(t_tree, "noise");
           m_cNoiseInjector.Init(tNode);
         }
         m_tReadings.resize(m_pcGroundSensorEntity->GetNumSensors());
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in rotzonly ground sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CGroundRotZOnlySensor::Update() {
      /*
       * We make the assumption that the robot is rotated only wrt to Z
       */
      CRadians cRotZ, cRotY, cRotX;
      /* Set robot center */
      CVector2 cCenterPos;
      /* Position of sensor on the ground after rototranslation */
      CVector2 cSensorPos;
      /* Go through the sensors */
      for(UInt32 i = 0; i < m_tReadings.size(); ++i) {
         CGroundSensorEquippedEntity::SSensor& sSens = m_pcGroundSensorEntity->GetSensor(i);
         /* Get anchor position and orientation */
         cCenterPos.Set(sSens.Anchor.Position.GetX(),
                        sSens.Anchor.Position.GetY());
         sSens.Anchor.Orientation.ToEulerAngles(cRotZ, cRotY, cRotX);
         /* Calculate sensor position on the ground */
         cSensorPos = sSens.Offset;
         cSensorPos.Rotate(cRotZ);
         cSensorPos += cCenterPos;
         /* Get the color */
         const CColor& cColor = m_pcFloorEntity->GetColorAtPoint(cSensorPos.GetX(),
                                                                 cSensorPos.GetY());
         /* Set the reading */
         m_tReadings[i] = cColor.ToGrayScale() / 255.0f;
         /* Apply noise to the sensor */
         if(m_cNoiseInjector.Enabled()) {
           m_tReadings[i] += m_cNoiseInjector.InjectNoise();
         }
         /* Is it a BW sensor? */
         if(sSens.Type == CGroundSensorEquippedEntity::TYPE_BLACK_WHITE) {
            /* Yes, set 0 or 1 */
            m_tReadings[i] = m_tReadings[i] < 0.5f ? 0.0f : 1.0f;
         }
         else {
            /* No, clamp the reading between 0 and 1 */
            UNIT.TruncValue(m_tReadings[i]);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CGroundRotZOnlySensor::Reset() {
      for(UInt32 i = 0; i < GetReadings().size(); ++i) {
         m_tReadings[i] = 0.0f;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CGroundRotZOnlySensor,
                   "ground", "rot_z_only",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "A generic ground sensor (optimized for 2D).",

                   "This sensor accesses a set of ground sensors. The sensors all return a value\n"
                   "between 0 and 1, where 0 means black and 1 means white. Depending on the type\n"
                   "of ground sensor, readings can either take 0 or 1 as value (bw sensors) or a\n"
                   "value in between (grayscale sensors). In controllers, you must include the\n"
                   "ci_ground_sensor.h header.\n\n"

                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <ground implementation=\"rot_z_only\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "OPTIONAL XML CONFIGURATION\n\n"

                   "----------------------------------------\n"
                   "Noise Injection\n"
                   "----------------------------------------\n" +


                   CNoiseInjector::GetQueryDocumentation({
                       .strDocName = "ground sensor",
                           .strXMLParent = "ground_sensor",
                           .strXMLTag = "noise",
                           .strSAAType = "sensor",
                           .bShowExamples = true}) +

                   "The final sensor reading after noise has been added is clamped to the [0-1] range.\n\n",

                   "Usable"
		  );

}
