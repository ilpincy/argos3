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

   class CLEDCheckOperation : public CPositionalIndex<CLEDEntity>::COperation {

   public:

      CLEDCheckOperation(CCI_ColoredBlobOmnidirectionalCameraSensor::TBlobList& t_blobs,
                         CEmbodiedEntity& c_embodied_entity) :
         m_tBlobs(t_blobs),
         m_cEmbodiedEntity(c_embodied_entity) {}
      virtual ~CLEDCheckOperation() {}

      virtual bool operator()(CLEDEntity& c_led) {
         if(c_led.GetColor() != CColor::BLACK) {
            m_cLEDRelativePos = c_led.GetPosition();
            m_cLEDRelativePos -= m_cEmbodiedEntity.GetPosition();
            m_cLEDRelativePosXY.Set(m_cLEDRelativePos.GetX(),
                                    m_cLEDRelativePos.GetY());
            m_tBlobs.push_back(new CCI_ColoredBlobOmnidirectionalCameraSensor::SBlob(c_led.GetColor(),
                                                                                     m_cLEDRelativePosXY.Angle(),
                                                                                     m_cLEDRelativePosXY.Length()));
         }
         return true;
      }

   private:

      CCI_ColoredBlobOmnidirectionalCameraSensor::TBlobList& m_tBlobs;
      CEmbodiedEntity& m_cEmbodiedEntity;
      CVector3 m_cLEDRelativePos;
      CVector2 m_cLEDRelativePosXY;
      CRadians m_cLEDRelativeAngle;

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
      m_fDistanceNoiseStdDev(0.0f),
      m_pcRNG(NULL),
      m_bShowRays(false) {}

   /****************************************/
   /****************************************/

   CColoredBlobOmnidirectionalCameraRotZOnlySensor::~CColoredBlobOmnidirectionalCameraRotZOnlySensor() {
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraRotZOnlySensor::SetRobot(CComposableEntity& c_entity) {
      /* Get and enable omndirectional camera equipped entity */
      m_pcOmnicamEntity = &(c_entity.GetComponent<COmnidirectionalCameraEquippedEntity>("omnidirectional_camera"));
      m_pcOmnicamEntity->SetCanBeEnabledIfDisabled(true);
      /* Get controllable entity */
      m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
      /* Get embodied entity */
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      /* Get embodied entity index */
      m_pcEmbodiedIndex = &(CSimulator::GetInstance().GetSpace().GetEmbodiedEntityIndex());
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraRotZOnlySensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_ColoredBlobOmnidirectionalCameraSensor::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Parse noise */
         GetNodeAttributeOrDefault(t_tree, "noise_std_dev", m_fDistanceNoiseStdDev, m_fDistanceNoiseStdDev);
         if(m_fDistanceNoiseStdDev > 0.0f) {
            m_pcRNG = CRandom::CreateRNG("argos");
         }
         /* Get LED medium from id specified in the XML */
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcLEDIndex = &(CSimulator::GetInstance().GetMedium<CLEDMedium>(strMedium).GetIndex());
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
         /* Erase old readings */
         m_sReadings.BlobList.clear();
         /* Calculate range on the ground */
         Real fHeightRange = m_pcOmnicamEntity->GetOffset().GetZ() + m_pcEmbodiedEntity->GetPosition().GetZ();
         Real fHalfHeightRange = fHeightRange * 0.5f;
         Real fGroundHalfRange = fHeightRange * Tan(m_pcOmnicamEntity->GetAperture());
         /* Create check operation */
         CLEDCheckOperation cOperation(m_sReadings.BlobList,
                                       *m_pcEmbodiedEntity);
         /* Go through LED entities in box range */
         m_pcLEDIndex->ForEntitiesInBoxRange(
            CVector3(m_pcOmnicamEntity->GetOffset().GetX(),
                     m_pcOmnicamEntity->GetOffset().GetY(),
                     fHalfHeightRange),
            CVector3(fGroundHalfRange, fGroundHalfRange, fHalfHeightRange),
            cOperation);
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
                   "A generic omnidirectional camera sensor to detect colored blobs",
                   "TODO\n\n",
                   "Usable"
		  );

}
