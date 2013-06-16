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

      CLEDCheckOperation() {}
      virtual ~CLEDCheckOperation() {}

      virtual bool operator()(CLEDEntity& c_led) {
         
         return true;
      }

   private:

      

   };

   /****************************************/
   /****************************************/

   CColoredBlobOmnidirectionalCameraRotZOnlySensor::CColoredBlobOmnidirectionalCameraRotZOnlySensor() {
   }

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
      m_pcOmnicamEntity->Enable();
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
      /* Increase data counter */
      ++m_sReadings.Counter;
      /* Erase old readings */
      m_sReadings.BlobList.clear();
      /* Calculate range on the ground */
      Real fHeightRange = m_pcOmnicamEntity->GetOffset().GetZ() + m_pcEmbodiedEntity->GetPosition().GetZ();
      Real fHalfHeightRange = fHeightRange * 0.5f;
      Real fGroundHalfRange = fHeightRange * Tan(m_pcOmnicamEntity->GetAperture());
      /* Create check operation */
      CLEDCheckOperation cOperation;
      /* Go through LED entities in box range */
      m_pcLEDIndex->ForEntitiesInBoxRange(
         CVector3(m_pcOmnicamEntity->GetOffset().GetX(),
                  m_pcOmnicamEntity->GetOffset().GetY(),
                  fHalfHeightRange),
         CVector3(fGroundHalfRange, fGroundHalfRange, fHalfHeightRange),
         cOperation);
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraRotZOnlySensor::Reset() {
      m_sReadings.Counter = 0;
      m_sReadings.BlobList.clear();
   }

   /****************************************/
   /****************************************/

}
