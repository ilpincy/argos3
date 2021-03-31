/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_widget.h"
#include "qtopengl_main_window.h"
#include "qtopengl_user_functions.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/plane.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/floor_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/positional_entity.h>

#include <QDir>
#include <QToolTip>
#include <QTimerEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QOpenGLFramebufferObject>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

namespace argos {

   static const Real ASPECT_RATIO         = 4.0f / 3.0f;

   /****************************************/
   /****************************************/

   CQTOpenGLWidget::CQTOpenGLWidget(QWidget* pc_parent,
                                    CQTOpenGLMainWindow& c_main_window,
                                    CQTOpenGLUserFunctions& c_user_functions) :
      QOpenGLWidget(pc_parent),
      m_cMainWindow(c_main_window),
      m_cUserFunctions(c_user_functions),
      nTimerId(-1),
      m_bFastForwarding(false),
      m_nDrawFrameEvery(1),
      m_nFrameCounter(0),
      m_bMouseGrabbed(false),
      m_bShiftPressed(false),
      m_bInvertMouse(false),
      m_cSimulator(CSimulator::GetInstance()),
      m_cSpace(m_cSimulator.GetSpace()),
      m_bShowBoundary(true),
      m_bUsingFloorTexture(false),
      m_pcFloorTexture(nullptr),
      m_pcGroundTexture(nullptr) {
      /* Set the widget's size policy */
      QSizePolicy cSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
      cSizePolicy.setHeightForWidth(true);
      setSizePolicy(cSizePolicy);
      /* Grab focus when clicked on */
      setFocusPolicy(Qt::ClickFocus);
      /* Force size and geometry */
      updateGeometry();
      /* Keys */
      m_mapPressedKeys[DIRECTION_UP]        = false;
      m_mapPressedKeys[DIRECTION_DOWN]      = false;
      m_mapPressedKeys[DIRECTION_LEFT]      = false;
      m_mapPressedKeys[DIRECTION_RIGHT]     = false;
      m_mapPressedKeys[DIRECTION_FORWARDS]  = false;
      m_mapPressedKeys[DIRECTION_BACKWARDS] = false;
   }

   /****************************************/
   /****************************************/

   CQTOpenGLWidget::~CQTOpenGLWidget() {
      makeCurrent();
      delete m_pcGroundTexture;
      if(m_bUsingFloorTexture) {
         delete m_pcFloorTexture;
      }
      doneCurrent();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::initializeGL() {
      /* Initializes the openGL functions */
      initializeOpenGLFunctions();
      /* Set clear color */
      glClearColor(0, .5, .5, 255); // dark cyan
      /* Set up the texture parameters for the floor plane
         (here we refer to the standard floor, not the floor entity) */
      m_pcGroundTexture = new QOpenGLTexture(QImage(m_cMainWindow.GetTextureDir() + "/ground.png"));
      m_pcGroundTexture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,
                                          QOpenGLTexture::Linear);
#ifdef ARGOS_WITH_FREEIMAGE
      /* Now take care of the floor entity */
      try {
         /* Create an image to use as texture */
         m_cSpace.GetFloorEntity().SaveAsImage("/tmp/argos_floor.png");
         m_bUsingFloorTexture = true;
         /* Use the image as texture */
         m_pcFloorTexture = new QOpenGLTexture(QImage("/tmp/argos_floor.png"));
         m_pcFloorTexture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,
                                            QOpenGLTexture::Linear);
         m_cSpace.GetFloorEntity().ClearChanged();
      }
      catch(CARGoSException& ex) {}
#endif
      /* Nicest hints */
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
      /* Setup lighting */
      GLfloat pfLightAmbient[]  = {   .2f,   .2f,  .2f, 1.0f };
      GLfloat pfLightDiffuse[]  = {   .8f,   .8f,  .8f, 1.0f };
      GLfloat pfLightPosition[] = { 50.0f , 50.0f , 2.0f , 1.0f };
      glLightfv(GL_LIGHT0, GL_AMBIENT,  pfLightAmbient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE,  pfLightDiffuse);
      glLightfv(GL_LIGHT0, GL_POSITION, pfLightPosition);
      glEnable(GL_LIGHT0);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::paintGL() {
      /* Clear accumulator buffer */
      glClearAccum(0.0, 0.0, 0.0, 0.0);
      /* Clear color buffer and depth buffer */
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      /* Smooth lines and shades */
      glEnable(GL_LINE_SMOOTH);
      glShadeModel(GL_SMOOTH);
      /* Enable depth testing */
      glEnable(GL_DEPTH_TEST);
      /* Enable face culling */
      glEnable(GL_CULL_FACE);
      /* Enable lighting */
      glEnable(GL_LIGHTING);
      /* Calculate the perspective matrix */
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(m_cCamera.GetActivePlacement().YFieldOfView.GetValue(),
                     ASPECT_RATIO,
                     0.1f, 1000.0f);
      /* Place the camera */
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      m_cCamera.Look();
      /* Draw the arena */
      DrawArena();
      /* Draw the objects */
      CEntity::TVector& vecEntities = m_cSpace.GetRootEntityVector();
      for(auto itEntities = vecEntities.begin();
          itEntities != vecEntities.end();
          ++itEntities) {
         glPushMatrix();
         CallEntityOperation<CQTOpenGLOperationDrawNormal, CQTOpenGLWidget, void>(*this, **itEntities);
         m_cUserFunctions.Call(**itEntities);
         glPopMatrix();
      }
      /* Draw the selected object, if necessary */
      if(m_sSelectionInfo.IsSelected) {
         glPushMatrix();
         CallEntityOperation<CQTOpenGLOperationDrawSelected, CQTOpenGLWidget, void>(*this, *m_sSelectionInfo.Entity);
         glPopMatrix();
      }
      /* Draw in world */
      glPushMatrix();
      m_cUserFunctions.DrawInWorld();
      glPopMatrix();
      /* Draw axes */
      DrawAxes();
      /* Execute overlay drawing */
      glShadeModel(GL_FLAT);
      glDisable(GL_LIGHTING);
      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);
      glMatrixMode(GL_MODELVIEW);
      QPainter cPainter(this);
      cPainter.setRenderHint(QPainter::Antialiasing);
      cPainter.setRenderHint(QPainter::TextAntialiasing);
      m_cUserFunctions.DrawOverlay(cPainter);
      // cPainter.drawText(rect(), QString("%1 FPS").arg(m_fFPS, 0, 'f', 0));
      cPainter.end();
      /* Grab frame, if necessary */
      if(m_sFrameGrabData.GUIGrabbing || m_sFrameGrabData.HeadlessGrabbing) {
         QString strFileName = QString("%1/%2%3.%4")
            .arg(m_sFrameGrabData.Directory)
            .arg(m_sFrameGrabData.BaseName)
            .arg(m_cSpace.GetSimulationClock(), 10, 10, QChar('0'))
            .arg(m_sFrameGrabData.Format);
         QToolTip::showText(pos() + geometry().center(), "Stored frame to \"" + strFileName);
         grabFramebuffer()
            .save(
               strFileName,
               nullptr,
               m_sFrameGrabData.Quality);
      }
   }

   /****************************************/
   /****************************************/

   CRay3 CQTOpenGLWidget::RayFromWindowCoord(int n_x,
                                             int n_y) {
      /* Make sure OpenGL context is correct */
      makeCurrent();
      /* Rescale coordinates by devicePixelRatio */
      n_x *= devicePixelRatio();
      n_y *= devicePixelRatio();
      /* Get current viewport */
      GLint nViewport[4];
      glGetIntegerv(GL_VIEWPORT, nViewport);
      /* Get OpenGL matrices */
      GLdouble fModelViewMatrix[16];
      GLdouble fProjectionMatrix[16];
      glGetDoublev(GL_MODELVIEW_MATRIX, fModelViewMatrix);
      glGetDoublev(GL_PROJECTION_MATRIX, fProjectionMatrix);
      /*
       * Convert mouse position in window into OpenGL representation
       */
      /* The x coordinate stays the same */
      GLfloat fWinX = n_x;
      /* The y coordinate of the window is top-left; in OpenGL is bottom-left */
      GLfloat fWinY = nViewport[3] - n_y;
      /*
       * Get the position of the ray start in the world
       * The ray starts at the near clipping plane (depth = 0.0f)
       */
      GLdouble fRayStartX, fRayStartY, fRayStartZ;
      gluUnProject(fWinX, fWinY, 0.0f,
                   fModelViewMatrix, fProjectionMatrix, nViewport,
                   &fRayStartX, &fRayStartY, &fRayStartZ);
      /*
       * Get the position of the ray end in the world
       * The ray starts at the far clipping plane (depth = 1.0f)
       */
      GLdouble fRayEndX, fRayEndY, fRayEndZ;
      gluUnProject(fWinX, fWinY, 1.0f,
                   fModelViewMatrix, fProjectionMatrix, nViewport,
                   &fRayEndX, &fRayEndY, &fRayEndZ);
      doneCurrent();
      return CRay3(CVector3(fRayStartX, fRayStartY, fRayStartZ),
                   CVector3(fRayEndX, fRayEndY, fRayEndZ));
   }

   /****************************************/
   /****************************************/

   CVector3 CQTOpenGLWidget::GetWindowCoordInWorld(int n_x,
                                                   int n_y) {
      /* Make sure OpenGL context is correct */
      makeCurrent();
      /* Rescale coordinates by devicePixelRatio */
      n_x *= devicePixelRatio();
      n_y *= devicePixelRatio();
      /* Get current viewport */
      GLint nViewport[4];
      glGetIntegerv(GL_VIEWPORT, nViewport);
      /* Get OpenGL matrices */
      GLdouble fModelViewMatrix[16];
      GLdouble fProjectionMatrix[16];
      glGetDoublev(GL_MODELVIEW_MATRIX, fModelViewMatrix);
      glGetDoublev(GL_PROJECTION_MATRIX, fProjectionMatrix);
      /*
       * Convert mouse position in window into a 3D representation
       */
      /* The x coordinate stays the same */
      GLfloat fWinX = n_x;
      /* The y coordinate of the window is top-left; in OpenGL is bottom-left */
      GLfloat fWinY = nViewport[3] - n_y;
      /* Read the z coordinate from the depth buffer in the back buffer */
      GLfloat fWinZ;
      glReadBuffer(GL_BACK);
      glReadPixels(n_x, (GLint)fWinY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &fWinZ);
      /* Get the actual position in the world */
      GLdouble fWorldX, fWorldY, fWorldZ;
      gluUnProject(fWinX, fWinY, fWinZ,
                   fModelViewMatrix, fProjectionMatrix, nViewport,
                   &fWorldX, &fWorldY, &fWorldZ);
      /*
       * Swap coordinates when creating the ray
       * In ARGoS, the up vector is the Z-axis, while in OpenGL it is the Y-axis
       */
      doneCurrent();
      return CVector3(fWorldX, fWorldZ, fWorldY);
   }

   /****************************************/
   /****************************************/

   CEntity* CQTOpenGLWidget::GetSelectedEntity() {
      return (m_sSelectionInfo.IsSelected ?
              m_sSelectionInfo.Entity :
              nullptr);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SelectEntity(CEntity& c_entity) {
      /* Check whether an entity had previously been selected */
      if(m_sSelectionInfo.IsSelected) {
         /* An entity had previously been selected */
         /* Is that entity already selected? */
         if(m_sSelectionInfo.Entity == &c_entity) return;
         /* Deselect the previous one */
         emit EntityDeselected(m_sSelectionInfo.Entity);
         m_cUserFunctions.EntityDeselected(
            *m_sSelectionInfo.Entity);
      }
      else {
         /* No entity had previously been selected */
         m_sSelectionInfo.IsSelected = true;
      }
      /* Select the new entity */
      m_sSelectionInfo.Entity = &c_entity;
      emit EntitySelected(&c_entity);
      m_cUserFunctions.EntitySelected(c_entity);
      update();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::DeselectEntity() {
      /* If no entity was selected, nothing to do */
      if(!m_sSelectionInfo.IsSelected) return;
      /* Deselect the entity */
      emit EntityDeselected(m_sSelectionInfo.Entity);
      m_cUserFunctions.EntityDeselected(
         *m_sSelectionInfo.Entity);
      m_sSelectionInfo.IsSelected = false;
      update();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SelectInScene(UInt32 un_x,
                                       UInt32 un_y) {
      CRay3 cRay = RayFromWindowCoord(un_x, un_y);
      SEmbodiedEntityIntersectionItem sItem;
      if(GetClosestEmbodiedEntityIntersectedByRay(sItem, cRay))
         SelectEntity(sItem.IntersectedEntity->GetRootEntity());
      else
         DeselectEntity();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::DrawEntity(CPositionalEntity& c_entity) {
      /* Get the position of the entity */
      const CVector3& cPosition = c_entity.GetPosition();
      /* Get the orientation of the entity */
      const CQuaternion& cOrientation = c_entity.GetOrientation();
      CRadians cZAngle, cYAngle, cXAngle;
      cOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      /* First, translate the entity */
      glTranslated(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
      /* Second, rotate the entity */
      glRotated(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotated(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotated(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::DrawEntity(CEmbodiedEntity& c_entity) {
      /* Get the position of the entity */
      const CVector3& cPosition = c_entity.GetOriginAnchor().Position;
      /* Get the orientation of the entity */
      const CQuaternion& cOrientation = c_entity.GetOriginAnchor().Orientation;
      CRadians cZAngle, cYAngle, cXAngle;
      cOrientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      /* First, translate the entity */
      glTranslated(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
      /* Second, rotate the entity */
      glRotated(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotated(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotated(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::DrawRays(CControllableEntity& c_entity) {
      if(! c_entity.GetCheckedRays().empty()) {
         glDisable(GL_LIGHTING);
         glLineWidth(1.0f);
         glBegin(GL_LINES);
         for(UInt32 i = 0; i < c_entity.GetCheckedRays().size(); ++i) {
            if(c_entity.GetCheckedRays()[i].first) {
               glColor3f(1.0, 0.0, 1.0);
            }
            else {
               glColor3f(0.0, 1.0, 1.0);
            }
            const CVector3& cStart = c_entity.GetCheckedRays()[i].second.GetStart();
            const CVector3& cEnd = c_entity.GetCheckedRays()[i].second.GetEnd();
            glVertex3d(cStart.GetX(), cStart.GetY(), cStart.GetZ());
            glVertex3d(cEnd.GetX(), cEnd.GetY(), cEnd.GetZ());
         }
         glEnd();
         glPointSize(5.0);
         glColor3f(0.0, 0.0, 0.0);
         glBegin(GL_POINTS);
         for(UInt32 i = 0; i < c_entity.GetIntersectionPoints().size(); ++i) {
            const CVector3& cPoint = c_entity.GetIntersectionPoints()[i];
            glVertex3d(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());
         }
         glEnd();
         glPointSize(1.0);
         glEnable(GL_LIGHTING);
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::DrawBoundingBox(CEmbodiedEntity& c_entity) {
      const SBoundingBox& sBBox = c_entity.GetBoundingBox();
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDisable(GL_LIGHTING);
      glLineWidth(3.0f);
      glColor3f(1.0f, 1.0f, 1.0f);
      /* This part covers the top and bottom faces (parallel to XY) */
      glBegin(GL_QUADS);
      /* Bottom face */
      glNormal3d(0.0f, 0.0f, -1.0f);
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      /* Top face */
      glNormal3d(0.0f, 0.0f, 1.0f);
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glEnd();
      /* This part covers the faces (South, East, North, West) */
      glBegin(GL_QUADS);
      /* South face */
      glNormal3d(-1.0f, 0.0f, 0.0f);
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      /* East face */
      glNormal3d(0.0f, -1.0f, 0.0f);
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      /* North face */
      glNormal3d(1.0f, 0.0f, 0.0f);
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      /* West face */
      glNormal3d(0.0f, 1.0f, 0.0f);
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3d(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3d(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      glEnd();
      glEnable(GL_LIGHTING);
      glLineWidth(1.0f);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::PlayExperiment() {
      m_bFastForwarding = false;
      if(nTimerId != -1) killTimer(nTimerId);
      nTimerId = startTimer(static_cast<UInt32>(CPhysicsEngine::GetSimulationClockTick() * 1000.0f));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::FastForwardExperiment() {
      m_nFrameCounter = 0;
      m_bFastForwarding = true;
      if(nTimerId != -1) killTimer(nTimerId);
      nTimerId = startTimer(1);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::PauseExperiment() {
      m_bFastForwarding = false;
      if(nTimerId != -1) killTimer(nTimerId);
      nTimerId = -1;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::StepExperiment() {
      if(!m_cSimulator.IsExperimentFinished()) {
         m_cSimulator.UpdateSpace();
         if(m_bFastForwarding) {
            /* Frame dropping happens only in fast-forward */
            m_nFrameCounter = m_nFrameCounter % m_nDrawFrameEvery;
            if(m_nFrameCounter == 0) {
               update();
            }
            ++m_nFrameCounter;
         } else {
            update();
         }
         m_cCamera.UpdateTimeline();
         emit StepDone(m_cSpace.GetSimulationClock());
      }
      else {
         PauseExperiment();
         emit ExperimentDone();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::ResetExperiment() {
      m_cSimulator.Reset();
      m_cCamera.Reset();
      delete m_pcGroundTexture;
      if(m_bUsingFloorTexture) delete m_pcFloorTexture;
      initializeGL();
      update();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SetDrawFrameEvery(SInt32 n_every) {
      m_nDrawFrameEvery = n_every;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SetGrabFrame(bool b_grab_on) {
     m_sFrameGrabData.GUIGrabbing = b_grab_on;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SetCamera(int n_camera) {
      m_cCamera.SetActivePlacement(n_camera);
      update();
      QToolTip::showText(pos() + geometry().center(), QString("Current camera: #%1").arg(n_camera+1));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SetCameraFocalLength(double f_length) {
      m_cCamera.GetActivePlacement().LensFocalLength = f_length / 1000.0f;
      m_cCamera.GetActivePlacement().CalculateYFieldOfView();
      update();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::KeyPressed(QKeyEvent* pc_event) {
      switch(pc_event->key()) {
         case Qt::Key_W:
         case Qt::Key_Up:
            /* Forwards */
            m_mapPressedKeys[DIRECTION_FORWARDS] = true;
            reactToKeyEvent();
            break;
         case Qt::Key_S:
         case Qt::Key_Down:
            /* Backwards */
            m_mapPressedKeys[DIRECTION_BACKWARDS] = true;
            reactToKeyEvent();
            break;
         case Qt::Key_A:
         case Qt::Key_Left:
            /* Left */
            m_mapPressedKeys[DIRECTION_LEFT] = true;
            reactToKeyEvent();
            break;
         case Qt::Key_D:
         case Qt::Key_Right:
            /* Right */
            m_mapPressedKeys[DIRECTION_RIGHT] = true;
            reactToKeyEvent();
            break;
         case Qt::Key_E:
            /* Up */
            m_mapPressedKeys[DIRECTION_UP] = true;
            reactToKeyEvent();
            break;
         case Qt::Key_Q:
            /* Up */
            m_mapPressedKeys[DIRECTION_DOWN] = true;
            reactToKeyEvent();
            break;
         default:
            /* Unknown key */
            QOpenGLWidget::keyPressEvent(pc_event);
            break;
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::KeyReleased(QKeyEvent* pc_event) {
      switch(pc_event->key()) {
         case Qt::Key_W:
         case Qt::Key_Up:
            /* Forwards */
            m_mapPressedKeys[DIRECTION_FORWARDS] = false;
            reactToKeyEvent();
            break;
         case Qt::Key_S:
         case Qt::Key_Down:
            /* Backwards */
            m_mapPressedKeys[DIRECTION_BACKWARDS] = false;
            reactToKeyEvent();
            break;
         case Qt::Key_A:
         case Qt::Key_Left:
            /* Left */
            m_mapPressedKeys[DIRECTION_LEFT] = false;
            reactToKeyEvent();
            break;
         case Qt::Key_D:
         case Qt::Key_Right:
            /* Right */
            m_mapPressedKeys[DIRECTION_RIGHT] = false;
            reactToKeyEvent();
            break;
         case Qt::Key_E:
            /* Up */
            m_mapPressedKeys[DIRECTION_UP] = false;
            reactToKeyEvent();
            break;
         case Qt::Key_Q:
            /* Up */
            m_mapPressedKeys[DIRECTION_DOWN] = false;
            reactToKeyEvent();
            break;
         default:
            /* Unknown key */
            QOpenGLWidget::keyPressEvent(pc_event);
            break;
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::DrawArena() {
      CVector3 cArenaSize(m_cSpace.GetArenaSize());
      CVector3 cArenaMinCorner(m_cSpace.GetArenaCenter().GetX() - cArenaSize.GetX() * 0.5f,
                               m_cSpace.GetArenaCenter().GetY() - cArenaSize.GetY() * 0.5f,
                               m_cSpace.GetArenaCenter().GetZ() - cArenaSize.GetZ() * 0.5f);
      CVector3 cArenaMaxCorner(m_cSpace.GetArenaCenter().GetX() + cArenaSize.GetX() * 0.5f,
                               m_cSpace.GetArenaCenter().GetY() + cArenaSize.GetY() * 0.5f,
                               m_cSpace.GetArenaCenter().GetZ() + cArenaSize.GetZ() * 0.5f);
      /* Disable lighting - no funny color effects */
      glDisable(GL_LIGHTING);
      /* Enable textures */
      glEnable(GL_TEXTURE_2D);
      /* Take care of the floor entity if necessary */
      /* The texture covers the object like a decal */
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
#ifdef ARGOS_WITH_FREEIMAGE
      if(m_bUsingFloorTexture) {
         /* Use the image as texture */
         if(m_cSpace.GetFloorEntity().HasChanged()) {
            /* Create an image to use as texture */
            m_pcFloorTexture->destroy();
            m_pcFloorTexture->create();
            m_cSpace.GetFloorEntity().SaveAsImage("/tmp/argos_floor.png");
            m_pcFloorTexture->setData(QImage("/tmp/argos_floor.png"));
            m_pcFloorTexture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,
                                               QOpenGLTexture::Linear);
            /* Clear the 'changed' flag on the floor entity */
            m_cSpace.GetFloorEntity().ClearChanged();
         }
         /* Draw the floor entity along with its texture */
         m_pcFloorTexture->bind();
         glBegin(GL_QUADS);
         glTexCoord2d(0.0f, 1.0f); glVertex3d(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), 0.0f);
         glTexCoord2d(1.0f, 1.0f); glVertex3d(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), 0.0f);
         glTexCoord2d(1.0f, 0.0f); glVertex3d(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), 0.0f);
         glTexCoord2d(0.0f, 0.0f); glVertex3d(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), 0.0f);
         glEnd();
      }
      else {
#endif
         /* Wrap the texture at the edges, which in this case means that it is repeated */
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         m_pcGroundTexture->bind();
         /* Draw the floor along with its texture */
         glBegin(GL_QUADS);
         glTexCoord2d(0.0f, cArenaSize.GetY());              glVertex3d(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), 0.0f);
         glTexCoord2d(cArenaSize.GetX(), cArenaSize.GetY()); glVertex3d(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), 0.0f);
         glTexCoord2d(cArenaSize.GetX(), 0.0f);              glVertex3d(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), 0.0f);
         glTexCoord2d(0.0f, 0.0f);                           glVertex3d(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), 0.0f);
         glEnd();
#ifdef ARGOS_WITH_FREEIMAGE
      }
#endif
      /* Disable the textures */
      glDisable(GL_TEXTURE_2D);
      if(m_bShowBoundary) {
         /* Draw walls */
         glDisable(GL_CULL_FACE);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glLineWidth(10.0f);
         glColor3f(0.0f, 0.0f, 0.0f);
         /* This part covers the top and bottom faces (parallel to XY) */
         glBegin(GL_QUADS);
         /* Top face */
         glVertex3d(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
         glVertex3d(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
         glVertex3d(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
         glVertex3d(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
         glEnd();
         /* This part covers the faces (South, East, North, West) */
         glBegin(GL_QUADS);
         /* South face */
         glVertex3d(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), cArenaMinCorner.GetZ());
         glVertex3d(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
         glVertex3d(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
         glVertex3d(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMinCorner.GetZ());
         /* East face */
         glVertex3d(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), cArenaMinCorner.GetZ());
         glVertex3d(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), cArenaMinCorner.GetZ());
         glVertex3d(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
         glVertex3d(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
         /* North face */
         glVertex3d(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), cArenaMinCorner.GetZ());
         glVertex3d(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMinCorner.GetZ());
         glVertex3d(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
         glVertex3d(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
         /* West face */
         glVertex3d(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMinCorner.GetZ());
         glVertex3d(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
         glVertex3d(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
         glVertex3d(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMinCorner.GetZ());
         glEnd();
         glLineWidth(1.0f);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glEnable(GL_CULL_FACE);
      }
      /* Restore lighting */
      glEnable(GL_LIGHTING);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::DrawAxes() {
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::timerEvent(QTimerEvent* pc_event) {
      StepExperiment();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::mousePressEvent(QMouseEvent* pc_event) {
      /*
       * Mouse press without shift
       * Either pure press, or press + CTRL
       */
      if(! (pc_event->modifiers() & Qt::ShiftModifier)) {
         if(! (pc_event->modifiers() & Qt::AltModifier)) {
            m_bMouseGrabbed = true;
            m_cMouseGrabPos = pc_event->pos();
         }
         else {
            m_cUserFunctions.MouseKeyPressed(pc_event);
         }
      }
      /*
       * Mouse press with shift
       */
      else {
         m_bMouseGrabbed = false;
         SelectInScene(pc_event->pos().x(),
                       pc_event->pos().y());
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::mouseReleaseEvent(QMouseEvent* pc_event) {
      /*
       * Mouse grabbed, selected entity, CTRL pressed
       */
      if(m_bMouseGrabbed &&
         m_sSelectionInfo.IsSelected &&
         (pc_event->modifiers() & Qt::ControlModifier)) {
         /* Treat selected entity as an embodied entity */
         auto* pcEntity = dynamic_cast<CEmbodiedEntity*>(m_sSelectionInfo.Entity);
         if(pcEntity == nullptr) {
            /* Treat selected entity as a composable entity with an embodied component */
            auto* pcCompEntity = dynamic_cast<CComposableEntity*>(m_sSelectionInfo.Entity);
            if(pcCompEntity != nullptr && pcCompEntity->HasComponent("body")) {
               pcEntity = &pcCompEntity->GetComponent<CEmbodiedEntity>("body");
            }
            else {
               /* All conversions failed, get out */
               m_bMouseGrabbed = false;
               return;
            }
         }
         /*
          * If we get here, pcEntity is set to a non-NULL value
          * Move the entity to the wanted place
          */
         /* Create a plane coincident with the world XY plane, centered at the entity position */
         CPlane cXYPlane(pcEntity->GetOriginAnchor().Position, CVector3::Z);
         /* Create a ray from the image pixel to the world */
         CRay3 cMouseRay =
            RayFromWindowCoord(pc_event->pos().x(),
                               pc_event->pos().y());
         /* Calculate the new entity position as the intersection of the projected mouse position
            with the plane created before */
         CVector3 cNewPos;
         if(cMouseRay.Intersects(cXYPlane, cNewPos)) {
            CVector3 cOldPos(pcEntity->GetOriginAnchor().Position);
            if(pcEntity->MoveTo(cNewPos, pcEntity->GetOriginAnchor().Orientation)) {
               m_cUserFunctions.EntityMoved(pcEntity->GetRootEntity(), cOldPos, cNewPos);
            }
            /* Entity moved, redraw */
            update();
         }
      }
      else {
         m_cUserFunctions.MouseKeyReleased(pc_event);
      }
      /*
       * Mouse was grabbed, button released -> ungrab mouse
       */
      m_bMouseGrabbed = false;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::mouseMoveEvent(QMouseEvent* pc_event) {
      /*
       * Moving while mouse grabbed -> camera movement
       */
      if(m_bMouseGrabbed) {
         if(! (pc_event->modifiers() & Qt::ControlModifier)) {
            /*
             * Camera movement
             */
            if(pc_event->buttons() == Qt::LeftButton) {
               if (m_bInvertMouse) m_cCamera.Rotate( pc_event->pos() - m_cMouseGrabPos);
               else m_cCamera.Rotate( m_cMouseGrabPos - pc_event->pos());
               m_cMouseGrabPos = pc_event->pos();
               update();
            }
            else if(pc_event->buttons() == Qt::RightButton) {
               QPoint cDelta(pc_event->pos() - m_cMouseGrabPos);
               m_cCamera.Move(-cDelta.y(), cDelta.x(), 0);
               m_cMouseGrabPos = pc_event->pos();
               update();
            }
            else if(pc_event->buttons() == Qt::MiddleButton) {
               QPoint cDelta(pc_event->pos() - m_cMouseGrabPos);
               m_cCamera.Move(0, 0, cDelta.y());
               m_cMouseGrabPos = pc_event->pos();
               update();
            }
         }
      }
      else {
         m_cUserFunctions.MouseMoved(pc_event);
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::wheelEvent(QWheelEvent *pc_event) {
      if(m_sSelectionInfo.IsSelected && (pc_event->modifiers() & Qt::ControlModifier)) {
         /* Treat selected entity as an embodied entity */
         auto* pcEntity = dynamic_cast<CEmbodiedEntity*>(m_sSelectionInfo.Entity);
         if(pcEntity == nullptr) {
            /* Treat selected entity as a composable entity with an embodied component */
            auto* pcCompEntity = dynamic_cast<CComposableEntity*>(m_sSelectionInfo.Entity);
            if(pcCompEntity != nullptr && pcCompEntity->HasComponent("body")) {
               pcEntity = &pcCompEntity->GetComponent<CEmbodiedEntity>("body");
            }
            else {
               return;
            }
         }
         /*
          * If we get here, pcEntity is set to a non-NULL value
          * Rotate the entity
          */
         CDegrees cDegrees(pc_event->angleDelta().y() / 8);
         CQuaternion cRotation(ToRadians(cDegrees), CVector3::Z);
         CQuaternion cOldOrientation(pcEntity->GetOriginAnchor().Orientation);
         CQuaternion cNewOrientation(cOldOrientation * cRotation);
         if(pcEntity->MoveTo(pcEntity->GetOriginAnchor().Position, cNewOrientation)) {
            m_cUserFunctions.EntityRotated(pcEntity->GetRootEntity(), cOldOrientation, cNewOrientation);
         }
         /* entity updated, redraw the scene */
         update();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::keyPressEvent(QKeyEvent* pc_event) {
      m_cUserFunctions.KeyPressed(pc_event);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::keyReleaseEvent(QKeyEvent* pc_event) {
      m_cUserFunctions.KeyReleased(pc_event);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::reactToKeyEvent() {
      SInt32 nForwardsBackwards = 0;
      SInt32 nSideways = 0;
      SInt32 nUpDown = 0;

      if(m_mapPressedKeys[DIRECTION_UP])        nUpDown++;
      if(m_mapPressedKeys[DIRECTION_DOWN])      nUpDown--;
      if(m_mapPressedKeys[DIRECTION_LEFT])      nSideways++;
      if(m_mapPressedKeys[DIRECTION_RIGHT])     nSideways--;
      if(m_mapPressedKeys[DIRECTION_FORWARDS])  nForwardsBackwards++;
      if(m_mapPressedKeys[DIRECTION_BACKWARDS]) nForwardsBackwards--;

      if(nForwardsBackwards != 0 ||
         nSideways != 0 ||
         nUpDown != 0) {
         m_cCamera.Move(15 * nForwardsBackwards,
                        15 * nSideways,
                        15 * nUpDown);
         update();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::resizeEvent(QResizeEvent* pc_event) {
      /* Call parent's resize event handler */
      QOpenGLWidget::resizeEvent(pc_event);
      /* Show new window size */
      QToolTip::showText(pos() + geometry().center(), QString("Size: %1 x %2").arg(pc_event->size().width()).arg(pc_event->size().height()));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SFrameGrabData::Init(TConfigurationNode& t_tree) {
      if(NodeExists(t_tree, "frame_grabbing")) {
         TConfigurationNode& tNode = GetNode(t_tree, "frame_grabbing");
         std::string strBuffer;
         /* Parse directory, removing trailing '/' */
         strBuffer = ".";
         GetNodeAttributeOrDefault(tNode, "directory", strBuffer, strBuffer);
         size_t unEndPos = strBuffer.find_last_not_of("/ \t");
         if(unEndPos != std::string::npos) {
            strBuffer = strBuffer.substr(0, unEndPos+1);
         }
         Directory = strBuffer.c_str();
         QDir cDirectory(Directory);
         if(!cDirectory.exists()) {
            THROW_ARGOSEXCEPTION("QTOpenGL: frame grabbing directory \"" << strBuffer << "\" does not exist. Create it first!");
         }
         /* Parse base name */
         strBuffer = "frame_";
         GetNodeAttributeOrDefault(tNode, "base_name", strBuffer, strBuffer);
         BaseName = strBuffer.c_str();
         /* Parse format */
         strBuffer = "png";
         GetNodeAttributeOrDefault(tNode, "format", strBuffer, strBuffer);
         Format = strBuffer.c_str();
         /* Parse quality */
         GetNodeAttributeOrDefault(tNode, "quality", Quality, Quality);

         /* Parse headless grabbing */
         GetNodeAttributeOrDefault(tNode,
                                   "headless_grabbing",
                                   HeadlessGrabbing,
                                   HeadlessGrabbing);
         /* Parse headless frame size */
         strBuffer = "1600x1200";
         GetNodeAttributeOrDefault(tNode,
                                   "headless_frame_size",
                                   strBuffer,
                                   strBuffer);
         uint dims[2];
         ParseValues(strBuffer, 2, dims, 'x');
         Size = QSize(dims[0], dims[1]);

         /* Parse headless frame rate */
         GetNodeAttributeOrDefault(tNode,
                                   "headless_frame_rate",
                                   HeadlessFrameRate,
                                   HeadlessFrameRate);
      }
   }

   /****************************************/
   /****************************************/

}
