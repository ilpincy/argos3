/**
 * @file <argos3/plugins/robots/generic/simulator/camera_sensor_algorithms/camera_sensor_tag_detector_algorithm.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CAMERA_SENSOR_TAG_DETECTOR_ALGORITHM_H
#define CAMERA_SENSOR_TAG_DETECTOR_ALGORITHM_H

namespace argos {
	class CCameraSensorTagDetectorAlgorithm;
}

#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/utility/math/matrix/transformationmatrix3.h>

#include <argos3/plugins/simulator/entities/tag_entity.h>
#include <argos3/plugins/robots/generic/simulator/camera_sensor_algorithm.h>
#include <argos3/plugins/robots/generic/control_interface/ci_camera_sensor_algorithms/ci_camera_sensor_tag_detector_algorithm.h>

namespace argos {
   
   /**
    * This class provides the most general interface to a camera.
    * The camera sensor enables the user to extract information from the images
    * acquired by the simulated or by the physical camera.
    */
   class CCameraSensorTagDetectorAlgorithm : public CCameraSensorSimulatedAlgorithm,
                                             public CCI_CameraSensorTagDetectorAlgorithm {
      
   public:
      class CUpdateOperation : public CBaseUpdateOperation,
                               public CPositionalIndex<CTagEntity>::COperation {

      public:
         /* constructor */
         CUpdateOperation(const CSquareMatrix<3>& c_projection_matrix,
                          const std::array<CPlane, 6>& arr_frustum_planes,
                          const CTransformationMatrix3& c_camera_to_world_transform,
                          const CVector3& c_camera_location,
                          CCameraSensorTagDetectorAlgorithm& c_algorithm) :
            CBaseUpdateOperation(c_projection_matrix,
                                 arr_frustum_planes,
                                 c_camera_to_world_transform,
                                 c_camera_location),
            m_cAlgorithm(c_algorithm) {
            m_cOcclusionCheckRay.SetStart(c_camera_location);
         }
         /* destructor */
         virtual ~CUpdateOperation() {}
         /* operation */
         virtual bool operator()(CTagEntity& c_tag) {
            if(GetAngleWithCamera(c_tag) > c_tag.GetObservableAngle()) {
               return true;
            }
            std::transform(std::begin(m_arrTagCornerOffsets),
                           std::end(m_arrTagCornerOffsets),
                           std::begin(m_arrTagCorners),
                           [&c_tag] (const CVector3& c_tag_corner_offset) {
               CVector3 cCorner(c_tag_corner_offset * c_tag.GetSideLength());
               cCorner.Rotate(c_tag.GetOrientation());
               return (cCorner + c_tag.GetPosition());
            });
            for(const CVector3& c_corner : m_arrTagCorners) {
               if(IsPointInsideFrustum(c_corner) == false) {
                  /* corner is not inside the frustum */
                  return true;
               }
            }
            for(const CVector3& c_corner : m_arrTagCorners) {
               m_cOcclusionCheckRay.SetEnd(c_corner);
               if(GetClosestEmbodiedEntityIntersectedByRay(m_sIntersectionItem, m_cOcclusionCheckRay)) {
                  /* corner is occluded */
                  m_cAlgorithm.AddCheckedRay(true, m_cOcclusionCheckRay);
                  return true;
               }
               else {
                  m_cAlgorithm.AddCheckedRay(false, m_cOcclusionCheckRay);
               }
            }
            std::transform(std::begin(m_arrTagCorners),
                           std::end(m_arrTagCorners),
                           std::begin(m_arrTagCornerPixels),
                           [this] (const CVector3& c_tag_corner) {
               return ProjectOntoSensor(c_tag_corner);
            });
            const CVector2& cCenterPixel = ProjectOntoSensor(c_tag.GetPosition());
            const std::string& strPayload = c_tag.GetPayload();
            m_cAlgorithm.AddReading(strPayload, cCenterPixel, m_arrTagCornerPixels);
            return true;
         }

      private:
         /* This order is consistent with the AprilTags algorithm */
         const std::array<CVector3, 4> m_arrTagCornerOffsets = {{
            {-0.5, -0.5, 0},
            {-0.5,  0.5, 0},
            { 0.5,  0.5, 0},
            { 0.5, -0.5, 0},
         }};
         std::array<CVector3, 4> m_arrTagCorners;
         std::array<CVector2, 4> m_arrTagCornerPixels;
         CRay3 m_cOcclusionCheckRay;
         SEmbodiedEntityIntersectionItem m_sIntersectionItem;
         CCameraSensorTagDetectorAlgorithm& m_cAlgorithm;
      };

   public:

      CCameraSensorTagDetectorAlgorithm();

      virtual ~CCameraSensorTagDetectorAlgorithm() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update(const CSquareMatrix<3>& c_projection_matrix,
                          const std::array<CPlane, 6>& arr_frustum_planes,
                          const CTransformationMatrix3& c_camera_to_world_transform,
                          const CVector3& c_camera_location,
                          const CVector3& c_bounding_box_position,
                          const CVector3& c_bounding_box_half_extents);

      void AddCheckedRay(bool b_intersected, const CRay3& c_ray) {
         if(m_bShowRays) {
            m_vecCheckedRays.emplace_back(b_intersected, c_ray);
         }
      }

      void AddReading(const std::string& str_payload,
                      const CVector2& c_center_pixel,
                      const std::array<CVector2, 4>& arr_corner_pixels) {
         m_vecReadings.emplace_back(str_payload, c_center_pixel, arr_corner_pixels);
      }

   private:
      bool                           m_bShowRays;
      CPositionalIndex<CTagEntity>*  m_pcTagIndex;
   };
}         

#endif
