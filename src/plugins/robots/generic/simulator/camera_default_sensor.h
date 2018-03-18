/**
 * @file <argos3/plugins/robots/generic/simulator/camera_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CAMERAS_DEFAULT_SENSOR_H
#define CAMERAS_DEFAULT_SENSOR_H

namespace argos {
   class CCameraDefaultSensor;
   class CEmbodiedEntity;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_camera_sensor.h>
#include <argos3/plugins/robots/generic/simulator/camera_sensor_algorithm.h>

namespace argos {

   class CCameraDefaultSensor : public CSimulatedSensor,
                                public CCI_CameraSensor {

   public:
      struct SSensor {
         SAnchor& Anchor;
         CTransformationMatrix3 Offset;
         CRange<Real> Range;
         CSquareMatrix<3> ProjectionMatrix;
         std::vector<CCameraSensorSimulatedAlgorithm*> Algorithms;
         Real NearPlaneWidth, NearPlaneHeight;
         Real FarPlaneWidth, FarPlaneHeight;
         /* constructor */
         SSensor(SAnchor& s_anchor,
                 const CTransformationMatrix3& c_offset,
                 const CRange<Real>& c_range,
                 const CSquareMatrix<3>& c_projection_matrix,
                 const CVector2& c_resolution,
                 const std::vector<CCameraSensorSimulatedAlgorithm*>& vec_algorithms) :
            Anchor(s_anchor),
            Offset(c_offset),
            Range(c_range),
            ProjectionMatrix(c_projection_matrix),
            Algorithms(vec_algorithms) {
            Real fWidthToDepthRatio = (0.5 * c_resolution.GetX()) / c_projection_matrix(0,0);
            Real fHeightToDepthRatio = (0.5 * c_resolution.GetY()) / c_projection_matrix(1,1);
            NearPlaneHeight = fHeightToDepthRatio * c_range.GetMin();
            NearPlaneWidth = fWidthToDepthRatio * c_range.GetMin();
            FarPlaneHeight = fHeightToDepthRatio * c_range.GetMax();
            FarPlaneWidth = fWidthToDepthRatio * c_range.GetMax();
         }
      };

   public:

      CCameraDefaultSensor();

      virtual ~CCameraDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

   protected:
      bool m_bShowFrustum;
      CEmbodiedEntity* m_pcEmbodiedEntity;
      CControllableEntity* m_pcControllableEntity;
      std::vector<SSensor> m_vecSensors;
   };
}

#endif
