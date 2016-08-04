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
#include <QPainter>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

namespace argos {

   static const Real ASPECT_RATIO         = 4.0f / 3.0f;
   static const UInt32 SELECT_BUFFER_SIZE = 128;
   
   /****************************************/
   /****************************************/

   CQTOpenGLWidget::CQTOpenGLWidget(const QGLFormat& c_format,
                                    QWidget* pc_parent,
                                    CQTOpenGLMainWindow* pc_main_window,
                                    CQTOpenGLUserFunctions& c_user_functions) :
      QGLWidget(c_format, pc_parent),
      pcMainWindow(pc_main_window),
      m_cUserFunctions(c_user_functions),
      nTimerId(-1),
      m_bAntiAliasing(false),
      m_bFastForwarding(false),
      m_nDrawFrameEvery(1),
      m_nFrameCounter(0),
      m_bMouseGrabbed(false),
      m_bShiftPressed(false),
      m_bInvertMouse(false),
      m_cSimulator(CSimulator::GetInstance()),
      m_cSpace(m_cSimulator.GetSpace()),
      m_bUsingFloorTexture(false),
      m_punSelectionBuffer(new GLuint[SELECT_BUFFER_SIZE])
#ifdef QTOPENGL_WITH_SDL
      , m_cJoystick(this)
#endif
   {
      setAutoFillBackground(false);
      QSizePolicy cSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
      cSizePolicy.setHeightForWidth(true);
      setSizePolicy(cSizePolicy);
      setFocusPolicy(Qt::ClickFocus);
      updateGeometry();
      /* Light data */
      m_pfLightAmbient = new GLfloat[4];
      m_pfLightDiffuse = new GLfloat[4];
      m_pfLight0Position = new GLfloat[4];
      m_pfLight1Position = new GLfloat[4];
      m_pfLightAmbient[0] = 0.1f;
      m_pfLightAmbient[1] = 0.1f;
      m_pfLightAmbient[2] = 0.1f;
      m_pfLightAmbient[3] = 1.0f;
      m_pfLightDiffuse[0] = 0.6f;
      m_pfLightDiffuse[1] = 0.6f;
      m_pfLightDiffuse[2] = 0.6f;
      m_pfLightDiffuse[3] = 1.0f;
      m_pfLight0Position[0] = 50.0f;
      m_pfLight0Position[1] = 50.0f;
      m_pfLight0Position[2] = 2.0f;
      m_pfLight0Position[3] = 1.0f;
      m_pfLight1Position[0] = -50.0f;
      m_pfLight1Position[1] = -50.0f;
      m_pfLight1Position[2] = 2.0f;
      m_pfLight1Position[3] = 1.0f;
      /* Keys */
      m_mapPressedKeys[DIRECTION_UP]        = false;
      m_mapPressedKeys[DIRECTION_DOWN]      = false;
      m_mapPressedKeys[DIRECTION_LEFT]      = false;
      m_mapPressedKeys[DIRECTION_RIGHT]     = false;
      m_mapPressedKeys[DIRECTION_FORWARDS]  = false;
      m_mapPressedKeys[DIRECTION_BACKWARDS] = false;
#ifdef QTOPENGL_WITH_SDL
      /* Joystick support */
      if(m_cJoystick.connected()) {
         m_cJoystick.open(0);
      }
#endif
   }

   /****************************************/
   /****************************************/

   CQTOpenGLWidget::~CQTOpenGLWidget() {
#ifdef QTOPENGL_WITH_SDL
      m_cJoystick.close();
#endif
      deleteTexture(m_unGroundTexture);
      glDeleteLists(1, m_unArenaList);
      if(m_bUsingFloorTexture) {
         deleteTexture(m_unFloorTexture);
         glDeleteLists(1, m_unFloorList);
      }
      delete[] m_punSelectionBuffer;
      delete[] m_pfLightAmbient;
      delete[] m_pfLightDiffuse;
      delete[] m_pfLight0Position;
      delete[] m_pfLight1Position;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::initializeGL() {
      /* Antialiasing */
      m_bAntiAliasing = format().sampleBuffers();
      /* Set up the texture parameters for the floor plane
         (here we refer to the standard floor, not the floor entity) */
      QImage cGroundTexture(pcMainWindow->GetTextureDir() + "/ground.png");
      m_unGroundTexture = bindTexture(cGroundTexture,
                                      GL_TEXTURE_2D,
                                      GL_RGB,
                                      QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption);
#ifdef ARGOS_WITH_FREEIMAGE
      /* Now take care of the floor entity */
      try {
         /* Create an image to use as texture */
         m_cSpace.GetFloorEntity().SaveAsImage("/tmp/argos_floor.png");
         m_bUsingFloorTexture = true;
         /* Use the image as texture */
         m_unFloorTexture = bindTexture(QImage("/tmp/argos_floor.png"),
                                        GL_TEXTURE_2D,
                                        GL_RGB,
                                        QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption);
         m_cSpace.GetFloorEntity().ClearChanged();
      }
      catch(CARGoSException& ex) {}
#endif
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::paintEvent(QPaintEvent*) {
      if(!isValid()) return;
      DrawScene();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::DrawAxes() {
      // @TODO
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::DrawScene() {
      m_fFPS = 1e9 / m_cElapsedTimer.nsecsElapsed();
      makeCurrent();
      resizeGL(width(), height());
      glShadeModel(GL_SMOOTH);
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_LIGHTING);
      glEnable(GL_CULL_FACE);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
      glEnable(GL_DEPTH_TEST);
      qglClearColor(Qt::darkCyan);
      glClearAccum(0.0, 0.0, 0.0, 0.0);
      /* Place the lights */
      glLightfv(GL_LIGHT0, GL_AMBIENT,  m_pfLightAmbient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE,  m_pfLightDiffuse);
      glLightfv(GL_LIGHT0, GL_POSITION, m_pfLight0Position);
      glLightfv(GL_LIGHT1, GL_AMBIENT,  m_pfLightAmbient);
      glLightfv(GL_LIGHT1, GL_DIFFUSE,  m_pfLightDiffuse);
      glLightfv(GL_LIGHT1, GL_POSITION, m_pfLight1Position);
      glEnable(GL_LIGHT0);
      glEnable(GL_LIGHT1);
      /* Calculate the perspective */
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(m_cCamera.GetActiveSettings().YFieldOfView.GetValue(),
                     ASPECT_RATIO,
                     0.1f, 1000.0f);
      /* Set up the right matrix mode */
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      /* Clear color buffer and depth buffer */
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      /* Place the camera */
      m_cCamera.Look();
      /* Switch anti-aliasing on */
      if(m_bAntiAliasing) {
         glEnable(GL_MULTISAMPLE);
      }
      /* Draw the arena */
      DrawArena();
      /* Draw the objects */
      CEntity::TVector& vecEntities = m_cSpace.GetRootEntityVector();
      for(CEntity::TVector::iterator itEntities = vecEntities.begin();
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
         CallEntityOperation<CQTOpenGLOperationDrawSelected, CQTOpenGLWidget, void>(*this, *vecEntities[m_sSelectionInfo.Index]);
         glPopMatrix();
      }
      /* Draw in world */
      glPushMatrix();
      m_cUserFunctions.DrawInWorld();
      glPopMatrix();
      /* Switch anti-aliasing off */
      if(m_bAntiAliasing) {
         glDisable(GL_MULTISAMPLE);
      }
      /* Draw axes */
      DrawAxes();
      /* Execute overlay drawing */
      glShadeModel(GL_FLAT);
      glDisable(GL_LIGHTING);
      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);
      glMatrixMode(GL_MODELVIEW);
      QPainter cPainter(this);
      if(m_bAntiAliasing) {
         cPainter.setRenderHint(QPainter::Antialiasing);
         cPainter.setRenderHint(QPainter::TextAntialiasing);
      }
      m_cUserFunctions.DrawOverlay(cPainter);
      // cPainter.drawText(rect(), QString("%1 FPS").arg(m_fFPS, 0, 'f', 0));
      cPainter.end();
      /* Grab frame, if necessary */
      if(m_sFrameGrabData.Grabbing) {
         QString strFileName = QString("%1/%2%3.%4")
            .arg(m_sFrameGrabData.Directory)
            .arg(m_sFrameGrabData.BaseName)
            .arg(m_cSpace.GetSimulationClock(), 5, 10, QChar('0'))
            .arg(m_sFrameGrabData.Format);
         QToolTip::showText(pos() + geometry().center(), "Stored frame to \"" + strFileName);
         grabFrameBuffer()
            .save(
               strFileName,
               0,
               m_sFrameGrabData.Quality);
      }
      m_cElapsedTimer.restart();
   }

   /****************************************/
   /****************************************/

   CEntity* CQTOpenGLWidget::GetSelectedEntity() {
      return (m_sSelectionInfo.IsSelected ?
              m_cSpace.GetRootEntityVector()[m_sSelectionInfo.Index] :
              NULL);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SelectEntity(CEntity& c_entity) {
      /* Look for the idx corresponding to the entity */
      size_t unIdx = 0;
      while(m_cSpace.GetRootEntityVector()[unIdx] != &c_entity)
         ++unIdx;
      /* Check whether an entity had previously been selected */
      if(m_sSelectionInfo.IsSelected) {
         /* An entity had previously been selected */
         /* Is that entity already selected? */
         if(m_sSelectionInfo.Index == unIdx) return;
         /* Deselect the previous one */
         emit EntityDeselected(m_sSelectionInfo.Index);
         m_cUserFunctions.EntityDeselected(
            *m_cSpace.GetRootEntityVector()[m_sSelectionInfo.Index]);
      }
      else {
         /* No entity had previously been selected */
         m_sSelectionInfo.IsSelected = true;
      }
      /* Select the new entity */
      m_sSelectionInfo.Index = unIdx;
      emit EntitySelected(unIdx);
      m_cUserFunctions.EntitySelected(
         *m_cSpace.GetRootEntityVector()[unIdx]);
      DrawScene();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::DeselectEntity() {
      /* If no entity was selected, nothing to do */
      if(!m_sSelectionInfo.IsSelected) return;
      /* Deselect the entity */
      emit EntityDeselected(m_sSelectionInfo.Index);
      m_cUserFunctions.EntityDeselected(
         *m_cSpace.GetRootEntityVector()[m_sSelectionInfo.Index]);
      m_sSelectionInfo.IsSelected = false;
      DrawScene();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SelectInScene(UInt32 un_x,
                                       UInt32 un_y) {
      /* Used to store the viewport size */
      GLint nViewport[4];
      /* Set the selection buffer */
      glSelectBuffer(SELECT_BUFFER_SIZE, m_punSelectionBuffer);
      /* Switch to select mode */
      glRenderMode(GL_SELECT);
      /* Set the projection matrix */
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      /* Set the viewport */
      glGetIntegerv(GL_VIEWPORT, nViewport);
      gluPickMatrix(un_x,
                    nViewport[3]-un_y,
                    5, 5,
                    nViewport);
      gluPerspective(m_cCamera.GetActiveSettings().YFieldOfView.GetValue(),
                     ASPECT_RATIO,
                     0.1f, 1000.0f);
      glMatrixMode(GL_MODELVIEW);
      /* Prepare name stack */
      glInitNames();
      /* Draw the objects */
      CEntity::TVector& vecEntities = m_cSpace.GetRootEntityVector();
      for(size_t i = 0; i < vecEntities.size(); ++i) {
         glPushName(i);
         glPushMatrix();
         CallEntityOperation<CQTOpenGLOperationDrawNormal, CQTOpenGLWidget, void>(*this, *vecEntities[i]);
         glPopMatrix();
         glPopName();
      }
      /* Restore the original projection matrix */
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glFlush();
      /* Return to normal rendering mode and get hit count */
      bool bWasSelected = m_sSelectionInfo.IsSelected;
      UInt32 unHits = glRenderMode(GL_RENDER);
      if (unHits == 0) {
         /* No hits, deselect what was selected */
         m_sSelectionInfo.IsSelected = false;
         if(bWasSelected) {
            emit EntityDeselected(m_sSelectionInfo.Index);
            m_cUserFunctions.EntityDeselected(
               *m_cSpace.GetRootEntityVector()[m_sSelectionInfo.Index]);
         }
      }
      else {
         /* There are hits!
          * Process them, keeping the closest hit
          */
         GLuint unNames;
         GLuint* punByte;
         GLuint unMinZ;
         GLuint* punName;
         punByte = m_punSelectionBuffer;
         unMinZ = 0xffffffff;
         for (UInt32 i = 0; i < unHits; i++) {	
            unNames = *punByte;
            ++punByte;
            if (*punByte < unMinZ) {
               unMinZ = *punByte;
               punName = punByte+2;
            }
            punByte += unNames+2;
         }
         /* Now *punName contains the closest hit */
         if(bWasSelected &&
            (m_sSelectionInfo.Index == *punName)) {
            /* The user clicked on the selected entity, deselect it */
            emit EntityDeselected(m_sSelectionInfo.Index);
            m_sSelectionInfo.IsSelected = false;
            m_cUserFunctions.EntitySelected(
               *m_cSpace.GetRootEntityVector()[m_sSelectionInfo.Index]);
         }
         if(bWasSelected &&
            (m_sSelectionInfo.Index != *punName)) {
            /* The user clicked on a different entity from the selected one */
            emit EntityDeselected(m_sSelectionInfo.Index);
            m_cUserFunctions.EntityDeselected(
               *m_cSpace.GetRootEntityVector()[m_sSelectionInfo.Index]);
            m_sSelectionInfo.Index = *punName;
            emit EntitySelected(m_sSelectionInfo.Index);
            m_cUserFunctions.EntitySelected(
               *m_cSpace.GetRootEntityVector()[m_sSelectionInfo.Index]);
         }
         else {
            /* There was nothing selected, and the user clicked on an entity */
            m_sSelectionInfo.IsSelected = true;
            m_sSelectionInfo.Index = *punName;
            emit EntitySelected(m_sSelectionInfo.Index);
            m_cUserFunctions.EntitySelected(
               *m_cSpace.GetRootEntityVector()[m_sSelectionInfo.Index]);
         }
      }
      DrawScene();
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
      glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
      /* Second, rotate the entity */
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
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
      glTranslatef(cPosition.GetX(), cPosition.GetY(), cPosition.GetZ());
      /* Second, rotate the entity */
      glRotatef(ToDegrees(cXAngle).GetValue(), 1.0f, 0.0f, 0.0f);
      glRotatef(ToDegrees(cYAngle).GetValue(), 0.0f, 1.0f, 0.0f);
      glRotatef(ToDegrees(cZAngle).GetValue(), 0.0f, 0.0f, 1.0f);
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
            glVertex3f(cStart.GetX(), cStart.GetY(), cStart.GetZ());
            glVertex3f(cEnd.GetX(), cEnd.GetY(), cEnd.GetZ());
         }
         glEnd();
         glPointSize(5.0);
         glColor3f(0.0, 0.0, 0.0);
         glBegin(GL_POINTS);
         for(UInt32 i = 0; i < c_entity.GetIntersectionPoints().size(); ++i) {
            const CVector3& cPoint = c_entity.GetIntersectionPoints()[i];
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());
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
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      /* Top face */
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glEnd();
      /* This part covers the faces (South, East, North, West) */
      glBegin(GL_QUADS);
      /* South face */
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      /* East face */
      glNormal3f(0.0f, -1.0f, 0.0f);
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      /* North face */
      glNormal3f(1.0f, 0.0f, 0.0f);
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MinCorner.GetY(), sBBox.MaxCorner.GetZ());
      /* West face */
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      glVertex3f(sBBox.MinCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MaxCorner.GetZ());
      glVertex3f(sBBox.MaxCorner.GetX(), sBBox.MaxCorner.GetY(), sBBox.MinCorner.GetZ());
      glEnd();
      glEnable(GL_LIGHTING);
      glLineWidth(1.0f);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::resizeGL(int n_width,
                                  int n_height) {
      glViewport(0, 0, n_width, n_height);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::PlayExperiment() {
      m_bFastForwarding = false;
      if(nTimerId != -1) killTimer(nTimerId);
      nTimerId = startTimer(CPhysicsEngine::GetSimulationClockTick() * 1000.0f);
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
               DrawScene();
            }
            ++m_nFrameCounter;
         } else {
            DrawScene();
         }
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
      deleteTexture(m_unGroundTexture);
      if(m_bUsingFloorTexture) deleteTexture(m_unFloorTexture);
      initializeGL();
      DrawScene();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SetAntiAliasing(bool b_antialias_on) {
      QGLFormat cGLFormat = format();
      cGLFormat.setSampleBuffers(b_antialias_on);
      setFormat(cGLFormat);
      m_bAntiAliasing = b_antialias_on;
      DrawScene();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SetDrawFrameEvery(SInt32 n_every) {
      m_nDrawFrameEvery = n_every;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SetGrabFrame(bool b_grab_on) {
      m_sFrameGrabData.Grabbing = b_grab_on;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SetCamera(int n_camera) {
      m_cCamera.SetActiveSettings(n_camera);
      DrawScene();
      QToolTip::showText(pos() + geometry().center(), QString("Current camera: #%1").arg(n_camera+1));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::SetCameraFocalLength(double f_length) {
      m_cCamera.GetActiveSettings().LensFocalLength = f_length / 1000.0f;
      m_cCamera.GetActiveSettings().CalculateYFieldOfView();
      m_cCamera.GetActiveSettings().CalculateSensitivity();
      QToolTip::showText(pos() + geometry().center(), QString("Motion sens = %1").arg(m_cCamera.GetActiveSettings().MotionSensitivity));
      DrawScene();
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
            QGLWidget::keyPressEvent(pc_event);
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
            QGLWidget::keyPressEvent(pc_event);
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
            deleteTexture(m_unFloorTexture);
            /* Create an image to use as texture */
            m_cSpace.GetFloorEntity().SaveAsImage("/tmp/argos_floor.png");
            m_unFloorTexture = bindTexture(QImage("/tmp/argos_floor.png"),
                                           GL_TEXTURE_2D,
                                           GL_RGB,
                                           QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption);
            /* Clear the 'changed' flag on the floor entity */
            m_cSpace.GetFloorEntity().ClearChanged();
         }
         glBindTexture(GL_TEXTURE_2D, m_unFloorTexture);
         /* Draw the floor entity along with its texture */
         glBegin(GL_QUADS);
         glTexCoord2d(0.0f, 1.0f); glVertex3f(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), 0.0f);
         glTexCoord2d(1.0f, 1.0f); glVertex3f(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), 0.0f);
         glTexCoord2d(1.0f, 0.0f); glVertex3f(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), 0.0f);
         glTexCoord2d(0.0f, 0.0f); glVertex3f(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), 0.0f);
         glEnd();
      }
      else {
#endif
         /* Wrap the texture at the edges, which in this case means that it is repeated */
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         glBindTexture(GL_TEXTURE_2D, m_unGroundTexture);
         /* Draw the floor along with its texture */
         glBegin(GL_QUADS);
         glTexCoord2f(0.0f, cArenaSize.GetY());              glVertex3f(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), 0.0f);
         glTexCoord2f(cArenaSize.GetX(), cArenaSize.GetY()); glVertex3f(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), 0.0f);
         glTexCoord2f(cArenaSize.GetX(), 0.0f);              glVertex3f(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), 0.0f);
         glTexCoord2f(0.0f, 0.0f);                           glVertex3f(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), 0.0f);
         glEnd();
#ifdef ARGOS_WITH_FREEIMAGE
      }
#endif
      /* Disable the textures */
      glDisable(GL_TEXTURE_2D);
      /* Draw walls */
      glDisable(GL_CULL_FACE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glLineWidth(3.0f);
      glColor3f(0.0f, 0.0f, 0.0f);
      /* This part covers the top and bottom faces (parallel to XY) */
      glBegin(GL_QUADS);
      /* Top face */
      glVertex3f(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
      glVertex3f(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
      glVertex3f(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
      glVertex3f(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
      glEnd();
      /* This part covers the faces (South, East, North, West) */
      glBegin(GL_QUADS);
      /* South face */
      glVertex3f(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), cArenaMinCorner.GetZ());
      glVertex3f(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
      glVertex3f(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
      glVertex3f(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMinCorner.GetZ());
      /* East face */
      glVertex3f(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), cArenaMinCorner.GetZ());
      glVertex3f(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), cArenaMinCorner.GetZ());
      glVertex3f(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
      glVertex3f(cArenaMinCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
      /* North face */
      glVertex3f(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), cArenaMinCorner.GetZ());
      glVertex3f(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMinCorner.GetZ());
      glVertex3f(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
      glVertex3f(cArenaMaxCorner.GetX(), cArenaMinCorner.GetY(), cArenaMaxCorner.GetZ());
      /* West face */
      glVertex3f(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMinCorner.GetZ());
      glVertex3f(cArenaMinCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
      glVertex3f(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMaxCorner.GetZ());
      glVertex3f(cArenaMaxCorner.GetX(), cArenaMaxCorner.GetY(), cArenaMinCorner.GetZ());
      glEnd();
      glLineWidth(1.0f);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glEnable(GL_CULL_FACE);
      /* Restore lighting */
      glEnable(GL_LIGHTING);
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
         m_bMouseGrabbed = true;
         m_cMouseGrabPos = pc_event->pos();
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
         CEmbodiedEntity* pcEntity = dynamic_cast<CEmbodiedEntity*>(
            m_cSpace.GetRootEntityVector()[m_sSelectionInfo.Index]);
         if(pcEntity == NULL) {
            /* Treat selected entity as a composable entity with an embodied component */
            CComposableEntity* pcCompEntity = dynamic_cast<CComposableEntity*>(
               m_cSpace.GetRootEntityVector()[m_sSelectionInfo.Index]);
            if(pcCompEntity->HasComponent("body")) {
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
         CRay3 cMouseRay = GetCamera().
            ProjectRayFromMousePosIntoWorld(pc_event->pos().x(),
                                            pc_event->pos().y());
         /* Calculate the new entity position as the intersection of the projected mouse position
            with the plane created before */
         CVector3 cNewPos;
         if(cMouseRay.Intersects(cXYPlane, cNewPos)) {
            pcEntity->MoveTo(cNewPos,
                             pcEntity->GetOriginAnchor().Orientation);
            /* Entity moved, redraw */
            DrawScene();
         }
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
               DrawScene();
            }
            else if(pc_event->buttons() == Qt::RightButton) {
               QPoint cDelta(pc_event->pos() - m_cMouseGrabPos);
               m_cCamera.Move(-cDelta.y(), cDelta.x(), 0);
               m_cMouseGrabPos = pc_event->pos();
               DrawScene();
            }
            else if(pc_event->buttons() == Qt::MidButton) {
               QPoint cDelta(pc_event->pos() - m_cMouseGrabPos);
               m_cCamera.Move(0, 0, cDelta.y());
               m_cMouseGrabPos = pc_event->pos();
               DrawScene();
            }
         }
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
         DrawScene();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::resizeEvent(QResizeEvent* pc_event) {
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
      }
   }

   /****************************************/
   /****************************************/

}
