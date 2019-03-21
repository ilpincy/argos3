/**
 * @file <argos3/plugins/robots/generic/simulator/camera_sensor_algorithm.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CAMERAS_SENSOR_ALGORITHM_H
#define CAMERAS_SENSOR_ALGORITHM_H

namespace argos {
   class CCameraSensorSimulatedAlgorithm;
}

#include <argos3/core/utility/plugins/factory.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/plane.h>
#include <argos3/core/utility/math/matrix/transformationmatrix3.h>
#include <argos3/core/simulator/entity/positional_entity.h>
#include <array>

namespace argos {

   class CCameraSensorSimulatedAlgorithm {

   public:

      class CBaseUpdateOperation {
      
      public:

         CBaseUpdateOperation(const CSquareMatrix<3>& c_projection_matrix,
                              const std::array<CPlane, 6>& arr_frustum_planes,
                              const CTransformationMatrix3& c_camera_to_world_transform,
                              const CVector3& c_camera_location) :
            m_cProjectionMatrix(c_projection_matrix),
            m_arrFrustumPlanes(arr_frustum_planes),
            m_cCameraToWorldTransform(c_camera_to_world_transform),
            m_cCameraLocation(c_camera_location) {}

         virtual ~CBaseUpdateOperation() {}

         CRadians GetAngleWithCamera(const CPositionalEntity& c_entity) const {
            CVector3 cEntityToCamera(m_cCameraLocation - c_entity.GetPosition());
            CVector3 cEntityDirection(CVector3::Z);
            cEntityDirection.Rotate(c_entity.GetOrientation());
            Real fDotProduct = cEntityDirection.DotProduct(cEntityToCamera);
            return ACos(fDotProduct / (cEntityDirection.Length() * cEntityToCamera.Length()));
         }

         CVector2 ProjectOntoSensor(const CVector3& c_vector) const {
            CVector3 cCameraToEntityTranslation(m_cCameraToWorldTransform * c_vector);
            /* this could be avoided if CVector3 inherited from CMatrix<3,1> */
            CMatrix<3,1> cCameraToEntityTranslationMatrix;
            cCameraToEntityTranslationMatrix(0,0) = 
               cCameraToEntityTranslation.GetX() / cCameraToEntityTranslation.GetZ();
            cCameraToEntityTranslationMatrix(1,0) =
               cCameraToEntityTranslation.GetY() / cCameraToEntityTranslation.GetZ();
            cCameraToEntityTranslationMatrix(2,0) = 1.0f;
            /* get image coordinates */              
            CMatrix<3,1> cImageCoordinates(m_cProjectionMatrix * cCameraToEntityTranslationMatrix);
            /* return as vector2 */
            return CVector2(cImageCoordinates(0,0), cImageCoordinates(1,0));
         }

         bool IsPointInsideFrustum(const CVector3& c_point) const {
            for(const CPlane& c_plane : m_arrFrustumPlanes) {
               if(c_plane.GetNormal().DotProduct(c_point - c_plane.GetPosition()) < 0.0) {
                  return false;
               }
            }
            return true;
         }

      protected:

         const CSquareMatrix<3>& m_cProjectionMatrix;
         const std::array<CPlane, 6>& m_arrFrustumPlanes;
         const CTransformationMatrix3& m_cCameraToWorldTransform;
         const CVector3& m_cCameraLocation;
      };

   public:

      virtual ~CCameraSensorSimulatedAlgorithm() {}

      virtual void Update(const CSquareMatrix<3>& c_projection_matrix,
                          const std::array<CPlane, 6>& arr_frustum_planes,
                          const CTransformationMatrix3& c_world_to_camera_transform,
                          const CVector3& c_camera_location,
                          const CVector3& c_bounding_box_position,
                          const CVector3& c_bounding_box_half_extents) = 0;

      const std::vector<std::pair<bool, CRay3> >& GetCheckedRays() const {
         return m_vecCheckedRays;
      }
      
   protected:

      std::vector<std::pair<bool, CRay3> > m_vecCheckedRays;

   };   
}

#define REGISTER_CAMERA_SENSOR_ALGORITHM(CLASSNAME,             \
                                         LABEL,                 \
                                         AUTHOR,                \
                                         VERSION,               \
                                         BRIEF_DESCRIPTION,     \
                                         LONG_DESCRIPTION,      \
                                         STATUS)                \
   REGISTER_SYMBOL(CCameraSensorSimulatedAlgorithm,             \
                   CLASSNAME,                                   \
                   LABEL,                                       \
                   AUTHOR,                                      \
                   VERSION,                                     \
                   BRIEF_DESCRIPTION,                           \
                   LONG_DESCRIPTION,                            \
                   STATUS)

#endif
