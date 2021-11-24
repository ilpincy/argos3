/**
 * @file <argos3/plugins/robots/drone/simulator/drone_cameras_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_CAMERAS_SYSTEM_DEFAULT_SENSOR_H
#define DRONE_CAMERAS_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CEmbodiedEntity;
   class CControllableEntity;
   struct SAnchor;
}

#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/core/utility/math/matrix/transformationmatrix3.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/plane.h>

#include <argos3/plugins/simulator/entities/tag_entity.h>
#include <argos3/plugins/robots/drone/control_interface/ci_drone_cameras_system_sensor.h>

#include <vector>
#include <array>

namespace argos {

   class CDroneCamerasSystemDefaultSensor : public CSimulatedSensor,
                                            public CCI_DroneCamerasSystemSensor {
   public:

      CDroneCamerasSystemDefaultSensor();

      virtual ~CDroneCamerasSystemDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void Visit(std::function<void(SInterface&)>);

   private:

      struct SSimulatedInterface : SInterface,
                                   CPositionalIndex<CTagEntity>::COperation {
         SSimulatedInterface(const std::string& str_label,
                             const CCI_DroneCamerasSystemSensor::TConfiguration& t_configuration,
                             const SAnchor& s_anchor,
                             CDroneCamerasSystemDefaultSensor& c_parent);
                             
         ~SSimulatedInterface() {}

         virtual void Reset();

         virtual void Update();

         virtual bool operator()(CTagEntity& c_tag);

         CVector2 GetResolution() const;

         const TConfiguration& GetConfiguration() const;

      private:

         const SAnchor& Anchor;

         CDroneCamerasSystemDefaultSensor& m_cParent;

         /* camera position and orientation */
         const TConfiguration& m_tConfiguration;

         /* AprilTag corner offsets / ordering */
         // TODO make static, initialize in CPP
         const std::array<CVector3, 4> m_arrTagCornerOffsets = {{
            {-0.5, -0.5, 0},
            { 0.5, -0.5, 0},
            { 0.5,  0.5, 0},
            {-0.5,  0.5, 0},
         }};

         /* cached data for operator() */
         /* TODO: remove dependency on m_cOffset */
         CTransformationMatrix3 m_cOffset;

         CVector3 m_cCameraPosition;
         CQuaternion m_cCameraOrientation;
         CTransformationMatrix3 m_cCameraToWorldTransform;
         CSquareMatrix<3> m_cProjectionMatrix;
         std::array<CPlane, 6> m_arrFrustumPlanes;

         /* for generating the fustrum */
         Real m_fNearPlaneWidth;
         Real m_fNearPlaneHeight;
         Real m_fFarPlaneWidth;
         Real m_fFarPlaneHeight;

         /* shared buffers */
         std::array<CVector3, 4> m_arrTagCorners;
         std::array<CVector2, 4> m_arrTagCornerPixels;
         CRay3 m_cOcclusionCheckRay;
         std::vector<SEmbodiedEntityIntersectionItem> m_vecIntersections;

         /* utility methods */
         CRadians GetAngleWithCamera(const CPositionalEntity& c_entity) const;
         CVector2 ProjectOntoSensor(const CVector3& c_point) const;
         bool IsInsideFrustum(const CVector3& c_point) const;
      };

      CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      CPositionalIndex<CTagEntity>& GetTagIndex() {
         return *m_pcTagIndex;
      }

      bool ShowFustrum() const {
         return m_bShowFrustum;
      }

      bool ShowTagRays() const {
         return m_bShowTagRays;
      }

   private:
      CControllableEntity* m_pcControllableEntity;
      CEmbodiedEntity* m_pcEmbodiedEntity;
      CPositionalIndex<CTagEntity>* m_pcTagIndex;
      /* debugging information */
      bool m_bShowFrustum;
      bool m_bShowTagRays;
      std::vector<SSimulatedInterface> m_vecSimulatedInterfaces;

   };
}

#endif
