/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_WIDGET_H
#define QTOPENGL_WIDGET_H

namespace argos {
   class CQTOpenGLWidget;
   class CQTOpenGLMainWindow;
   class CSpace;
   class CSimulator;
   class CQTOpenGLBox;
   class CQTOpenGLUserFunctions;
   class CPositionalEntity;
   class CControllableEntity;
   class CEmbodiedEntity;
}

#ifdef QTOPENGL_WITH_SDL
#   include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_joystick.h>
#endif

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_camera.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <QGLWidget>
#include <QElapsedTimer>

#ifdef __APPLE__
#include <glu.h>
#else
#include <GL/glu.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawNormal : public CEntityOperation<CQTOpenGLOperationDrawNormal, CQTOpenGLWidget, void> {
   public:
      virtual ~CQTOpenGLOperationDrawNormal() {}
   };

   class CQTOpenGLOperationDrawSelected : public CEntityOperation<CQTOpenGLOperationDrawSelected, CQTOpenGLWidget, void> {
   public:
      virtual ~CQTOpenGLOperationDrawSelected() {}
   };

#define REGISTER_QTOPENGL_ENTITY_OPERATION(ACTION, OPERATION, ENTITY)   \
   REGISTER_ENTITY_OPERATION(ACTION, CQTOpenGLWidget, OPERATION, void, ENTITY);

   /****************************************/
   /****************************************/

   class CQTOpenGLWidget : public QGLWidget {

      Q_OBJECT

    public:

      struct SFrameGrabData {
         bool Grabbing;
         QString Directory;
         QString BaseName;
         QString Format;
         SInt32 Quality;

         SFrameGrabData() :
            Grabbing(false),
            Directory("."),
            BaseName("frame_"),
            Format("png"),
            Quality(-1) {}

         void Init(TConfigurationNode& t_tree);
      };

      struct SSelectionInfo {
         bool IsSelected;
         size_t Index;

         SSelectionInfo() :
            IsSelected(false),
            Index(0) {}
      };

   public:

      CQTOpenGLWidget(const QGLFormat& c_format,
                      QWidget* pc_parent,
                      CQTOpenGLMainWindow* pc_main_window,
                      CQTOpenGLUserFunctions& c_user_functions);
      virtual ~CQTOpenGLWidget();

      virtual void initializeGL();
      virtual void resizeGL(int n_width,
                            int n_height);
      virtual void paintEvent(QPaintEvent*);

      void DrawScene();

      CEntity* GetSelectedEntity();
      void SelectEntity(CEntity& c_entity);
      void DeselectEntity();

      void SelectInScene(UInt32 un_x,
                         UInt32 un_y);

      void DrawEntity(CPositionalEntity& c_entity);
      void DrawEntity(CEmbodiedEntity& c_entity);

      void DrawRays(CControllableEntity& c_entity);

      void DrawBoundingBox(CEmbodiedEntity& c_entity);

      inline virtual int heightForWidth(int w) const {
         return (w * 3) / 4;
      }

      inline virtual QSize sizeHint() const {
         return QSize(1024,768);
      }

      inline virtual QSize minimumSize() const {
         return QSize(320,240);
      }

      inline CQTOpenGLUserFunctions& GetUserFunctions() {
         return m_cUserFunctions;
      }

      inline CQTOpenGLCamera& GetCamera() {
         return m_cCamera;
      }

      inline SFrameGrabData& GetFrameGrabData() {
         return m_sFrameGrabData;
      }

      inline void SetInvertMouse(bool b_InvertMouse) {
    	  m_bInvertMouse = b_InvertMouse;
      }

   signals:

      /**
       * Emitted whenever a time step has been executed.
       * @param n_step The time-step count
       */
      void StepDone(int n_step);

      /**
       * Emitted when the experiment is finished.
       * An experiment can finish "naturally", when CSimulator::IsExperimentFinished()
       * returns <tt>true</tt>; or "forcedly", when the user pressed the terminate button.
       */
      void ExperimentDone();

      /**
       * Emitted when an entity is selected.
       * @param The index of the selected entity
       */
      void EntitySelected(size_t un_index);

      /**
       * Emitted when an entity is deselected.
       * @param The index of the deselected entity
       */
      void EntityDeselected(size_t un_index);

   public slots:

      /**
       * Plays the experiment.
       * Internally sets a timer whose period corresponds to the
       * XML attribute 'ticks_per_second' in the .argos file.
       */
      void PlayExperiment();

      /**
       * Fast forwards the experiment.
       * Internally sets a timer whose period is 1ms.
       */
      void FastForwardExperiment();

      /**
       * Pauses the experiment.
       * The experiment can be resumed with PlayExperiment() or
       * FastForwardExperiment().
       */
      void PauseExperiment();

      /**
       * Executes one experiment time step.
       */
      void StepExperiment();

      /**
       * Resets the state of the experiment to its state right after
       * initialization.
       */
      void ResetExperiment();

      /**
       * Toggles OpenGL anti-aliasing.
       */
      void SetAntiAliasing(bool b_antialias_on);

      /**
       * When fast-forwarding, sets every how many steps a frame must be drawn. 
       */
      void SetDrawFrameEvery(int n_every);

      /**
       * Toggles frame grabbing.
       */
      void SetGrabFrame(bool b_grab_on);

      /**
       * Sets the current camera in use.
       * @param n_camera The index of the wanted camera [0-11]
       */
      void SetCamera(int n_camera);

      /**
       * Sets the focal length of the current camera.
       * @param f_length The desired focal length
       */
      void SetCameraFocalLength(double f_length);

      /**
       * Handles key press events.
       */
      void KeyPressed(QKeyEvent* pc_event);

      /**
       * Handles key release events.
       */
      void KeyReleased(QKeyEvent* pc_event);

   protected:

      void DrawArena();
      void DrawAxes();

      virtual void timerEvent(QTimerEvent* pc_event);
      virtual void mousePressEvent(QMouseEvent* pc_event);
      virtual void mouseReleaseEvent(QMouseEvent* pc_event);
      virtual void mouseMoveEvent(QMouseEvent* pc_event);
      virtual void keyPressEvent(QKeyEvent* pc_event);
      virtual void keyReleaseEvent(QKeyEvent* pc_event);
      virtual void resizeEvent(QResizeEvent* pc_event);
      void reactToKeyEvent();

   private:

      CQTOpenGLMainWindow* pcMainWindow;
      CQTOpenGLUserFunctions& m_cUserFunctions;

      SInt32 nTimerId;
      bool m_bAntiAliasing;
      bool m_bFastForwarding;
      SInt32 m_nDrawFrameEvery;
      SInt32 m_nFrameCounter;

      bool m_bMouseGrabbed;
      bool m_bShiftPressed;
      QPoint m_cMouseGrabPos;
      bool m_bInvertMouse;
      SSelectionInfo m_sSelectionInfo;

      CSimulator& m_cSimulator;
      CSpace& m_cSpace;

      bool   m_bUsingFloorTexture;
      GLuint m_unFloorTexture;
      GLuint m_unGroundTexture;
      GLuint m_unCeilingTexture;
      GLuint m_unBoxTexture;

      GLfloat* m_pfLightAmbient;
      GLfloat* m_pfLightDiffuse;
      GLfloat* m_pfLight0Position;
      GLfloat* m_pfLight1Position;

      GLuint m_unArenaList;
      GLuint m_unFloorList;
      GLuint* m_punSelectionBuffer;

#ifdef QTOPENGL_WITH_SDL
      CQTOpenGLJoystick m_cJoystick;
#endif
      CQTOpenGLCamera m_cCamera;
      SFrameGrabData m_sFrameGrabData;
      QElapsedTimer m_cElapsedTimer;
      qreal m_fFPS;

      enum EDirection {
         DIRECTION_UP = 1,
         DIRECTION_DOWN,
         DIRECTION_LEFT,
         DIRECTION_RIGHT,
         DIRECTION_FORWARDS,
         DIRECTION_BACKWARDS,
      };

      QMap<EDirection, int> m_mapPressedKeys;
   };

}

#endif
