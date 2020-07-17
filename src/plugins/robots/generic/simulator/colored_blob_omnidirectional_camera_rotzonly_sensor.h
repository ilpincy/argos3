#ifndef COLORED_BLOB_OMNIDIRECTIONAL_CAMERA_ROTZONLY_SENSOR_H
#define COLORED_BLOB_OMNIDIRECTIONAL_CAMERA_ROTZONLY_SENSOR_H

namespace argos {
   class CColoredBlobOmnidirectionalCameraRotZOnlySensor;
   class COmnidirectionalCameraEquippedEntity;
   class CLEDEntity;
   class CControllableEntity;
   class COmnidirectionalCameraLEDCheckOperation;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_colored_blob_omnidirectional_camera_sensor.h>
#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

  class CColoredBlobOmnidirectionalCameraRotZOnlySensor : public CCI_ColoredBlobOmnidirectionalCameraSensor,
                                                          public CSimulatedSensor {

   public:

      CColoredBlobOmnidirectionalCameraRotZOnlySensor();

      virtual ~CColoredBlobOmnidirectionalCameraRotZOnlySensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void Destroy();

      virtual void Enable();

      virtual void Disable();

    /**
       * Returns true if the rays must be shown in the GUI.
       * @return true if the rays must be shown in the GUI.
       */
      inline bool IsShowRays() {
         return m_bShowRays;
      }

      /**
       * Sets whether or not the rays must be shown in the GUI.
       * @param b_show_rays true if the rays must be shown, false otherwise
       */
      inline void SetShowRays(bool b_show_rays) {
         m_bShowRays = b_show_rays;
      }

   protected:

      bool                                     m_bEnabled;
      COmnidirectionalCameraEquippedEntity*    m_pcOmnicamEntity;
      CControllableEntity*                     m_pcControllableEntity;
      CEmbodiedEntity*                         m_pcEmbodiedEntity;
      CPositionalIndex<CLEDEntity>*            m_pcLEDIndex;
      CPositionalIndex<CEmbodiedEntity>*       m_pcEmbodiedIndex;
      COmnidirectionalCameraLEDCheckOperation* m_pcOperation;
      bool                                     m_bShowRays;

      /* Noise injectors */
    CNoiseInjector m_cDistanceNoiseInjector;
    CNoiseInjector m_cAzimuthNoiseInjector;
   };
}

#endif
