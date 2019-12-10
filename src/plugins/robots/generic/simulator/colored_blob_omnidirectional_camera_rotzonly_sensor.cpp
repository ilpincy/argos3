#include "colored_blob_omnidirectional_camera_rotzonly_sensor.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/led_entity.h>
#include <argos3/plugins/simulator/entities/omnidirectional_camera_equipped_entity.h>
#include <argos3/plugins/simulator/media/led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   class COmnidirectionalCameraLEDCheckOperation : public CPositionalIndex<CLEDEntity>::COperation {

   public:

      COmnidirectionalCameraLEDCheckOperation(
         CCI_ColoredBlobOmnidirectionalCameraSensor::TBlobList& t_blobs,
         COmnidirectionalCameraEquippedEntity& c_omnicam_entity,
         CEmbodiedEntity& c_embodied_entity,
         CControllableEntity& c_controllable_entity,
         CNoiseInjector& c_distance_injector,
         CNoiseInjector& c_azimuth_injector,
         bool b_show_rays) :
         m_tBlobs(t_blobs),
         m_cOmnicamEntity(c_omnicam_entity),
         m_cEmbodiedEntity(c_embodied_entity),
         m_cControllableEntity(c_controllable_entity),
         m_cDistanceNoiseInjector(c_distance_injector),
         m_cAzimuthNoiseInjector(c_azimuth_injector),
         m_bShowRays(b_show_rays) {
         m_pcRootSensingEntity = &m_cEmbodiedEntity.GetParent();
      }

      virtual ~COmnidirectionalCameraLEDCheckOperation() {
         while(! m_tBlobs.empty()) {
            delete m_tBlobs.back();
            m_tBlobs.pop_back();
         }
      }

      virtual bool operator()(CLEDEntity& c_led) {
         /* Process this LED only if it's lit */
         if(c_led.GetColor() != CColor::BLACK) {
            if(c_led.HasParent()) {
               /* Filter out the LEDs belonging to the sensing entity by checking if they share the same parent entity */
               m_pcRootOfLEDEntity = &c_led.GetParent();
               while(m_pcRootOfLEDEntity->HasParent()) m_pcRootOfLEDEntity = &m_pcRootOfLEDEntity->GetParent();
               if(m_pcRootSensingEntity == m_pcRootOfLEDEntity) {
                  return true;
               }
            }
            /* If we are here, it's because the LED must be processed */
            m_cOcclusionCheckRay.SetEnd(c_led.GetPosition());
            m_cLEDRelativePos = c_led.GetPosition();
            m_cLEDRelativePos -= m_cCameraPos;
            m_cLEDRelativePosXY.Set(m_cLEDRelativePos.GetX(),
                                    m_cLEDRelativePos.GetY());
            if(Abs(m_cLEDRelativePos.GetX()) < m_fGroundHalfRange &&
               Abs(m_cLEDRelativePos.GetY()) < m_fGroundHalfRange &&
               m_cLEDRelativePos.GetZ() < m_cCameraPos.GetZ() &&
               !GetClosestEmbodiedEntityIntersectedByRay(m_sIntersectionItem,
                                                         m_cOcclusionCheckRay,
                                                         m_cEmbodiedEntity)) {
               /* If noise was setup, add it */
              if(m_cDistanceNoiseInjector.Enabled()) {
                  m_cLEDRelativePosXY += CVector2(
                     m_cLEDRelativePosXY.Length() * m_cDistanceNoiseInjector.InjectNoise(),
                     CRadians(m_cAzimuthNoiseInjector.InjectNoise()));
               }
               m_tBlobs.push_back(new CCI_ColoredBlobOmnidirectionalCameraSensor::SBlob(
                                     c_led.GetColor(),
                                     NormalizedDifference(m_cLEDRelativePosXY.Angle(), m_cCameraOrient),
                                     m_cLEDRelativePosXY.Length() * 100.0f));
               if(m_bShowRays) {
                  m_cControllableEntity.AddCheckedRay(false, CRay3(m_cCameraPos, c_led.GetPosition()));
               }
            }
         }
         return true;
      }

      void Setup(Real f_ground_half_range) {
         while(! m_tBlobs.empty()) {
            delete m_tBlobs.back();
            m_tBlobs.pop_back();
         }
         m_fGroundHalfRange = f_ground_half_range;
         m_cEmbodiedEntity.GetOriginAnchor().Orientation.ToEulerAngles(m_cCameraOrient, m_cTmp1, m_cTmp2);
         m_cCameraPos = m_cEmbodiedEntity.GetOriginAnchor().Position;
         m_cCameraPos += m_cOmnicamEntity.GetOffset();
         m_cOcclusionCheckRay.SetStart(m_cCameraPos);
      }

   private:

      CCI_ColoredBlobOmnidirectionalCameraSensor::TBlobList& m_tBlobs;
      COmnidirectionalCameraEquippedEntity& m_cOmnicamEntity;
      CEmbodiedEntity& m_cEmbodiedEntity;
      CControllableEntity& m_cControllableEntity;
      Real m_fGroundHalfRange;
      CNoiseInjector& m_cDistanceNoiseInjector;
      CNoiseInjector& m_cAzimuthNoiseInjector;
      bool m_bShowRays;
      CEntity* m_pcRootSensingEntity;
      CEntity* m_pcRootOfLEDEntity;
      CVector3 m_cCameraPos;
      CRadians m_cCameraOrient;
      CRadians m_cTmp1, m_cTmp2;
      CVector3 m_cLEDRelativePos;
      CVector2 m_cLEDRelativePosXY;
      SEmbodiedEntityIntersectionItem m_sIntersectionItem;
      CRay3 m_cOcclusionCheckRay;
   };

   /****************************************/
   /****************************************/

   CColoredBlobOmnidirectionalCameraRotZOnlySensor::CColoredBlobOmnidirectionalCameraRotZOnlySensor() :
      m_bEnabled(false),
      m_pcOmnicamEntity(NULL),
      m_pcControllableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcLEDIndex(NULL),
      m_pcEmbodiedIndex(NULL),
      m_bShowRays(false) {
   }
   /****************************************/
   /****************************************/

   CColoredBlobOmnidirectionalCameraRotZOnlySensor::~CColoredBlobOmnidirectionalCameraRotZOnlySensor() {
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraRotZOnlySensor::SetRobot(CComposableEntity& c_entity) {
      /* Get omndirectional camera equipped entity */
      m_pcOmnicamEntity = &(c_entity.GetComponent<COmnidirectionalCameraEquippedEntity>("omnidirectional_camera"));
      /* Get controllable entity */
      m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
      /* Get embodied entity */
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraRotZOnlySensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_ColoredBlobOmnidirectionalCameraSensor::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Init noise injection */
         if(NodeExists(t_tree, "noise")) {
           TConfigurationNode& tNode = GetNode(t_tree, "noise");
           m_cDistanceNoiseInjector.Init(tNode);
           /* always uniform noise for angle */
           m_cAzimuthNoiseInjector.InitUniform(CRange<Real>(0.0,
                                                            CRadians::TWO_PI.GetValue()));
         }
         /* Get LED medium from id specified in the XML */
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcLEDIndex = &(CSimulator::GetInstance().GetMedium<CLEDMedium>(strMedium).GetIndex());
         /* Create check operation */
         m_pcOperation = new COmnidirectionalCameraLEDCheckOperation(
            m_sReadings.BlobList,
            *m_pcOmnicamEntity,
            *m_pcEmbodiedEntity,
            *m_pcControllableEntity,
            m_cDistanceNoiseInjector,
            m_cAzimuthNoiseInjector,
            m_bShowRays);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the colored blob omnidirectional camera rotzonly sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraRotZOnlySensor::Update() {
      if(m_bEnabled) {
         /* Increase data counter */
         ++m_sReadings.Counter;
         /* Calculate range on the ground */
         CVector3 cCameraPos = m_pcOmnicamEntity->GetOffset();
         cCameraPos += m_pcEmbodiedEntity->GetOriginAnchor().Position;
         Real fGroundHalfRange = cCameraPos.GetZ() * Tan(m_pcOmnicamEntity->GetAperture());
         /* Prepare the operation */
         m_pcOperation->Setup(fGroundHalfRange);
         /* Go through LED entities in box range */
         m_pcLEDIndex->ForEntitiesInBoxRange(
            CVector3(cCameraPos.GetX(),
                     cCameraPos.GetY(),
                     cCameraPos.GetZ() * 0.5f),
            CVector3(fGroundHalfRange, fGroundHalfRange, cCameraPos.GetZ() * 0.5f),
            *m_pcOperation);
      }
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraRotZOnlySensor::Reset() {
      m_sReadings.Counter = 0;
      m_sReadings.BlobList.clear();
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraRotZOnlySensor::Destroy() {
      delete m_pcOperation;
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraRotZOnlySensor::Enable() {
      m_pcOmnicamEntity->Enable();
      m_bEnabled = true;
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraRotZOnlySensor::Disable() {
      m_pcOmnicamEntity->Disable();
      m_bEnabled = false;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CColoredBlobOmnidirectionalCameraRotZOnlySensor,
                   "colored_blob_omnidirectional_camera", "rot_z_only",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",

                   "A generic omnidirectional camera sensor to detect colored blobs.",
                   "This sensor accesses an omnidirectional camera that detects colored blobs. The\n"
                   "sensor returns a list of blobs, each defined by a color and a position with\n"
                   "respect to the robot reference point on the ground. In controllers, you must\n"
                   "include the ci_colored_blob_omnidirectional_camera_sensor.h header.\n\n"

                   "REQUIRED XML CONFIGURATION\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <colored_blob_omnidirectional_camera implementation=\"rot_z_only\"\n"
                   "                                             medium=\"leds\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "The 'medium' attribute must be set to the id of the leds medium declared in the\n"
                   "<media> section.\n\n"

                   "OPTIONAL XML CONFIGURATION\n\n"

                   "It is possible to draw the rays shot by the camera sensor in the OpenGL\n"
                   "visualization. This can be useful for sensor debugging but also to understand\n"
                   "what's wrong in your controller. In OpenGL, the rays are drawn in cyan when\n"
                   "they are not obstructed and in purple when they are. In case a ray is\n"
                   "obstructed, a black dot is drawn where the intersection occurred.\n"
                   "To turn this functionality on, add the attribute \"show_rays\" as in this\n"
                   "example:\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <colored_blob_omnidirectional_camera implementation=\"rot_z_only\"\n"
                   "                                             medium=\"leds\" />\n"
                   "                                             show_rays=\"true\" />\n"
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
                       .strDocName = "camera sensor",
                           .strXMLParent = "colored_blob_omnidirectional_camera",
                           .strXMLTag = "noise",
                           .strSAAType = "sensor",
                           .bShowExamples = true}) +

                   "Each timestep the camera is enabled, a vector of randomly generated noise\n"
                   "{'model', Uniform(0, 2PI)} is added the (distance, angle) reading for each detected.\n"
                   "blob. That is, the model of noise for the distance measure for the blob is configurable,\n"
                   "and the type for the angle measure for the blob is always Uniform(0, 2PI). The color\n"
                   "of the detected blob is unaffected by noise injection.\n\n"

                   "OPTIMIZATION HINTS\n\n"

                   "1. For small swarms, enabling the sensor (and therefore causing ARGoS to\n"
                   "   update its readings each timestep) unconditionally does not impact performance too\n"
                   "   much. For large swarms, it can impact performance, and selectively\n"
                   "   enabling/disabling the sensor according to when each individual robot needs it\n"
                   "   (e.g., only when it is looking for an LED equipped entity) can increase performance\n"
                   "   by only requiring ARGoS to update the readings on timesteps they will be used.\n",

                   "Usable"
		  );

}
