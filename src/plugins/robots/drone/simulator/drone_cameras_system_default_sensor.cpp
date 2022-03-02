/**
 * @file <argos3/plugins/robots/drone/simulator/drone_cameras_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_cameras_system_default_sensor.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>

#include <argos3/plugins/simulator/media/tag_medium.h>

#define CAMERA_RANGE_MIN 0.010
#define CAMERA_RANGE_MAX 2.500

namespace argos {

   /****************************************/
   /****************************************/

   CDroneCamerasSystemDefaultSensor::CDroneCamerasSystemDefaultSensor() :
      m_pcControllableEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcTagIndex(nullptr),
      m_bShowFrustum(false),
      m_bShowTagRays(false) {}

   /****************************************/
   /****************************************/

   void CDroneCamerasSystemDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
         /* get required components */
         m_pcControllableEntity =
            &(c_entity.GetComponent<CControllableEntity>("controller"));
         m_pcEmbodiedEntity =
            &(c_entity.GetComponent<CEmbodiedEntity>("body"));
         /* allocate memory for the sensor interfaces */
         m_vecSimulatedInterfaces.reserve(DEFAULT_SENSOR_CONFIGURATION.size());
         /* get the anchors for the sensor interfaces from m_mapSensorConfig */
         for(const std::pair<const std::string, TConfiguration>& t_config : DEFAULT_SENSOR_CONFIGURATION) {
            const char* pchAnchor = std::get<const char*>(t_config.second);
            const SAnchor& sAnchor =
               c_entity.GetComponent<CEmbodiedEntity>("body").GetAnchor(pchAnchor);
            m_vecSimulatedInterfaces.emplace_back(t_config.first, t_config.second, sAnchor, *this);
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error setting robot for the drone cameras system sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDroneCamerasSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* initialize the parent class */
         CCI_DroneCamerasSystemSensor::Init(t_tree);
         /* show the frustum and detection rays */
         GetNodeAttributeOrDefault(t_tree, "show_frustum", m_bShowFrustum, m_bShowFrustum);
         GetNodeAttributeOrDefault(t_tree, "show_tag_rays", m_bShowTagRays, m_bShowTagRays);
         /* get index */
         std::string strTagMedium;
         GetNodeAttribute(t_tree, "tag_medium", strTagMedium);
         m_pcTagIndex =
            &(CSimulator::GetInstance().GetMedium<CTagMedium>(strTagMedium).GetIndex());
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the drone cameras system sensor", ex);
      }
      Update();
   }

   /****************************************/
   /****************************************/

   void CDroneCamerasSystemDefaultSensor::Update() {
      /* update each camera */
      for(SSimulatedInterface& s_simulated_interface : m_vecSimulatedInterfaces) {
         s_simulated_interface.Update();        
      }
   }

   /****************************************/
   /****************************************/

   void CDroneCamerasSystemDefaultSensor::Reset() {
      /* reset the cameras */
      for(SSimulatedInterface& s_simulated_interface : m_vecSimulatedInterfaces) {
         s_simulated_interface.Reset();
      }
      /* reset the base class */
      CCI_DroneCamerasSystemSensor::Reset();
   }

   /****************************************/
   /****************************************/

   void CDroneCamerasSystemDefaultSensor::Visit(std::function<void(SInterface&)> fn_visitor) {
      for(SSimulatedInterface& s_interface : m_vecSimulatedInterfaces) {
         fn_visitor(s_interface);
      }
   }

   /****************************************/
   /****************************************/

   CDroneCamerasSystemDefaultSensor::SSimulatedInterface::
      SSimulatedInterface(const std::string& str_label,
                          const CCI_DroneCamerasSystemSensor::TConfiguration& t_configuration,
                          const SAnchor& s_anchor,
                          CDroneCamerasSystemDefaultSensor& c_parent) :
      SInterface(str_label),
      Anchor(s_anchor),
      m_cParent(c_parent),
      m_tConfiguration(t_configuration) {
      /* set up the project matrix */
      m_cProjectionMatrix.SetIdentityMatrix();
      m_cProjectionMatrix(0,0) = DEFAULT_CAMERA_FOCAL_LENGTH_X;
      m_cProjectionMatrix(1,1) = DEFAULT_CAMERA_FOCAL_LENGTH_Y;
      m_cProjectionMatrix(0,2) = DEFAULT_CAMERA_PRINCIPAL_POINT_X;
      m_cProjectionMatrix(1,2) = DEFAULT_CAMERA_PRINCIPAL_POINT_Y;
      /* calculate fustrum constants */
      Real fWidthToDepthRatio = (0.5 * DEFAULT_CAMERA_RESOLUTION_X) / DEFAULT_CAMERA_FOCAL_LENGTH_X;
      Real fHeightToDepthRatio = (0.5 * DEFAULT_CAMERA_RESOLUTION_Y) / DEFAULT_CAMERA_FOCAL_LENGTH_Y;
      m_fNearPlaneHeight = fHeightToDepthRatio * CAMERA_RANGE_MIN;
      m_fNearPlaneWidth = fWidthToDepthRatio * CAMERA_RANGE_MIN;
      m_fFarPlaneHeight = fHeightToDepthRatio * CAMERA_RANGE_MAX;
      m_fFarPlaneWidth = fWidthToDepthRatio * CAMERA_RANGE_MAX;
      /* transformation matrix */
      m_cOffset.SetFromComponents(std::get<CQuaternion>(m_tConfiguration),
                                  std::get<CVector3>(m_tConfiguration));
   }

   /****************************************/
   /****************************************/

   void CDroneCamerasSystemDefaultSensor::
      SSimulatedInterface::Reset() {
      Tags.clear();
   }

   /****************************************/
   /****************************************/

   void CDroneCamerasSystemDefaultSensor::
      SSimulatedInterface::Update() {
      /* increment the interface timestamp */
      Timestamp += CPhysicsEngine::GetSimulationClockTick();
      /* clear out the readings from the last update */
      Tags.clear();
      /* if the sensor is enabled */
      if(Enabled) {
         /* TODO move these sensor parameters to class wide to avoid recalling constructors */
         CTransformationMatrix3 cWorldToAnchorTransform;
         CTransformationMatrix3 cWorldToCameraTransform;
         CVector3 cLookAt, cUp;
         CVector3 cX, cY, cZ;
         CVector3 cFarCenter, cNearCenter;
         std::array<CVector3, 8> arrFrustumExtents;
         CVector3& cNearTopLeft = arrFrustumExtents[0];
         CVector3& cNearTopRight = arrFrustumExtents[1];
         CVector3& cNearBottomLeft = arrFrustumExtents[2];
         CVector3& cNearBottomRight = arrFrustumExtents[3];
         CVector3& cFarTopLeft = arrFrustumExtents[4];
         CVector3& cFarTopRight = arrFrustumExtents[5];
         CVector3& cFarBottomLeft = arrFrustumExtents[6];
         CVector3& cFarBottomRight = arrFrustumExtents[7];
         /* calculate transform matrices */
         cWorldToAnchorTransform.SetFromComponents(Anchor.Orientation, Anchor.Position);
         cWorldToCameraTransform = cWorldToAnchorTransform * m_cOffset;
         m_cCameraToWorldTransform = cWorldToCameraTransform.GetInverse();
         /* calculate camera direction vectors */
         m_cCameraPosition = cWorldToCameraTransform.GetTranslationVector();
         m_cCameraOrientation = Anchor.Orientation * std::get<CQuaternion>(m_tConfiguration);
         cLookAt = cWorldToCameraTransform * CVector3::Z;
         cUp = -CVector3::Y;
         cUp.Rotate(cWorldToCameraTransform.GetRotationMatrix());
         /* calculate direction vectors */
         cZ = m_cCameraPosition - cLookAt;
         cZ.Normalize();
         cX = cUp;
         cX.CrossProduct(cZ);
         cX.Normalize();
         cY = cZ;
         cY.CrossProduct(cX);
         /* calculate frustum coordinates */
         cNearCenter = m_cCameraPosition - cZ * CAMERA_RANGE_MIN;
         cFarCenter = m_cCameraPosition - cZ * CAMERA_RANGE_MAX;
         cNearTopLeft = cNearCenter + (cY * m_fNearPlaneHeight) - (cX * m_fNearPlaneWidth);
         cNearTopRight = cNearCenter + (cY * m_fNearPlaneHeight) + (cX * m_fNearPlaneWidth);
         cNearBottomLeft = cNearCenter - (cY * m_fNearPlaneHeight) - (cX * m_fNearPlaneWidth);
         cNearBottomRight = cNearCenter - (cY * m_fNearPlaneHeight) + (cX * m_fNearPlaneWidth);
         cFarTopLeft = cFarCenter + (cY * m_fFarPlaneHeight) - (cX * m_fFarPlaneWidth);
         cFarTopRight = cFarCenter + (cY * m_fFarPlaneHeight) + (cX * m_fFarPlaneWidth);
         cFarBottomLeft = cFarCenter - (cY * m_fFarPlaneHeight) - (cX * m_fFarPlaneWidth);
         cFarBottomRight = cFarCenter - (cY * m_fFarPlaneHeight) + (cX * m_fFarPlaneWidth);
         /* show frustum if enabled by adding outline to the checked rays vector */
         if(m_cParent.ShowFustrum()) {
            std::vector<std::pair<bool, CRay3> >& vecCheckedRays =
               m_cParent.GetControllableEntity().GetCheckedRays();
            vecCheckedRays.emplace_back(false, CRay3(cNearTopLeft, cNearTopRight));
            vecCheckedRays.emplace_back(false, CRay3(cNearTopRight, cNearBottomRight));
            vecCheckedRays.emplace_back(false, CRay3(cNearBottomRight, cNearBottomLeft));
            vecCheckedRays.emplace_back(false, CRay3(cNearBottomLeft, cNearTopLeft));
            vecCheckedRays.emplace_back(false, CRay3(cFarTopLeft, cFarTopRight));
            vecCheckedRays.emplace_back(false, CRay3(cFarTopRight, cFarBottomRight));
            vecCheckedRays.emplace_back(false, CRay3(cFarBottomRight, cFarBottomLeft));
            vecCheckedRays.emplace_back(false, CRay3(cFarBottomLeft, cFarTopLeft));
            vecCheckedRays.emplace_back(false, CRay3(cNearTopLeft, cFarTopLeft));
            vecCheckedRays.emplace_back(false, CRay3(cNearTopRight, cFarTopRight));
            vecCheckedRays.emplace_back(false, CRay3(cNearBottomRight, cFarBottomRight));
            vecCheckedRays.emplace_back(false, CRay3(cNearBottomLeft, cFarBottomLeft));
         }
         /* generate a bounding box for the frustum */
         CVector3 cBoundingBoxMinCorner(cNearCenter);
         CVector3 cBoundingBoxMaxCorner(cNearCenter);
         for(const CVector3& c_point : arrFrustumExtents) {
            if(c_point.GetX() > cBoundingBoxMaxCorner.GetX()) cBoundingBoxMaxCorner.SetX(c_point.GetX());
            if(c_point.GetX() < cBoundingBoxMinCorner.GetX()) cBoundingBoxMinCorner.SetX(c_point.GetX());
            if(c_point.GetY() > cBoundingBoxMaxCorner.GetY()) cBoundingBoxMaxCorner.SetY(c_point.GetY());
            if(c_point.GetY() < cBoundingBoxMinCorner.GetY()) cBoundingBoxMinCorner.SetY(c_point.GetY());
            if(c_point.GetZ() > cBoundingBoxMaxCorner.GetZ()) cBoundingBoxMaxCorner.SetZ(c_point.GetZ());
            if(c_point.GetZ() < cBoundingBoxMinCorner.GetZ()) cBoundingBoxMinCorner.SetZ(c_point.GetZ());
         }
         cBoundingBoxMaxCorner *= 0.5f;
         cBoundingBoxMinCorner *= 0.5f;
         CVector3 cBoundingBoxPosition(cBoundingBoxMaxCorner + cBoundingBoxMinCorner);
         CVector3 cBoundingBoxHalfExtents(cBoundingBoxMaxCorner - cBoundingBoxMinCorner);
         /* generate frustum planes */
         m_arrFrustumPlanes[0].SetFromThreePoints(cNearTopRight, cNearTopLeft, cFarTopLeft);
         m_arrFrustumPlanes[1].SetFromThreePoints(cNearBottomLeft, cNearBottomRight, cFarBottomRight);
         m_arrFrustumPlanes[2].SetFromThreePoints(cNearTopLeft, cNearBottomLeft, cFarBottomLeft);
         m_arrFrustumPlanes[3].SetFromThreePoints(cNearBottomRight, cNearTopRight, cFarBottomRight);
         m_arrFrustumPlanes[4].SetFromThreePoints(cNearTopLeft, cNearTopRight, cNearBottomRight);
         m_arrFrustumPlanes[5].SetFromThreePoints(cFarTopRight, cFarTopLeft, cFarBottomLeft);
         /* set the start of the ray for checking occlusions to the camera's location */
         m_cOcclusionCheckRay.SetStart(m_cCameraPosition);
         /* detect tags */
         m_cParent.GetTagIndex().ForEntitiesInBoxRange(cBoundingBoxPosition,
                                                       cBoundingBoxHalfExtents,
                                                       *this);
      }
   }

   /****************************************/
   /****************************************/

   bool CDroneCamerasSystemDefaultSensor::
      SSimulatedInterface::operator()(CTagEntity& c_tag) {
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
         if(IsInsideFrustum(c_corner) == false) {
            /* no more checks necessary, move on to the next tag */
            return true;
         }
      }
      for(const CVector3& c_corner : m_arrTagCorners) {
         m_cOcclusionCheckRay.SetEnd(c_corner);
         if(GetEmbodiedEntitiesIntersectedByRay(m_vecIntersections, m_cOcclusionCheckRay)) {
            CEntity& cEntityWithTag = c_tag.GetRootEntity();
            CEntity& cEntityWithCamera = m_cParent.GetControllableEntity().GetRootEntity();
            for(const SEmbodiedEntityIntersectionItem& s_item : m_vecIntersections) {
               CEntity& cIntersectionEntity = s_item.IntersectedEntity->GetRootEntity();
               if(&cIntersectionEntity != &cEntityWithTag && &cIntersectionEntity != &cEntityWithCamera) {
                  /* corner is occluded */
                  if(m_cParent.ShowTagRays()) {
                     std::vector<std::pair<bool, CRay3> >& vecCheckedRays =
                        m_cParent.GetControllableEntity().GetCheckedRays();
                     vecCheckedRays.emplace_back(true, m_cOcclusionCheckRay);
                  }
                  /* no more checks necessary, move on to the next tag */
                  return true;
               }
            }
         }
         if(m_cParent.ShowTagRays()) {
            std::vector<std::pair<bool, CRay3> >& vecCheckedRays =
               m_cParent.GetControllableEntity().GetCheckedRays();
            vecCheckedRays.emplace_back(false, m_cOcclusionCheckRay);
         }
      }
      std::transform(std::begin(m_arrTagCorners),
                     std::end(m_arrTagCorners),
                     std::begin(m_arrTagCornerPixels),
                     [this] (const CVector3& c_tag_corner) {
         return ProjectOntoSensor(c_tag_corner);
      });
      const CVector2& cCenterPixel = ProjectOntoSensor(c_tag.GetPosition());
      /* try to convert tag payload to an unsigned integer */
      UInt32 unId = 0;
      try {
         std::string strId(c_tag.GetPayload());
         auto itRemove =
            std::remove_if(std::begin(strId),
                           std::end(strId),
                           [] (char ch) {
                              return (std::isdigit(ch) == 0);
                           }); 
         strId.erase(itRemove, std::end(strId));
         unId = std::stoul(strId);
      }
      catch(const std::logic_error& err_logic) {}
      /* TODO: Update the position and orientation calculations to avoid the use of matrices */
      CVector3 cTagPosition = m_cCameraToWorldTransform * c_tag.GetPosition();
      /* Direction of the tag should be pointing inside of the tag */
      CQuaternion cTagOrientation = m_cCameraOrientation.Inverse() * c_tag.GetOrientation() * CQuaternion(CRadians::PI, CVector3::X);
      /* transfer readings to the control interface */
      Tags.emplace_back(unId, cTagPosition, cTagOrientation, cCenterPixel, m_arrTagCornerPixels);
      return true;
   }

   /****************************************/
   /****************************************/

   CVector2 CDroneCamerasSystemDefaultSensor::SSimulatedInterface::GetResolution() const {
      return CVector2(DEFAULT_CAMERA_RESOLUTION_X, DEFAULT_CAMERA_RESOLUTION_Y);
   }

   /****************************************/
   /****************************************/

   const CCI_DroneCamerasSystemSensor::TConfiguration&
      CDroneCamerasSystemDefaultSensor::SSimulatedInterface::GetConfiguration() const {
      return m_tConfiguration;
   }

   /****************************************/
   /****************************************/

   CRadians CDroneCamerasSystemDefaultSensor::
      SSimulatedInterface::GetAngleWithCamera(const CPositionalEntity& c_entity) const {
      CVector3 cEntityToCamera(m_cCameraPosition - c_entity.GetPosition());
      CVector3 cEntityDirection(CVector3::Z);
      cEntityDirection.Rotate(c_entity.GetOrientation());
      Real fDotProduct = cEntityDirection.DotProduct(cEntityToCamera);
      return ACos(fDotProduct / (cEntityDirection.Length() * cEntityToCamera.Length()));
   }

   /****************************************/
   /****************************************/

   CVector2 CDroneCamerasSystemDefaultSensor::
      SSimulatedInterface::ProjectOntoSensor(const CVector3& c_point) const {
      CVector3 cCameraToEntityTranslation(m_cCameraToWorldTransform * c_point);
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

   /****************************************/
   /****************************************/

   bool CDroneCamerasSystemDefaultSensor::
      SSimulatedInterface::IsInsideFrustum(const CVector3& c_point) const {
      for(const CPlane& c_plane : m_arrFrustumPlanes) {
         if(c_plane.GetNormal().DotProduct(c_point - c_plane.GetPosition()) < 0.0) {
            return false;
         }
      }
      return true;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CDroneCamerasSystemDefaultSensor,
                   "drone_cameras_system", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The multi-camera sensor for the drone that detects tags.",
                   "The documentation for this sensor is \e]8;;https://github.com/ilpincy/argos3/blob/master/src/plugins/robots/drone/README.md#drone_cameras_system\aavailable on Github\e]8;;\a.",
                   "Usable");
}
