/**
 * @file <argos3/plugins/robots/generic/simulator/camera_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "camera_default_sensor.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/robots/generic/simulator/camera_sensor_algorithm.h>

namespace argos {

   /****************************************/
   /****************************************/

   CCameraDefaultSensor::CCameraDefaultSensor() :
      m_bShowFrustum(false),
      m_pcEmbodiedEntity(nullptr),
      m_pcControllableEntity(nullptr) {}

   /****************************************/
   /****************************************/

   void CCameraDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      /* Get the embodied and controllable entities */
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
   }

   /****************************************/
   /****************************************/

   void CCameraDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_CameraSensor::Init(t_tree);
         /* Show the frustums */
         GetNodeAttributeOrDefault(t_tree, "show_frustum", m_bShowFrustum, m_bShowFrustum);
         /* For each camera */
         TConfigurationNodeIterator itCamera("camera");
         for(itCamera = itCamera.begin(&t_tree);
             itCamera != itCamera.end();
             ++itCamera) {
            /* Get camera indentifier */
            std::string strId;
            GetNodeAttribute(*itCamera, "id", strId);
            /* Parse and look up the anchor */
            std::string strAnchorId;
            GetNodeAttribute(*itCamera, "anchor", strAnchorId);
            SAnchor& sAnchor = m_pcEmbodiedEntity->GetAnchor(strAnchorId);
            /* parse the offset */
            CVector3 cOffsetPosition;
            CQuaternion cOffsetOrientation;
            GetNodeAttribute(*itCamera, "position", cOffsetPosition);
            GetNodeAttribute(*itCamera, "orientation", cOffsetOrientation);
            CTransformationMatrix3 cOffset(cOffsetOrientation, cOffsetPosition);
            /* parse the range */
            CRange<Real> cRange;
            GetNodeAttribute(*itCamera, "range", cRange);
            /* create the projection matrix */
            CSquareMatrix<3> cProjectionMatrix;
            cProjectionMatrix.SetIdentityMatrix();
            /* set the focal length */
            CVector2 cFocalLength;
            GetNodeAttribute(*itCamera, "focal_length", cFocalLength);
            cProjectionMatrix(0,0) = cFocalLength.GetX(); // Fx
            cProjectionMatrix(1,1) = cFocalLength.GetY(); // Fy
            /* set the principal point */
            CVector2 cPrincipalPoint;
            GetNodeAttribute(*itCamera, "principal_point", cPrincipalPoint);
            cProjectionMatrix(0,2) = cPrincipalPoint.GetX(); // Px
            cProjectionMatrix(1,2) = cPrincipalPoint.GetY(); // Py
            /* set the distortion parameters */
            /*
            CMatrix<1,5> cDistortionParameters;
            std::string strDistortionParameters;
            Real pfDistortionParameters[3];
            GetNodeAttribute(*itCamera, "distortion_parameters", strDistortionParameters);
            ParseValues<Real>(strDistortionParameters, 3, pfDistortionParameters, ',');
            cDistortionParameters(0,0) = pfDistortionParameters[0]; // K1
            cDistortionParameters(0,1) = pfDistortionParameters[1]; // K2
            cDistortionParameters(0,4) = pfDistortionParameters[2]; // K3
            */
            /* parse the resolution */
            CVector2 cResolution;
            GetNodeAttribute(*itCamera, "resolution", cResolution);
            /* create and initialise the algorithms */
            std::vector<CCameraSensorSimulatedAlgorithm*> vecSimulatedAlgorithms;
            std::vector<CCI_CameraSensorAlgorithm*> vecAlgorithms;
            TConfigurationNodeIterator itAlgorithm;
            for(itAlgorithm = itAlgorithm.begin(&(*itCamera));
                itAlgorithm != itAlgorithm.end();
                ++itAlgorithm) {
               /* create the algorithm */
               CCameraSensorSimulatedAlgorithm* pcAlgorithm = 
                  CFactory<CCameraSensorSimulatedAlgorithm>::New(itAlgorithm->Value());
               /* check that algorithm inherits from a control interface */
               CCI_CameraSensorAlgorithm* pcCIAlgorithm = 
                  dynamic_cast<CCI_CameraSensorAlgorithm*>(pcAlgorithm);
               if(pcCIAlgorithm == nullptr) {
                  THROW_ARGOSEXCEPTION("Algorithm \"" << itAlgorithm->Value() << 
                                       "\" does not inherit from CCI_CameraSensorAlgorithm");
               }
               /* initialize the algorithm's control interface */
               pcCIAlgorithm->Init(*itAlgorithm);
               /* store pointers to the algorithms */
               vecSimulatedAlgorithms.push_back(pcAlgorithm);
               vecAlgorithms.push_back(pcCIAlgorithm);
            }
            /* create the simulated sensor */
            m_vecSensors.emplace_back(sAnchor, cOffset, cRange, cProjectionMatrix,
                                      cResolution, vecSimulatedAlgorithms);
            /* create the sensor's control interface */
            m_vecInterfaces.emplace_back(strId, vecAlgorithms);
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing camera sensor", ex);
      }
      Update();
   }

   /****************************************/
   /****************************************/

   void CCameraDefaultSensor::Update() {
      /* vector of controller rays */
      std::vector<std::pair<bool, CRay3> >& vecCheckedRays =
         m_pcControllableEntity->GetCheckedRays();
      /* sensor parameters */
      CTransformationMatrix3 cWorldToAnchorTransform;
      CTransformationMatrix3 cWorldToCameraTransform;
      CTransformationMatrix3 cCameraToWorldTransform;
      CVector3 cCameraLocation, cLookAt, cUp;
      CVector3 cX, cY, cZ;
      CVector3 cNearCenter, cNearTopLeft, cNearTopRight, cNearBottomLeft, cNearBottomRight;
      CVector3 cFarCenter, cFarTopLeft, cFarTopRight, cFarBottomLeft, cFarBottomRight;
      std::array<CPlane, 6> arrFrustumPlanes;
      CVector3 cBoundingBoxMinCorner, cBoundingBoxMaxCorner;
      CVector3 cBoundingBoxPosition, cBoundingBoxHalfExtents;
      /* for each camera sensor */
      for(SSensor& s_sensor : m_vecSensors) {
         /* calculate transform matrices */
         cWorldToAnchorTransform.SetFromComponents(s_sensor.Anchor.Orientation, s_sensor.Anchor.Position);
         cWorldToCameraTransform = cWorldToAnchorTransform * s_sensor.Offset;
         cCameraToWorldTransform = cWorldToCameraTransform.GetInverse();
         /* calculate camera direction vectors */
         cCameraLocation = cWorldToCameraTransform.GetTranslationVector();
         cLookAt = cWorldToCameraTransform * CVector3::Z;
         cUp = CVector3(0,-1,0); // -Y
         cUp.Rotate(cWorldToCameraTransform.GetRotationMatrix());
         /* calculate direction vectors */
         cZ = cCameraLocation - cLookAt;
         cZ.Normalize();
         cX = cUp;
         cX.CrossProduct(cZ);
         cX.Normalize();
         cY = cZ;
         cY.CrossProduct(cX);
         /* calculate frustum coordinates */
         cNearCenter = cCameraLocation - cZ * s_sensor.Range.GetMin();
         cFarCenter = cCameraLocation - cZ * s_sensor.Range.GetMax();
         cNearTopLeft = cNearCenter + (cY * s_sensor.NearPlaneHeight) - (cX * s_sensor.NearPlaneWidth);
         cNearTopRight = cNearCenter + (cY * s_sensor.NearPlaneHeight) + (cX * s_sensor.NearPlaneWidth);
         cNearBottomLeft = cNearCenter - (cY * s_sensor.NearPlaneHeight) - (cX * s_sensor.NearPlaneWidth);
         cNearBottomRight = cNearCenter - (cY * s_sensor.NearPlaneHeight) + (cX * s_sensor.NearPlaneWidth);
         cFarTopLeft = cFarCenter + (cY * s_sensor.FarPlaneHeight) - (cX * s_sensor.FarPlaneWidth);
         cFarTopRight = cFarCenter + (cY * s_sensor.FarPlaneHeight) + (cX * s_sensor.FarPlaneWidth);
         cFarBottomLeft = cFarCenter - (cY * s_sensor.FarPlaneHeight) - (cX * s_sensor.FarPlaneWidth);
         cFarBottomRight = cFarCenter - (cY * s_sensor.FarPlaneHeight) + (cX * s_sensor.FarPlaneWidth);
         /* show frustum if enabled by adding outline to the checked rays vector */
         if(m_bShowFrustum) {
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
         //std::cerr << cFarBottomRight.GetZ() << "\t" << cFarBottomLeft.GetZ() << std::endl; TODO
         /* generate a bounding box for the frustum */
         cBoundingBoxMinCorner = cNearCenter;
         cBoundingBoxMaxCorner = cNearCenter;
         for(const CVector3& c_point : {
               cNearTopLeft, cNearTopRight, cNearBottomLeft, cNearBottomRight, 
               cFarTopLeft, cFarTopRight, cFarBottomLeft, cFarBottomRight
            }) {
            if(c_point.GetX() > cBoundingBoxMaxCorner.GetX()) {
               cBoundingBoxMaxCorner.SetX(c_point.GetX());
            }
            if(c_point.GetX() < cBoundingBoxMinCorner.GetX()) {
               cBoundingBoxMinCorner.SetX(c_point.GetX());
            }
            if(c_point.GetY() > cBoundingBoxMaxCorner.GetY()) {
               cBoundingBoxMaxCorner.SetY(c_point.GetY());
            }
            if(c_point.GetY() < cBoundingBoxMinCorner.GetY()) {
               cBoundingBoxMinCorner.SetY(c_point.GetY());
            }
            if(c_point.GetZ() > cBoundingBoxMaxCorner.GetZ()) {
               cBoundingBoxMaxCorner.SetZ(c_point.GetZ());
            }
            if(c_point.GetZ() < cBoundingBoxMinCorner.GetZ()) {
               cBoundingBoxMinCorner.SetZ(c_point.GetZ());
            }
         }
         cBoundingBoxMaxCorner *= 0.5;
         cBoundingBoxMinCorner *= 0.5;
         cBoundingBoxPosition = (cBoundingBoxMaxCorner + cBoundingBoxMinCorner);
         cBoundingBoxHalfExtents = (cBoundingBoxMaxCorner - cBoundingBoxMinCorner);
         /* generate frustum planes */
         arrFrustumPlanes[0].SetFromThreePoints(cNearTopRight, cNearTopLeft, cFarTopLeft);
         arrFrustumPlanes[1].SetFromThreePoints(cNearBottomLeft, cNearBottomRight, cFarBottomRight);
         arrFrustumPlanes[2].SetFromThreePoints(cNearTopLeft, cNearBottomLeft, cFarBottomLeft);
         arrFrustumPlanes[3].SetFromThreePoints(cNearBottomRight, cNearTopRight, cFarBottomRight);
         arrFrustumPlanes[4].SetFromThreePoints(cNearTopLeft, cNearTopRight, cNearBottomRight);
         arrFrustumPlanes[5].SetFromThreePoints(cFarTopRight, cFarTopLeft, cFarBottomLeft);
         /* execute each algorithm */
         for(CCameraSensorSimulatedAlgorithm* pc_algorithm : s_sensor.Algorithms) {
            pc_algorithm->Update(s_sensor.ProjectionMatrix,
                                 arrFrustumPlanes,
                                 cCameraToWorldTransform,
                                 cCameraLocation,
                                 cBoundingBoxPosition,
                                 cBoundingBoxHalfExtents);
            /* transfer any rays to the controllable entity for rendering */
            vecCheckedRays.insert(std::end(vecCheckedRays),
                                  std::begin(pc_algorithm->GetCheckedRays()),
                                  std::end(pc_algorithm->GetCheckedRays()));
         }
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CCameraDefaultSensor,
                   "cameras", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",

                   "A generic multi-camera sensor capable of running various algorithms",
                   "The generic multi-camera sensor can be attached to any composable entity in\n"
                   "ARGoS that contains an embodied entity with at least one anchor. The sensor\n"
                   "can be initialized with a number of cameras each running different algorithms\n"
                   "for detecting different objects in the simulation. The sensor is designed so\n"
                   "that algorithms can project a feature in the simulation on to the virtual\n"
                   "sensor and store its 2D pixel coordinates as a reading. The implementation\n"
                   "of algorithms that behave differently, however, is also possible.\n\n"

                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <cameras implementation=\"default\"/>\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "OPTIONAL XML CONFIGURATION\n\n"

                   "It is possible to draw the frustum of each camera sensor in the OpenGL\n"
                   "visualization. This can be useful for sensor debugging but also to understand\n"
                   "what's wrong in your controller. To turn this functionality on, add the\n"
                   "attribute \"show_frustum\" as follows:\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <cameras implementation=\"default\" show_frustum=\"true\"/>\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "To add a camera to the plugin, create a camera node as shown in the following\n"
                   "example. A camera is defined by its range (how close and how far the camera\n"
                   "can see), its anchor and its position and orientation offsets from that\n"
                   "that anchor, its focal length and principal point (which define the\n"
                   "projection matrix), and its resolution.\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <cameras implementation=\"default\" show_frustum=\"true\">\n"
                   "          <camera id=\"camera0\" range=\"0.025:0.25\" anchor=\"origin\"\n"
                   "                  position=\"0.1,0,0.1\" orientation=\"90,-90,0\"\n"
                   "                  focal_length=\"800,800\" principal_point=\"320,240\"\n"
                   "                  resolution=\"640,480\"/>\n"
                   "        </cameras>\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "To run an algorithm on the camera sensor, simply add the algorithm as a node\n"
                   "under the camera node. At the time of writing, three algorithms are available\n"
                   "by default: led_detector, directional_led_detector, and tag_detector. Each of\n"
                   "algorithms requires a medium attribute that specifies the medium where the\n"
                   "target entities are indexed. By setting the show_rays attribute to true, you\n"
                   "can see whether or not a target was partially occluded by another object in\n"
                   "the simulation. For example:\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <cameras implementation=\"default\" show_frustum=\"true\">\n"
                   "          <camera id=\"camera0\" range=\"0.025:0.25\" anchor=\"origin\"\n"
                   "                  position=\"0.1,0,0.1\" orientation=\"90,-90,0\"\n"
                   "                  focal_length=\"800,800\" principal_point=\"320,240\"\n"
                   "                  resolution=\"640,480\">\n"
                   "             <led_detector medium=\"leds\" show_rays=\"true\"/>\n"
                   "          </camera>\n"
                   "        </cameras>\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n",

                   "Usable");
}
