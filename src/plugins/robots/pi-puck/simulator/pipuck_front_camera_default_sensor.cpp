/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_front_camera_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_front_camera_default_sensor.h"

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>

#include <argos3/plugins/simulator/media/directional_led_medium.h>
#include <argos3/plugins/simulator/media/tag_medium.h>

#define CAMERA_RANGE_MIN 0.01f
#define CAMERA_RANGE_MAX 0.5f
/* when detecting LEDs, an LED should be within 1 cm of the sampled location
   this prevents the detection of adjacent LEDs */
#define DETECT_LED_DIST_THRES 0.01f

namespace argos {

   /****************************************/
   /****************************************/

   CPiPuckFrontCameraDefaultSensor::CPiPuckFrontCameraDefaultSensor() :
      m_pcControllableEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcDirectionalLEDIndex(nullptr),
      m_pcTagIndex(nullptr),
      m_bShowFrustum(false),
      m_bShowTagRays(false),
      m_bShowLEDRays(false),
      m_fTimestamp(0.0) {}

   /****************************************/
   /****************************************/

   void CPiPuckFrontCameraDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
         m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
         m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error setting robot for the Pi-Puck front camera sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckFrontCameraDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* initialize the parent class */
         CCI_PiPuckFrontCameraSensor::Init(t_tree);
         /* show the frustum? */
         GetNodeAttributeOrDefault(t_tree, "show_frustum", m_bShowFrustum, m_bShowFrustum);
         GetNodeAttributeOrDefault(t_tree, "show_tag_rays", m_bShowTagRays, m_bShowTagRays);
         GetNodeAttributeOrDefault(t_tree, "show_led_rays", m_bShowLEDRays, m_bShowLEDRays);
         /* set up the project matrix */
         m_cProjectionMatrix.SetIdentityMatrix();
         m_cProjectionMatrix(0,0) = CCI_PiPuckFrontCameraSensor::m_cFocalLength.GetX();
         m_cProjectionMatrix(1,1) = CCI_PiPuckFrontCameraSensor::m_cFocalLength.GetY();
         m_cProjectionMatrix(0,2) = CCI_PiPuckFrontCameraSensor::m_cPrincipalPoint.GetX();
         m_cProjectionMatrix(1,2) = CCI_PiPuckFrontCameraSensor::m_cPrincipalPoint.GetY();
         /* calculate fustrum constants */
         Real fWidthToDepthRatio = (0.5 * CCI_PiPuckFrontCameraSensor::m_cResolution.GetX()) /
            CCI_PiPuckFrontCameraSensor::m_cFocalLength.GetX();
         Real fHeightToDepthRatio = (0.5 * CCI_PiPuckFrontCameraSensor::m_cResolution.GetY()) /
            CCI_PiPuckFrontCameraSensor::m_cFocalLength.GetY();
         m_fNearPlaneHeight = fHeightToDepthRatio * CAMERA_RANGE_MIN;
         m_fNearPlaneWidth = fWidthToDepthRatio * CAMERA_RANGE_MIN;
         m_fFarPlaneHeight = fHeightToDepthRatio * CAMERA_RANGE_MAX;
         m_fFarPlaneWidth = fWidthToDepthRatio * CAMERA_RANGE_MAX;
         /* transformation matrix */
         m_cOffset.SetFromComponents(
            CCI_PiPuckFrontCameraSensor::m_cOrientationOffset,
            CCI_PiPuckFrontCameraSensor::POSITION_OFFSET);
         /* get indices */
         std::string strTagMedium;
         GetNodeAttributeOrDefault(t_tree, "tag_medium", strTagMedium, strTagMedium);
         if(!strTagMedium.empty()) {
            m_pcTagIndex =
               &(CSimulator::GetInstance().GetMedium<CTagMedium>(strTagMedium).GetIndex());
         }
         std::string strDirectionalLedMedium;
         GetNodeAttributeOrDefault(t_tree, "directional_led_medium", strDirectionalLedMedium, strDirectionalLedMedium);
         if(!strDirectionalLedMedium.empty()) {
            m_pcDirectionalLEDIndex =
               &(CSimulator::GetInstance().GetMedium<CDirectionalLEDMedium>(strDirectionalLedMedium).GetIndex());
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the pipuck camera system sensor", ex);
      }
      Update();
   }

   /****************************************/
   /****************************************/

   void CPiPuckFrontCameraDefaultSensor::Update() {
      /* clear out the readings from the last update */
      m_vecTags.clear();
      m_vecLedCache.clear();
      /* increment the timestamp */
      m_fTimestamp += CPhysicsEngine::GetSimulationClockTick();
      /* if the sensor is enabled */
      if(IsEnabled()) {
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
         SAnchor& s_anchor = m_pcEmbodiedEntity->GetOriginAnchor();
         cWorldToAnchorTransform.SetFromComponents(s_anchor.Orientation, s_anchor.Position);
         cWorldToCameraTransform = cWorldToAnchorTransform * m_cOffset;
         m_cCameraToWorldTransform = cWorldToCameraTransform.GetInverse();
         /* calculate camera direction vectors */
         m_cCameraPosition = cWorldToCameraTransform.GetTranslationVector();
         m_cCameraOrientation = s_anchor.Orientation *
            CCI_PiPuckFrontCameraSensor::m_cOrientationOffset;
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
         if(m_bShowFrustum) {
            std::vector<std::pair<bool, CRay3> >& vecCheckedRays = m_pcControllableEntity->GetCheckedRays();
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
         if(m_pcTagIndex) {
            m_pcTagIndex->ForEntitiesInBoxRange(cBoundingBoxPosition,
                                                cBoundingBoxHalfExtents,
                                                *this);
         }
         /* detect directional LEDs */
         if(m_pcDirectionalLEDIndex) {
            m_pcDirectionalLEDIndex->ForEntitiesInBoxRange(cBoundingBoxPosition,
                                                         cBoundingBoxHalfExtents,
                                                         *this);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckFrontCameraDefaultSensor::Reset() {
      /* clear the LED cache */
      m_vecLedCache.clear();
      m_vecTags.clear();
      m_fTimestamp = 0.0;
      /* reset the base class */
      CCI_PiPuckFrontCameraSensor::Reset();
   }

   /****************************************/
   /****************************************/

   bool CPiPuckFrontCameraDefaultSensor::operator()(CTagEntity& c_tag) {
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
         if(GetClosestEmbodiedEntityIntersectedByRay(m_sIntersectionItem, m_cOcclusionCheckRay)) {
            /* corner is occluded */
            if(m_bShowTagRays) {
               m_pcControllableEntity->GetCheckedRays().emplace_back(true, m_cOcclusionCheckRay);
            }
            /* no more checks necessary, move on to the next tag */
            return true;
         }
         else {
            /* corner not occluded */
            if(m_bShowTagRays) {
               m_pcControllableEntity->GetCheckedRays().emplace_back(false, m_cOcclusionCheckRay);
            }
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
         unId = std::stoul(c_tag.GetPayload());
      }
      catch(const std::logic_error& err_logic) {}
      /* TODO: Update the position and orientation calculations to avoid the use of matrices */
      CVector3 cTagPosition = m_cCameraToWorldTransform * c_tag.GetPosition();
      CQuaternion cTagOrientation = m_cCameraOrientation.Inverse() * c_tag.GetOrientation();
      /* transfer readings to the control interface */
      m_vecTags.emplace_back(unId, cTagPosition, cTagOrientation, cCenterPixel, m_arrTagCornerPixels);
      return true;
   }

   /****************************************/
   /****************************************/

   bool CPiPuckFrontCameraDefaultSensor::operator()(CDirectionalLEDEntity& c_led) {
      if(c_led.GetColor() == CColor::BLACK) {
         return true;
      }
      if(GetAngleWithCamera(c_led) > c_led.GetObservableAngle()) {
         return true;
      }
      const CVector3& cLedPosition = c_led.GetPosition();
      if(IsInsideFrustum(cLedPosition) == false) {
         return true;
      }
      m_cOcclusionCheckRay.SetEnd(cLedPosition);
      if(!GetClosestEmbodiedEntityIntersectedByRay(m_sIntersectionItem, m_cOcclusionCheckRay)) {
         m_vecLedCache.emplace_back(c_led.GetColor(), cLedPosition, ProjectOntoSensor(cLedPosition));
         if(m_bShowLEDRays) {
            m_pcControllableEntity->GetCheckedRays().emplace_back(false, m_cOcclusionCheckRay);
         }
      }
      else {
         if(m_bShowLEDRays) {
            m_pcControllableEntity->GetCheckedRays().emplace_back(true, m_cOcclusionCheckRay);
         }
      }
      return true;
   }

   /****************************************/
   /****************************************/

   CPiPuckFrontCameraDefaultSensor::ELedState
      CPiPuckFrontCameraDefaultSensor::DetectLed(const CVector3& c_position) const {    
      /* c_position is the led in camera's coordinate system, 
         transfer it to global coordinate system */
      CVector3 cLedPosition(c_position);
      cLedPosition.Rotate(m_cCameraOrientation);
      cLedPosition += m_cCameraPosition;
      /* find the closest LED */
      std::vector<SLed>::const_iterator itClosestLed =
         std::min_element(std::cbegin(m_vecLedCache),
                          std::cend(m_vecLedCache),
                          [&cLedPosition] (const SLed& s_lhs_led, const SLed& s_rhs_led) {
         return (Distance(s_lhs_led.Position, cLedPosition) <
                 Distance(s_rhs_led.Position, cLedPosition));
      });
      /* if no LEDs were found or if the closest LED is more than 0.5 cm away,
         return ELedState::OFF */
      if(itClosestLed == std::end(m_vecLedCache) ||
         Distance(itClosestLed->Position, cLedPosition) > DETECT_LED_DIST_THRES) {
         return ELedState::OFF;
      }
      /* At this point, we have the closest LED, estimate its state (mapped from
         an exact color in simulation */
      if(itClosestLed->Color == CColor::MAGENTA) {
         return ELedState::Q1;
      }
      else if(itClosestLed->Color == CColor::ORANGE) {
         return ELedState::Q2;
      }
      else if(itClosestLed->Color == CColor::GREEN) {
         return ELedState::Q3;
      }
      else if(itClosestLed->Color == CColor::BLUE) {
         return ELedState::Q4;
      }
      else {
         return ELedState::OFF;
      }
   }

   /****************************************/
   /****************************************/

   CRadians CPiPuckFrontCameraDefaultSensor::GetAngleWithCamera(const CPositionalEntity& c_entity) const {
      CVector3 cEntityToCamera(m_cCameraPosition - c_entity.GetPosition());
      CVector3 cEntityDirection(CVector3::Z);
      cEntityDirection.Rotate(c_entity.GetOrientation());
      Real fDotProduct = cEntityDirection.DotProduct(cEntityToCamera);
      return ACos(fDotProduct / (cEntityDirection.Length() * cEntityToCamera.Length()));
   }

   /****************************************/
   /****************************************/

   CVector2 CPiPuckFrontCameraDefaultSensor::ProjectOntoSensor(const CVector3& c_point) const {
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

   bool CPiPuckFrontCameraDefaultSensor::IsInsideFrustum(const CVector3& c_point) const {
      for(const CPlane& c_plane : m_arrFrustumPlanes) {
         if(c_plane.GetNormal().DotProduct(c_point - c_plane.GetPosition()) < 0.0) {
            return false;
         }
      }
      return true;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CPiPuckFrontCameraDefaultSensor,
                   "pipuck_front_camera", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The Pi-Puck front camera sensor.",
                   "The documentation for this sensor is \e]8;;https://github.com/ilpincy/argos3/blob/master/src/plugins/robots/pi-puck/README.md#pipuck_front_camera\aavailable on Github\e]8;;\a.",
                   "Usable"
   );
}
