#ifndef COLORED_BLOB_PERSPECTIVE_CAMERA_DEFAULT_SENSOR_H
#define COLORED_BLOB_PERSPECTIVE_CAMERA_DEFAULT_SENSOR_H

namespace argos {
   class CColoredBlobPerspectiveCameraDefaultSensor;
   class CPerspectiveCameraEquippedEntity;
   class CLEDEntity;
   class CControllableEntity;
   class CPerspectiveCameraLEDCheckOperation;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_colored_blob_perspective_camera_sensor.h>

namespace argos {

   class CColoredBlobPerspectiveCameraDefaultSensor : public CCI_ColoredBlobPerspectiveCameraSensor,
                                                      public CSimulatedSensor {

   public:

      CColoredBlobPerspectiveCameraDefaultSensor();

      virtual ~CColoredBlobPerspectiveCameraDefaultSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void Destroy();

      virtual void Enable();
      
      virtual void Disable();
			
   protected:

      bool                                 m_bEnabled;
      CPerspectiveCameraEquippedEntity*    m_pcCamEntity;
      CControllableEntity*                 m_pcControllableEntity;
      CEmbodiedEntity*                     m_pcEmbodiedEntity;
      CPositionalIndex<CLEDEntity>*        m_pcLEDIndex;
      CPositionalIndex<CEmbodiedEntity>*   m_pcEmbodiedIndex;
      CPerspectiveCameraLEDCheckOperation* m_pcOperation;
      bool                                 m_bShowRays;

   };
}

#endif
