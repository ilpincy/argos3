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

      inline const QString& GetIconDir() const {
         return m_strIconDir;
      }

      inline const QString& GetTextureDir() const {
         return m_strTextureDir;
      }

   private:

      void ReadSettingsPreCreation();
      void ReadSettingsPostCreation();
      void WriteSettings();

      void CreateSimulationActions();
      void CreateCameraActions();
      void CreatePOVRayActions();
      void CreateHelpActions();

      void CreateSimulationToolBar();
      void CreateSimulationMenu();
      void CreateCameraToolBar();
      void CreateCameraMenu();
      void CreatePOVRayMenu();
      void CreateHelpMenu();

      void CreateOpenGLWidget(TConfigurationNode& t_tree);

      void CreateLogMessageDock();

      void CreateConnections();

      CQTOpenGLUserFunctions* CreateUserFunctions(TConfigurationNode& t_tree);

      virtual void closeEvent(QCloseEvent* pc_event);

   signals:

      void CameraSwitched(int);
      void SimulationReset();

   public slots:

      void StopSimulation();
      void SimulationCanProceed(bool);
      void SimulationDone();
      void ResetSimulation();
      void CameraXMLPopUp();
      QString GetCameraXMLData();
      // void POVRaySceneXMLPopUp();
      // QString GetPOVRaySceneXMLData();
      // void POVRayScenePreview();
      void SwitchCamera(QAction*);

   private:

      CQTOpenGLWidget* m_pcOpenGLWidget;
      QString m_strIconDir;
      QString m_strTextureDir;

      QAction* m_pcPlayAction;
      QAction* m_pcStepAction;
      QAction* m_pcFastForwardAction;
      QAction* m_pcResetAction;
      QAction* m_pcCaptureAction;
      QAction* m_pcQuitAction;
      QSpinBox* m_pcDrawFrameEvery;
      QLCDNumber* m_pcCurrentStepLCD;
      QToolBar* m_pcSimulationToolBar;
      QMenu* m_pcSimulationMenu;

      QAction* m_pcToggleAntiAliasingAction;
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
