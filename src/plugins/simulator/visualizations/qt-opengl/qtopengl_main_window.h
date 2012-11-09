/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file <simulator/visualizations/qtopengl_main_window.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
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

   public slots:

      void SimulationDone();
      void SimulationReset();
      void CameraXMLPopUp();
      QString GetCameraXMLData();
      // void POVRaySceneXMLPopUp();
      // QString GetPOVRaySceneXMLData();
      // void POVRayScenePreview();
      void SwitchCamera(QAction*);

   private:

      CQTOpenGLWidget* m_pcOpenGLWidget;
      QString m_strIconDir;

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
