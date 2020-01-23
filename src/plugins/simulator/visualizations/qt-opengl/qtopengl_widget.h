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

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_camera.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

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

   class CQTOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {

    Q_OBJECT

    public:

      /**
       * Data regarding frame grabbing
       */
      struct SFrameGrabData {
         bool GUIGrabbing;          // true when grabbing enabled from GUI
         bool HeadlessGrabbing;     // true when headless grabbing enabled
         UInt32 HeadlessFrameRate;  // Grab every nth frame
         QString Directory;         // output directory
         QString BaseName;          // frame file basename
         QString Format;            // output file format
         SInt32 Quality;            // output quality [0-100]
         QSize  Size;               // Frame size

         SFrameGrabData() :
            GUIGrabbing(false),
            HeadlessGrabbing(false),
            HeadlessFrameRate(1),
            Directory("."),
            BaseName("frame_"),
            Format("png"),
            Quality(-1),
            Size(1600, 1200) {}

         void Init(TConfigurationNode& t_tree);
      };

      /**
       * Data arelated to robot selection
       */
      struct SSelectionInfo {
         bool IsSelected;         // true when an entity is selected
         CEntity* Entity;         // the selected entity

         SSelectionInfo() :
            IsSelected(false),
            Entity(NULL) {}
      };

   public:

      /**
       * Class constructor.
       * @param pc_parent The containing widget.
       * @param pc_main_window A pointer to the main window.
       * @param c_user_functions A reference to the main window.
       */
      CQTOpenGLWidget(QWidget* pc_parent,
                      CQTOpenGLMainWindow& c_main_window,
                      CQTOpenGLUserFunctions& c_user_functions);
      /**
       * Class destructor.
       */
      virtual ~CQTOpenGLWidget();

      /**
       * Called when the GL context must be initialized.  This happens
       * once per runtime, either before the first call to resizeGL()
       * or to paintGL().
       */
      virtual void initializeGL();

      /**
       * Logic for scene drawing.
       */
      virtual void paintGL();

      /**
       * Casts a ray from the given window coordinate.
       * The ray goes from the near clipping plane to the far one.
       */
      CRay3 RayFromWindowCoord(int n_x,
                               int n_y);

      /**
       * Returns the position in the world corresponding to the given window coordinate.
       */
      CVector3 GetWindowCoordInWorld(int n_x,
                                     int n_y);

      /**
       * Returns the currently selected entity, or NULL if none is selected.
       * @return The currently selected entity.
       */
      CEntity* GetSelectedEntity();

      /**
       * Selects the passed entity.
       * @param c_entity The entity to select.
       */
      void SelectEntity(CEntity& c_entity);

      /**
       * Deselects the currently selected entity.
       * If no entity is selected, nothing is done.
       */
      void DeselectEntity();

      /**
       * Selects the entity closest to the camera at the given screen coordinates.
       * @param un_x The screen X coordinate.
       * @param un_y The screen Y coordinate.
       */
      void SelectInScene(UInt32 un_x,
                         UInt32 un_y);

      /**
       * Draws a positional entity.
       */
      void DrawEntity(CPositionalEntity& c_entity);

      /**
       * Draws an embodied entity.
       */
      void DrawEntity(CEmbodiedEntity& c_entity);

      /**
       * Draws a ray.
       */
      void DrawRays(CControllableEntity& c_entity);

      /**
       * Draws the bounding box of an embodied entity.
       */
      void DrawBoundingBox(CEmbodiedEntity& c_entity);

      /**
       * Called internally by Qt to set the aspect ratio.
       */
      inline virtual int heightForWidth(int w) const {
         return (w * 3) / 4;
      }

      /**
       * Called internally by Qt to know the preferred widget size.
       */
      inline virtual QSize sizeHint() const {
         return QSize(1024,768);
      }

      /**
       * Called internally by Qt to know the minimum widget size.
       */
      inline virtual QSize minimumSize() const {
         return QSize(320,240);
      }

      /**
       * Returns a reference to the user functions.
       */
      inline CQTOpenGLUserFunctions& GetUserFunctions() {
         return m_cUserFunctions;
      }

      /**
       * Returns a reference to the camera.
       */
      inline CQTOpenGLCamera& GetCamera() {
         return m_cCamera;
      }

      /**
       * Returns the current frame grabbing data.
       */
      inline SFrameGrabData& GetFrameGrabData() {
         return m_sFrameGrabData;
      }

      /**
       * Sets whether the mouse should be inverted when moving.
       */
      inline void SetInvertMouse(bool b_invert_mouse) {
         m_bInvertMouse = b_invert_mouse;
      }

      /**
       * Sets whether the boundary walls should be rendered.
       */
      inline void SetShowBoundary(bool b_show_boundary) {
         m_bShowBoundary = b_show_boundary;
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
       * @param The selected entity
       */
      void EntitySelected(CEntity* pc_entity);

      /**
       * Emitted when an entity is deselected.
       * @param The deselected entity
       */
      void EntityDeselected(CEntity* pc_entity);

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

      void DrawScene();
      void DrawArena();
      void DrawAxes();

      virtual void timerEvent(QTimerEvent* pc_event);
      virtual void mousePressEvent(QMouseEvent* pc_event);
      virtual void mouseReleaseEvent(QMouseEvent* pc_event);
      virtual void mouseMoveEvent(QMouseEvent* pc_event);
      virtual void wheelEvent(QWheelEvent *pc_event);
      virtual void keyPressEvent(QKeyEvent* pc_event);
      virtual void keyReleaseEvent(QKeyEvent* pc_event);
      virtual void resizeEvent(QResizeEvent* pc_event);
      void reactToKeyEvent();

   private:

      /** Reference to the main window */
      CQTOpenGLMainWindow& m_cMainWindow;
      /** Reference to the user functions */
      CQTOpenGLUserFunctions& m_cUserFunctions;

      /** Id used for the step timer */
      SInt32 nTimerId;
      /** True when fast forwarding */
      bool m_bFastForwarding;
      /** Counter for how many frames should be simulated between two renderings */
      SInt32 m_nDrawFrameEvery;
      /** Counter for the current frame */
      SInt32 m_nFrameCounter;

      /** True when the mouse is grabbed by this widget */
      bool m_bMouseGrabbed;
      /** True when shift is pressed */
      bool m_bShiftPressed;
      /** Position of the mouse when first clicked */
      QPoint m_cMouseGrabPos;
      /** Whether to invert the mouse motion */
      bool m_bInvertMouse;
      /** Data on entity selection */
      SSelectionInfo m_sSelectionInfo;

      /** Reference to the simulator state */
      CSimulator& m_cSimulator;
      /** Reference to the space state */
      CSpace& m_cSpace;

      /** True if the boundary walls should be shown */
      bool m_bShowBoundary;

      /** True if using a user-defined texture for the floor */
      bool m_bUsingFloorTexture;
      /** The user-defined floor texture */
      QOpenGLTexture* m_pcFloorTexture;
      /** Default ground texture */
      QOpenGLTexture* m_pcGroundTexture;

      /** Ambient attribute for light */
      GLfloat* m_pfLightAmbient;
      /** Diffuse attribute for light */
      GLfloat* m_pfLightDiffuse;
      /** Position attribute for light 0 */
      GLfloat* m_pfLight0Position;
      /** Position attribute for light 1 */
      GLfloat* m_pfLight1Position;

      /** Display list for arena elements */
      //GLuint m_unArenaList;
      /** Display list for floor elements */
      //GLuint m_unFloorList;

      /** Current state of the camera */
      CQTOpenGLCamera m_cCamera;
      /** Data on frame grabbing */
      SFrameGrabData m_sFrameGrabData;

      /** Current direction of motion */
      enum EDirection {
         DIRECTION_UP = 1,
         DIRECTION_DOWN,
         DIRECTION_LEFT,
         DIRECTION_RIGHT,
         DIRECTION_FORWARDS,
         DIRECTION_BACKWARDS,
      };

      /** Mapping between keys and motion direction */
      QMap<EDirection, int> m_mapPressedKeys;
   };

}

#endif
