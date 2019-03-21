/**
 * @file <argos3/plugins/robots/generic/simulator/camera_sensor_algorithms/camera_sensor_directional_led_detector_algorithm.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CAMERA_SENSOR_DIRECTIONAL_LED_DETECTOR_ALGORITHM_H
#define CAMERA_SENSOR_DIRECTIONAL_LED_DETECTOR_ALGORITHM_H

namespace argos {
	class CCameraSensorDirectionalLEDDetectorAlgorithm;
}

#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/utility/math/matrix/transformationmatrix3.h>

#include <argos3/plugins/simulator/entities/directional_led_entity.h>
#include <argos3/plugins/robots/generic/simulator/camera_sensor_algorithm.h>
#include <argos3/plugins/robots/generic/control_interface/ci_camera_sensor_algorithms/ci_camera_sensor_directional_led_detector_algorithm.h>

namespace argos {
   
   /**
    * This class provides the most general interface to a camera.
    * The camera sensor enables the user to extract information from the images
    * acquired by the simulated or by the physical camera.
    */
   class CCameraSensorDirectionalLEDDetectorAlgorithm : public CCameraSensorSimulatedAlgorithm,
                                                        public CCI_CameraSensorDirectionalLEDDetectorAlgorithm {
      
   public:
      class CUpdateOperation : public CBaseUpdateOperation,
                               public CPositionalIndex<CDirectionalLEDEntity>::COperation {

      public:
         /* constructor */
         CUpdateOperation(const CSquareMatrix<3>& c_projection_matrix,
                          const std::array<CPlane, 6>& arr_frustum_planes,
                          const CTransformationMatrix3& c_camera_to_world_transform,
                          const CVector3& c_camera_location,
                          CCameraSensorDirectionalLEDDetectorAlgorithm& c_algorithm) :
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
         virtual bool operator()(CDirectionalLEDEntity& c_led) {
            if(c_led.GetColor() == CColor::BLACK) {
               return true;
            }
            if(GetAngleWithCamera(c_led) > c_led.GetObservableAngle()) {
               return true;
            }
            const CVector3& cLedPosition = c_led.GetPosition();
            if(IsPointInsideFrustum(cLedPosition) == false) {
               return true;
            }
            m_cOcclusionCheckRay.SetEnd(cLedPosition);
            if(GetClosestEmbodiedEntityIntersectedByRay(m_sIntersectionItem, m_cOcclusionCheckRay)) {
               m_cAlgorithm.AddCheckedRay(true, m_cOcclusionCheckRay);
               return true;
            }
            m_cAlgorithm.AddCheckedRay(false, m_cOcclusionCheckRay);
            m_cAlgorithm.AddReading(c_led.GetColor(), ProjectOntoSensor(cLedPosition));
            return true;
         }

      private:
         CRay3 m_cOcclusionCheckRay;
         SEmbodiedEntityIntersectionItem m_sIntersectionItem;
         CCameraSensorDirectionalLEDDetectorAlgorithm& m_cAlgorithm;
      };

   public:

      CCameraSensorDirectionalLEDDetectorAlgorithm();

      virtual ~CCameraSensorDirectionalLEDDetectorAlgorithm() {}

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

      void AddReading(const CColor& c_color, const CVector2& c_center) {
         m_vecReadings.emplace_back(c_color, c_center);
      }

   private:
      bool                                      m_bShowRays;
      CPositionalIndex<CDirectionalLEDEntity>*  m_pcLEDIndex;
   };
}         

#endif
