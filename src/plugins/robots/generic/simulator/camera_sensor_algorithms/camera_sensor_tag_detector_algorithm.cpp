/**
 * @file <argos3/plugins/robots/generic/simulator/camera_sensor_algorithms/camera_sensor_tag_detector_algorithm.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "camera_sensor_tag_detector_algorithm.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/math/matrix/squarematrix.h>
#include <argos3/core/utility/math/matrix/transformationmatrix3.h>

#include <argos3/plugins/simulator/media/tag_medium.h>
#include <argos3/plugins/simulator/entities/tag_entity.h>

namespace argos {

   /****************************************/
   /****************************************/   

   CCameraSensorTagDetectorAlgorithm::CCameraSensorTagDetectorAlgorithm() :
      m_bShowRays(false),
      m_pcTagIndex(nullptr) {}

   /****************************************/
   /****************************************/   
   
   void CCameraSensorTagDetectorAlgorithm::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_CameraSensorAlgorithm::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Get tag medium from id specified in the XML */
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcTagIndex = &(CSimulator::GetInstance().GetMedium<CTagMedium>(strMedium).GetIndex());
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the tag detector algorithm", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CCameraSensorTagDetectorAlgorithm::Update(const CSquareMatrix<3>& c_projection_matrix,
                                                  const std::array<CPlane, 6>& arr_frustum_planes,
                                                  const CTransformationMatrix3& c_camera_to_world_transform,
                                                  const CVector3& c_camera_location,
                                                  const CVector3& c_bounding_box_position,
                                                  const CVector3& c_bounding_box_half_extents) {
      /* Define a class for the update operation */
      CUpdateOperation cUpdateOperation(c_projection_matrix, arr_frustum_planes,
                                        c_camera_to_world_transform, c_camera_location,
                                        *this);
      /* Clear out readings from last update */
      m_vecReadings.clear();
      /* Clear out checked rays from last update */
      m_vecCheckedRays.clear();
      /* Run the operation */
      m_pcTagIndex->ForEntitiesInBoxRange(c_bounding_box_position,
                                          c_bounding_box_half_extents,
                                          cUpdateOperation);
   }

   /****************************************/
   /****************************************/

   REGISTER_CAMERA_SENSOR_ALGORITHM(CCameraSensorTagDetectorAlgorithm,
                                    "tag_detector",
                                    "Michael Allwright [allsey87@gmail.com]",
                                    "1.0",
                                    "This algorithm detects nearby tags seen by the camera and\n"
                                    "returns the coordinates of their corners to the sensor",
                                    "This algorithm detects nearby tags seen by the camera and\n"
                                    "returns the coordinates of their corners to the sensor",
                                    "Under development");
}
