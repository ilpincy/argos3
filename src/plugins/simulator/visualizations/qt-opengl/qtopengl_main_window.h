/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_main_window.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_MAIN_WINDOW_H
#define QTOPENGL_MAIN_WINDOW_H

namespace argos {
   class CQTOpenGLMainWindow;
   class CQTOpenGLWidget;
   class CQTOpenGLLogStream;
   class CQTOpenGLUserFunctions;
}

class QMainWindow;
class QWidget;
class QHBoxLayout;
class QPushButton;
class QLCDNumber;
class QStatusBar;
class QDockWidget;
class QTextEdit;
class QButtonGroup;
class QSpinBox;
class QDoubleSpinBox;
class QActionGroup;

#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/simulator/visualization/visualization.h>
#include <QMainWindow>

namespace argos {

   class CQTOpenGLMainWindow : public QMainWindow {

      Q_OBJECT

   public:

      CQTOpenGLMainWindow(TConfigurationNode& t_tree);
      virtual ~CQTOpenGLMainWindow();

      inline CQTOpenGLWidget& GetOpenGLWidget() {
         return *m_pcOpenGLWidget;
      }

      inline const CQTOpenGLWidget& GetOpenGLWidget() const {
         return *m_pcOpenGLWidget;
      }

      inline CQTOpenGLUserFunctions& GetUserFunctions() {
         return *m_pcUserFunctions;
      }

      inline const CQTOpenGLUserFunctions& GetUserFunctions() const {
         return *m_pcUserFunctions;
      }

      inline const QString& GetIconDir() const {
         return m_strIconDir;
      }

      inline const QString& GetTextureDir() const {
         return m_strTextureDir;
      }

      inline const QString& GetModelDir() const {
         return m_strModelDir;
      }

   private:

      void ReadSettingsPreCreation();
      void ReadSettingsPostCreation();
      void WriteSettings();

      void CreateExperimentActions();
      void CreateCameraActions();
      void CreatePOVRayActions();
      void CreateHelpActions();

      void CreateExperimentToolBar();
      void CreateExperimentMenu();
      void CreateCameraToolBar();
      void CreateCameraMenu();
      void CreatePOVRayMenu();
      void CreateHelpMenu();

      void CreateOpenGLWidget(TConfigurationNode& t_tree);
      void CreateLogMessageDock();
      void CreateConnections();

      void CreateUserFunctions(TConfigurationNode& t_tree);

      virtual void closeEvent(QCloseEvent* pc_event);

   signals:

      /**
       * Emitted whenever the user presses a camera button to switch camera.
       * @param n_camera The index of the desired camera
       */
      void CameraSwitched(int n_camera);

      /**
       * Emitted when the experiment has just been started.
       */
      void ExperimentStarted();

      /**
       * Emitted when the experiment has (re)started playing.
       */
      void ExperimentPlaying();

      /**
       * Emitted when the experiment has (re)started fast-forwarding.
       */
      void ExperimentFastForwarding();

      /**
       * Emitted when the experiment has been paused.
       * The difference between pausing and suspending an experiment
       * is that pausing results from an action of the user; suspending
       * is caused by an error condition.
       * To resume execution, call PlayExperiment() or FastForwardExperiment().
       */
      void ExperimentPaused();

      /**
       * Emitted when the experiment has been suspended.
       * The difference between pausing and suspending an experiment
       * is that pausing results from an action of the user; suspending
       * is caused by an error condition.
       * To resume execution, call ResumeExperiment().
       */
      void ExperimentSuspended();

      /**
       * Emitted when the experiment has just been resumed.
       * This marks the fact that the error condition has been solved.
       */
      void ExperimentResumed();

      /**
       * Emitted when the experiment is finished.
       * An experiment can finish "naturally", when CSimulator::IsExperimentFinished()
       * returns <tt>true</tt>; or "forcedly", when the user pressed the terminate button.
       */
      void ExperimentDone();

      /**
       * Emitted when the experiment has been reset.
       */
      void ExperimentReset();

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
       * Executes one experiment time step.
       */
      void StepExperiment();

      /**
       * Pauses the experiment.
       * The experiment can be resumed with PlayExperiment() or
       * FastForwardExperiment().
       */
      void PauseExperiment();

      /**
       * Terminates the execution of the experiment.
       */
      void TerminateExperiment();

      /**
       * Resets the state of the experiment to its state right after
       * initialization.
       */
      void ResetExperiment();

      /**
       * Suspends an experiment due to an error.
       */
      void SuspendExperiment();

      /**
       * Resumes a suspended experiment.
       */
      void ResumeExperiment();

      void CameraXMLPopUp();

      QString GetCameraXMLData();

      void SwitchCamera(QAction*);

      // void POVRaySceneXMLPopUp();
      // QString GetPOVRaySceneXMLData();
      // void POVRayScenePreview();

   private:

      enum EExperimentState {
         EXPERIMENT_INITIALIZED = 0,
         EXPERIMENT_PLAYING,
         EXPERIMENT_FAST_FORWARDING,
         EXPERIMENT_PAUSED,
         EXPERIMENT_SUSPENDED,
         EXPERIMENT_DONE
      };

   private:

      CQTOpenGLWidget* m_pcOpenGLWidget;
      QString m_strIconDir;
      QString m_strTextureDir;
      QString m_strModelDir;

      EExperimentState m_eExperimentState;

      QAction* m_pcPlayAction;
      QAction* m_pcFastForwardAction;
      QAction* m_pcStepAction;
      QAction* m_pcResetAction;
      QAction* m_pcTerminateAction;
      QAction* m_pcPauseAction;
      QAction* m_pcCaptureAction;
      QAction* m_pcQuitAction;
      QSpinBox* m_pcDrawFrameEvery;
      QLCDNumber* m_pcCurrentStepLCD;
      QToolBar* m_pcExperimentToolBar;
      QMenu* m_pcExperimentMenu;

      QAction* m_pcShowCameraXMLAction;
      QList<QAction*> m_pcSwitchCameraActions;
      QActionGroup* m_pcSwitchCameraActionGroup;
      QDoubleSpinBox* m_pcFocalLength;
      QToolBar* m_pcCameraToolBar;
      QMenu* m_pcCameraMenu;

      QAction* m_pcPOVRayXMLAction;
      QAction* m_pcPOVRayPreviewAction;
      QMenu* m_pcPOVRayMenu;

      QAction* m_pcAboutQTAction;
      QMenu* m_pcHelpMenu;

      QStatusBar* m_pcStatusbar;

      QDockWidget* m_pcLogDock;
      QDockWidget* m_pcLogErrDock;
      QTextEdit* m_pcDockLogBuffer;
      QTextEdit* m_pcDockLogErrBuffer;
      CQTOpenGLLogStream* m_pcLogStream;
      CQTOpenGLLogStream* m_pcLogErrStream;

      bool m_bWasLogColored;

      CQTOpenGLUserFunctions* m_pcUserFunctions;
   };

}

#endif
