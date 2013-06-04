#ifndef COLORED_BLOB_OMNIDIRECTIONAL_CAMERA_ROTZONLY_SENSOR_H
#define COLORED_BLOB_OMNIDIRECTIONAL_CAMERA_ROTZONLY_SENSOR_H

namespace {
   class CColoredBlobOmnidirectionalCameraRotZOnlySensor;
}

#include <argos3/plugins/generic/control_interface/ci_colored_blob_omnidirectional_camera_sensor.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace {
   class CColoredBlobOmnidirectionalCameraSensor : public CSimulatedSensor,
                                                   public CCI_ColoredBlobOmnidirectionalCameraSensor {

   public:

      CColoredBlobOmnidirectionalCameraRotZOnlySensor();

      virtual ~CColoredBlobOmnidirectionalCameraRotZOnlySensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   };
}

#endif
