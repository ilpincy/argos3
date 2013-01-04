/**
 * @file <simulator/visualizations/qtopengl_widget.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_widget.h"
#include "qtopengl_user_functions.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/floor_entity.h>

#include <QDir>
#include <QToolTip>
#include <QTimerEvent>
#include <QMouseEvent>
#include <QPainter>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

namespace argos {

   static const Real ASPECT_RATIO = 4.0f / 3.0f;

   /****************************************/
   /****************************************/
   
   CQTOpenGLWidget::CQTOpenGLWidget(QWidget* pc_parent,
                                    CQTOpenGLUserFunctions& c_user_functions) :
      QGLWidget(pc_parent),
      m_cUserFunctions(c_user_functions),
      nTimerId(-1),
      m_bAntiAliasing(false),
      m_bFastForwarding(false),
      m_nDrawFrameEvery(1),
      m_nFrameCounter(0),
      m_bMouseGrabbed(false),
      m_bInvertMouse(false),
      m_cSimulator(CSimulator::GetInstance()),
      m_cSpace(m_cSimulator.GetSpace()),
      m_bUsingFloorTexture(false)
#ifdef QTOPENGL_WITH_SDL
      , m_cJoystick(this)
#endif
   {
      m_cUserFunctions.SetOpenGLWidget(*this);
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
      /* Antialiasing */
      m_bAntiAliasing = format().sampleBuffers();
#ifdef QTOPENGL_WITH_SDL
      /* Joystick support */
      if(m_cJoystick.connected()) {
         m_cJoystick.open(0);
      }
#endif
      /* Set the texture directory */
      std::string strSTDBaseDirectory(CSimulator::GetInstance().GetInstallationDirectory());
      strSTDBaseDirectory += "/simulator/visualizations/qt-opengl/textures/";
      m_strBaseDirectory = strSTDBaseDirectory.c_str();
      /* Initialize the arena */
      makeCurrent();
      initializeGL();
      resizeGL(width(), height());
      InitializeArena();
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

      delete[] m_pfLightAmbient;
      delete[] m_pfLightDiffuse;
      delete[] m_pfLight0Position;
      delete[] m_pfLight1Position;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::initializeGL() {
      glShadeModel(GL_SMOOTH);
      glEnable(GL_LIGHTING);
      glEnable(GL_CULL_FACE);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
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
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::paintEvent(QPaintEvent*) {
      if(!isValid()) return;
      DrawScene();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::DrawScene() {
      makeCurrent();
      resizeGL(width(), height());
      initializeGL();
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
      /* Draw the arena */
      if(m_bAntiAliasing) {
         glEnable(GL_MULTISAMPLE);
      }
      glPushMatrix();
      glCallList(m_unArenaList);
      if(m_bUsingFloorTexture) {
         if(m_cSpace.GetFloorEntity().HasChanged()) {
            deleteTexture(m_unFloorTexture);
            /* Create an image to use as texture */
            m_cSpace.GetFloorEntity().SaveAsImage("/tmp/argos_floor.png");
            /* Use the image as texture */
            m_unFloorTexture = bindTexture(QImage("/tmp/argos_floor.png"));
            /* Clear the 'changed' flag on the floor entity */
            m_cSpace.GetFloorEntity().ClearChanged();
         }
         else {
            glBindTexture(GL_TEXTURE_2D, m_unFloorTexture);
         }
         glCallList(m_unFloorList);
      }
      glPopMatrix();
      /* Draw the objects */
      CEntity::TVector& vecEntities = m_cSpace.GetEntityVector();
      for(CEntity::TVector::iterator itEntities = vecEntities.begin();
          itEntities != vecEntities.end();
          ++itEntities) {
         glPushMatrix();
         /** @todo call function to draw entity */
         //itEntities->Accept(*m_pcVisitor);
         glPopMatrix();
      }
      if(m_bAntiAliasing) {
         glDisable(GL_MULTISAMPLE);
      }
      glPushMatrix();
      m_cUserFunctions.DrawInWorld();
      glPopMatrix();
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
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::resizeGL(int n_width,
                                  int n_height) {
      glViewport(0, 0, n_width, n_height);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::PlayPauseSimulation(bool b_play) {
      m_bFastForwarding = false;
      if(b_play) {
         if(nTimerId != -1) killTimer(nTimerId);
         nTimerId = startTimer(100);
      }
      else {
         killTimer(nTimerId);
         nTimerId = -1;
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::FastForwardPauseSimulation(bool b_play) {
      m_nFrameCounter = 0;
      if(b_play) {
         m_bFastForwarding = true;
         if(nTimerId != -1) killTimer(nTimerId);
         nTimerId = startTimer(1);
      }
      else {
         m_bFastForwarding = false;
         killTimer(nTimerId);
         nTimerId = -1;
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::StepSimulation() {
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
         killTimer(nTimerId);
         nTimerId = -1;
         emit SimulationDone();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::ResetSimulation() {
      m_cSimulator.Reset();
      InitializeArena();
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

   void CQTOpenGLWidget::InitializeArena() {
      const Real fFloorSide      = 1000.0f;
      const Real fFloorElevation = -0.001f;

      /* Create a display list for faster drawing */
      m_unArenaList = glGenLists(1);
      glNewList(m_unArenaList, GL_COMPILE);
      /* Disable lighting - no funny color effects */
      glDisable(GL_LIGHTING);
      /* Set up the texture parameters for the floor
         (here we refer to the standard floor, not the floor entity) */
      QImage cGroundTexture(QString(m_strBaseDirectory + "ground.png"));
      m_unGroundTexture = bindTexture(cGroundTexture);
      /* The texture covers the object like a decal */
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
      /* Bilinear filtering for zooming in and out */
      gluBuild2DMipmaps(GL_TEXTURE_2D, 3, cGroundTexture.width(), cGroundTexture.height(),
                        GL_LUMINANCE, GL_UNSIGNED_BYTE, cGroundTexture.bits());
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      /* Wrap the texture at the edges, which in this case means that it is repeated */
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      /* Draw the floor along with its texture */
      glEnable(GL_TEXTURE_2D);
      glBegin(GL_QUADS);
      glTexCoord2d(                0.5f,                 0.5f); glVertex3f(-fFloorSide, -fFloorSide, fFloorElevation);
      glTexCoord2d(2.0f*fFloorSide+0.5f,                 0.5f); glVertex3f( fFloorSide, -fFloorSide, fFloorElevation);
      glTexCoord2d(2.0f*fFloorSide+0.5f, 2.0f*fFloorSide+0.5f); glVertex3f( fFloorSide,  fFloorSide, fFloorElevation);
      glTexCoord2d(                0.5f, 2.0f*fFloorSide+0.5f); glVertex3f(-fFloorSide,  fFloorSide, fFloorElevation);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      /* Restore lighting */
      glEnable(GL_LIGHTING);
      /* The list is done */
      glEndList();

      /* Now take care of the floor entity */
      try {
         /* Create an image to use as texture */
         m_cSpace.GetFloorEntity().SaveAsImage("/tmp/argos_floor.png");
         m_bUsingFloorTexture = true;
         /* Use the image as texture */
         m_unFloorTexture = bindTexture(QImage("/tmp/argos_floor.png"));
         /* Draw the floor entity along with its texture */
         CVector3 cHalfArenaSize = m_cSpace.GetArenaSize() * 0.5f;
         /* Create a display list for faster drawing */
         m_unFloorList = glGenLists(1);
         glNewList(m_unFloorList, GL_COMPILE);
         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
         /* Disabling the depth test solves an annoying glitch:
            since the floor and the floor entity are very close, the
            textures flicker */
         glDisable(GL_DEPTH_TEST);
         glEnable(GL_TEXTURE_2D);
         glBegin(GL_QUADS);
         glTexCoord2d(0.0f, 0.0f); glVertex3f(-cHalfArenaSize.GetX(), -cHalfArenaSize.GetY(), 0.0f);
         glTexCoord2d(1.0f, 0.0f); glVertex3f( cHalfArenaSize.GetX(), -cHalfArenaSize.GetY(), 0.0f);
         glTexCoord2d(1.0f, 1.0f); glVertex3f( cHalfArenaSize.GetX(),  cHalfArenaSize.GetY(), 0.0f);
         glTexCoord2d(0.0f, 1.0f); glVertex3f(-cHalfArenaSize.GetX(),  cHalfArenaSize.GetY(), 0.0f);
         glEnd();
         glDisable(GL_TEXTURE_2D);
         /* Restore depth test */
         glEnable(GL_DEPTH_TEST);
         /* Restore lighting */
         glEnable(GL_LIGHTING);
         /* The list is done */
         glEndList();
      }
      catch(CARGoSException& ex) {
         LOGERR << "[WARNING] No floor entity specified, the floor will be blank" << std::endl;
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::timerEvent(QTimerEvent* pc_event) {
      StepSimulation();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::mouseMoveEvent(QMouseEvent* pc_event) {
      if(m_bMouseGrabbed) {
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

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::mousePressEvent(QMouseEvent* pc_event) {
      m_bMouseGrabbed = true;
      m_cMouseGrabPos = pc_event->pos();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::mouseReleaseEvent(QMouseEvent* pc_event) {
      m_bMouseGrabbed = false;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLWidget::keyPressEvent(QKeyEvent* pc_event) {
      switch(pc_event->key()) {
         case Qt::Key_W:
         case Qt::Key_Up:
            /* Forwards */
            m_mapPressedKeys[DIRECTION_UP] = true;
            reactToKeyEvent();
            break;
         case Qt::Key_S:
         case Qt::Key_Down:
            /* Backwards */
            m_mapPressedKeys[DIRECTION_DOWN] = true;
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
            m_mapPressedKeys[DIRECTION_FORWARDS] = true;
            reactToKeyEvent();
            break;
         case Qt::Key_Q:
            /* Up */
            m_mapPressedKeys[DIRECTION_BACKWARDS] = true;
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

   void CQTOpenGLWidget::keyReleaseEvent(QKeyEvent* pc_event) {
      QGLWidget::keyPressEvent(pc_event);
      switch(pc_event->key()) {
         case Qt::Key_Up:
            /* Forwards */
            m_mapPressedKeys[DIRECTION_UP] = false;
            reactToKeyEvent();
            break;
         case Qt::Key_Down:
            /* Backwards */
            m_mapPressedKeys[DIRECTION_DOWN] = false;
            reactToKeyEvent();
            break;
         case Qt::Key_Left:
            /* Left */
            m_mapPressedKeys[DIRECTION_LEFT] = false;
            reactToKeyEvent();
            break;
         case Qt::Key_Right:
            /* Right */
            m_mapPressedKeys[DIRECTION_RIGHT] = false;
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

   void CQTOpenGLWidget::resizeEvent(QResizeEvent* pc_event) {
      QToolTip::showText(pos() + geometry().center(), QString("Size: %1 x %2").arg(pc_event->size().width()).arg(pc_event->size().height()));
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
         m_cCamera.Move(nForwardsBackwards, nSideways, nUpDown);
         DrawScene();
      }
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
