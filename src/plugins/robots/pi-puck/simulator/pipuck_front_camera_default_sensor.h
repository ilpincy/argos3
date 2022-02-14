/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_front_camera_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_FRONT_CAMERA_DEFAULT_SENSOR_H
#define PIPUCK_FRONT_CAMERA_DEFAULT_SENSOR_H

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
#include <argos3/plugins/simulator/entities/directional_led_entity.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_front_camera_sensor.h>

#include <vector>
#include <array>

namespace argos {

   class CPiPuckFrontCameraDefaultSensor : public CSimulatedSensor,
                                           public CCI_PiPuckFrontCameraSensor,
                                           public CPositionalIndex<CTagEntity>::COperation,
                                           public CPositionalIndex<CDirectionalLEDEntity>::COperation {

   public:

      /* represents LEDs inside a frustum */
      struct SLed {
         SLed(const CColor& c_color,
              const CVector3& c_position,
              const CVector2& c_center) :
            Color(c_color),
            Position(c_position),
            Center(c_center) {}
         CColor Color;
         CVector3 Position;
         CVector2 Center;
      };

   public:

      CPiPuckFrontCameraDefaultSensor();

      virtual ~CPiPuckFrontCameraDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual bool operator()(CTagEntity& c_tag);

      virtual bool operator()(CDirectionalLEDEntity& c_led);

      virtual ELedState DetectLed(const CVector3& c_position) const;

      virtual const STag::TVector& GetTags() const {
         return m_vecTags;
      }

      virtual Real GetTimestamp() const {
         return m_fTimestamp;
      }

   private:

      CRadians GetAngleWithCamera(const CPositionalEntity& c_entity) const;

      CVector2 ProjectOntoSensor(const CVector3& c_point) const;

      bool IsInsideFrustum(const CVector3& c_point) const;

   private:
      CControllableEntity* m_pcControllableEntity;
      CEmbodiedEntity* m_pcEmbodiedEntity;
      CPositionalIndex<CDirectionalLEDEntity>* m_pcDirectionalLEDIndex;
      CPositionalIndex<CTagEntity>* m_pcTagIndex;

      /* debugging information */
      bool m_bShowFrustum;
      bool m_bShowTagRays;
      bool m_bShowLEDRays;

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
      SEmbodiedEntityIntersectionItem m_sIntersectionItem;

      /* AprilTag corner offsets / ordering */
      const std::array<CVector3, 4> m_arrTagCornerOffsets = {{
         {-0.5, -0.5, 0},
         { 0.5, -0.5, 0},
         { 0.5,  0.5, 0},
         {-0.5,  0.5, 0},
      }};

      std::vector<SLed> m_vecLedCache;
      std::vector<STag> m_vecTags;
      Real m_fTimestamp;

   };
}

#endif
