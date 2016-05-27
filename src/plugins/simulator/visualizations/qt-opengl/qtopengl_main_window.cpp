/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_main_window.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_widget.h"
#include "qtopengl_log_stream.h"
#include "qtopengl_user_functions.h"
#include "qtopengl_main_window.h"

#include <argos3/core/config.h>
#include <argos3/core/utility/plugins/dynamic_loading.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/loop_functions.h>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include <QLabel>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QToolBar>
#include <QLayout>
#include <QMenuBar>
#include <QSettings>

namespace argos {

   /****************************************/
   /****************************************/

   class CQTOpenGLLayout : public QLayout {

   public:

      CQTOpenGLLayout() :
         m_pcQTOpenGLItem(NULL) {
         setContentsMargins(0, 0, 0, 0);
      }

      virtual ~CQTOpenGLLayout() {
         if(m_pcQTOpenGLItem != NULL) {
            delete m_pcQTOpenGLItem;
         }
      }

      virtual void addItem(QLayoutItem* item) {
         if(m_pcQTOpenGLItem != NULL) {
            delete m_pcQTOpenGLItem;
         }
         m_pcQTOpenGLItem = item;
      }
      virtual int count() const {
         return (m_pcQTOpenGLItem != NULL) ? 1 : 0;
      }

      virtual QLayoutItem* itemAt(int index) const {
         return (index == 0) ? m_pcQTOpenGLItem : NULL;
      }

      virtual QLayoutItem* takeAt(int index) {
         if(index == 0) {
            QLayoutItem* pcRetVal = m_pcQTOpenGLItem;
            m_pcQTOpenGLItem = NULL;
            return pcRetVal;
         }
         else {
            return NULL;
         }
      }

      virtual QSize minimumSize () const {
         return QSize(320,240);
      }

      virtual QSize sizeHint () const {
         return QSize(640,480);
      }

      virtual void setGeometry(const QRect& r) {
         /* Set the layout geometry */
         QLayout::setGeometry(r);
         if(m_pcQTOpenGLItem != NULL) {
            /* Calculate the candidate sizes for the QTOpenGL widget */
            /* One is height-driven */
            QRect cCandidate1(r.x(), r.y(), (r.height() * 4) / 3, r.height());
            /* The other is width-driven */
            QRect cCandidate2(r.x(), r.y(), r.width(), (r.width() * 3) / 4);
            /* Pick the one that fits the rectangle better */
            if(r.contains(cCandidate1)) {
               /* Horizontal padding needed */
               int nPadding = (r.width() - cCandidate1.width()) / 2;
               cCandidate1.translate(nPadding, 0);
               m_pcQTOpenGLItem->setGeometry(cCandidate1);
            }
            else {
               /* Vertical padding needed */
               int nPadding = (r.height() - cCandidate2.height()) / 2;
               cCandidate2.translate(0, nPadding);
               m_pcQTOpenGLItem->setGeometry(cCandidate2);
            }
         }
      }

   private:

      QLayoutItem* m_pcQTOpenGLItem;

   };

   /****************************************/
   /****************************************/

   CQTOpenGLMainWindow::CQTOpenGLMainWindow(TConfigurationNode& t_tree) :
      m_pcUserFunctions(NULL) {
      /* Main window settings */
      std::string strTitle;
      GetNodeAttributeOrDefault<std::string>(t_tree, "title", strTitle, "ARGoS v" ARGOS_VERSION "-" ARGOS_RELEASE);
      setWindowTitle(tr(strTitle.c_str()));
      /* Restore settings, if any */
      ReadSettingsPreCreation();
      /* Add a status bar */
      m_pcStatusbar = new QStatusBar(this);
      setStatusBar(m_pcStatusbar);
      /* Create actions */
      CreateExperimentActions();
      CreateCameraActions();
      // CreatePOVRayActions();
      CreateHelpActions();
      /* Create user functions */
      CreateUserFunctions(t_tree);
      /* Create the central widget */
      CreateOpenGLWidget(t_tree);
      /* Create menus */
      CreateExperimentMenu();
      CreateCameraMenu();
      // CreatePOVRayMenu();
      CreateHelpMenu();
      /* Create toolbars */
      CreateExperimentToolBar();
      CreateCameraToolBar();
      /* Create the message dock window */
      CreateLogMessageDock();
      /* Restore settings, if any */
      ReadSettingsPostCreation();
      /* Creates the signal/slot connections */
      CreateConnections();
      /* Set experiment state */
      m_eExperimentState = EXPERIMENT_INITIALIZED;
      /* Should we play instantly? */
      bool bAutoPlay = false;
      GetNodeAttributeOrDefault(t_tree, "autoplay", bAutoPlay, bAutoPlay);
      if(bAutoPlay) {
         PlayExperiment();
      }
   }

   /****************************************/
   /****************************************/

   CQTOpenGLMainWindow::~CQTOpenGLMainWindow() {
      delete m_pcUserFunctions;
      delete m_pcLogStream;
      delete m_pcLogErrStream;
      if(m_bWasLogColored) {
         LOG.EnableColoredOutput();
         LOGERR.EnableColoredOutput();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::ReadSettingsPreCreation() {
      QSettings cSettings;
      cSettings.beginGroup("MainWindow");
      resize(cSettings.value("size", QSize(640,480)).toSize());
      move(cSettings.value("position", QPoint(0,0)).toPoint());
      if(cSettings.contains("icon_dir")) {
         m_strIconDir = cSettings.value("icon_dir").toString();
         if(m_strIconDir.at(m_strIconDir.length()-1) != '/') {
            m_strIconDir.append("/");
         }
      }
      else {
         m_strIconDir = QString::fromStdString(CSimulator::GetInstance().GetInstallationDirectory());
         m_strIconDir += "/include/argos3/plugins/simulator/visualizations/qt-opengl/icons/";
      }
      if(cSettings.contains("texture_dir")) {
         m_strTextureDir = cSettings.value("texture_dir").toString();
         if(m_strTextureDir.at(m_strTextureDir.length()-1) != '/') {
            m_strTextureDir.append("/");
         }
      }
      else {
         m_strTextureDir = QString::fromStdString(CSimulator::GetInstance().GetInstallationDirectory());
         m_strTextureDir += "/include/argos3/plugins/simulator/visualizations/qt-opengl/textures/";
      }
      cSettings.endGroup();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::ReadSettingsPostCreation() {
      QSettings cSettings;
      cSettings.beginGroup("MainWindow");
      restoreState(cSettings.value("docks").toByteArray());
      cSettings.endGroup();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::WriteSettings() {
      QSettings cSettings;
      cSettings.beginGroup("MainWindow");
      cSettings.setValue("docks", saveState());
      cSettings.setValue("size", size());
      cSettings.setValue("position", pos());
      cSettings.setValue("anti-aliasing", m_pcToggleAntiAliasingAction->isChecked());
      cSettings.setValue("icon_dir", m_strIconDir);
      cSettings.setValue("texture_dir", m_strTextureDir);
      cSettings.endGroup();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateExperimentActions() {
      /* Add the play action */
      QIcon cPlayIcon;
      cPlayIcon. addPixmap(QPixmap(m_strIconDir + "/play.png"));
      m_pcPlayAction = new QAction(cPlayIcon, tr("&Play"), this);
      m_pcPlayAction->setShortcut(Qt::Key_P);
      m_pcPlayAction->setToolTip(tr("Play experiment"));
      m_pcPlayAction->setStatusTip(tr("Play experiment"));
      if(! CSimulator::GetInstance().IsRealTimeClock()) {
         /* Add the step action */
         QIcon cStepIcon;
         cStepIcon.addPixmap(QPixmap(m_strIconDir + "/step.png"));
         m_pcStepAction = new QAction(cStepIcon, tr("&Step"), this);
         m_pcStepAction->setToolTip(tr("Step experiment"));
         m_pcStepAction->setStatusTip(tr("Step experiment"));
         m_pcStepAction->setShortcut(Qt::Key_X);
         /* Add the fast forward action */
         QIcon cFastForwardIcon;
         cFastForwardIcon.addPixmap(QPixmap(m_strIconDir + "/fast_forward.png"));
         m_pcFastForwardAction = new QAction(cFastForwardIcon, tr("&Fast Forward"), this);
         m_pcFastForwardAction->setToolTip(tr("Fast forward experiment"));
         m_pcFastForwardAction->setStatusTip(tr("Fast forward experiment"));
         m_pcFastForwardAction->setShortcut(Qt::Key_F);
         /* Add the pause action */
         QIcon cPauseIcon;
         cPauseIcon. addPixmap(QPixmap(m_strIconDir + "/pause.png"));
         m_pcPauseAction = new QAction(cPauseIcon, tr("&Pause"), this);
         m_pcPauseAction->setShortcut(Qt::Key_O);
         m_pcPauseAction->setToolTip(tr("Pause experiment"));
         m_pcPauseAction->setStatusTip(tr("Pause experiment"));
         m_pcPauseAction->setEnabled(false);
      }
      /* Add the terminate action */
      QIcon cTerminateIcon;
      cTerminateIcon.addPixmap(QPixmap(m_strIconDir + "/stop.png"));
      m_pcTerminateAction = new QAction(cTerminateIcon, tr("&Terminate"), this);
      m_pcTerminateAction->setShortcut(Qt::Key_T);
      m_pcTerminateAction->setToolTip(tr("Terminate experiment"));
      m_pcTerminateAction->setStatusTip(tr("Terminate experiment"));
      m_pcTerminateAction->setEnabled(false);
      /* Add the reset action */
      QIcon cResetIcon;
      cResetIcon.addPixmap(QPixmap(m_strIconDir + "/reset.png"));
      m_pcResetAction = new QAction(cResetIcon, tr("&Reset"), this);
      m_pcResetAction->setToolTip(tr("Reset experiment"));
      m_pcResetAction->setStatusTip(tr("Reset experiment"));
      m_pcResetAction->setShortcut(Qt::Key_R);
      m_pcResetAction->setEnabled(false);
      /* Add the capture action */
      QIcon cCaptureIcon;
      cCaptureIcon.addPixmap(QPixmap(m_strIconDir + "/record.png"));
      m_pcCaptureAction = new QAction(cCaptureIcon, tr("&Capture"), this);
      m_pcCaptureAction->setToolTip(tr("Capture frames"));
      m_pcCaptureAction->setStatusTip(tr("Capture frames"));
      m_pcCaptureAction->setCheckable(true);
      m_pcCaptureAction->setShortcut(Qt::Key_C);
      /* Add the quit action */
      m_pcQuitAction = new QAction(tr("&Quit"), this);
      m_pcQuitAction->setStatusTip(tr("Quit the simulator"));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateCameraActions() {
      /* Add the switch camera buttons */
      m_pcSwitchCameraActionGroup = new QActionGroup(this);
      QIcon cCameraIcon;
      cCameraIcon.addPixmap(QPixmap(m_strIconDir + "/camera.png"));
      for(UInt32 i = 0; i < 12; ++i) {
         QAction* pcAction = new QAction(cCameraIcon, tr(QString("Camera %1").arg(i+1).toAscii().data()), m_pcSwitchCameraActionGroup);
         pcAction->setToolTip(tr(QString("Switch to camera %1").arg(i+1).toAscii().data()));
         pcAction->setStatusTip(tr(QString("Switch to camera %1").arg(i+1).toAscii().data()));
         pcAction->setCheckable(true);
         pcAction->setShortcut(Qt::Key_F1 + i);
         pcAction->setData(i);
         m_pcSwitchCameraActions.push_back(pcAction);
      }
      m_pcSwitchCameraActions.first()->setChecked(true);
      /* Add the toogle anti-aliasing button */
      m_pcToggleAntiAliasingAction = new QAction(tr("&Anti-alias"), this);
      m_pcToggleAntiAliasingAction->setStatusTip(tr("Toogle anti-aliasing in OpenGL rendering"));
      m_pcToggleAntiAliasingAction->setCheckable(true);
      /* Add the show camera XML button */
      m_pcShowCameraXMLAction = new QAction(tr("&Show XML..."), this);
      m_pcShowCameraXMLAction->setStatusTip(tr("Show XML configuration for all cameras"));
   }

   /****************************************/
   /****************************************/

   // void CQTOpenGLMainWindow::CreatePOVRayActions() {
   //    /* Add the POVRay XML button */
   //    QIcon cPOVRayXMLIcon;
   //    m_pcPOVRayXMLAction = new QAction(cPOVRayXMLIcon, tr("&Edit XML"), this);
   //    m_pcPOVRayXMLAction->setToolTip(tr("Edit POV-Ray XML configuration"));
   //    m_pcPOVRayXMLAction->setStatusTip(tr("Edit POV-Ray XML configuration"));
   //    /* Add the POVRay Preview button */
   //    QIcon cPOVRayPreviewIcon;
   //    m_pcPOVRayPreviewAction = new QAction(cPOVRayPreviewIcon, tr("&Preview"), this);
   //    m_pcPOVRayPreviewAction->setToolTip(tr("Preview POV-Ray rendering of this scene"));
   //    m_pcPOVRayPreviewAction->setStatusTip(tr("Preview POV-Ray rendering of this scene"));
   // }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateHelpActions() {
      /* Add the 'about qt' button */
      m_pcAboutQTAction = new QAction(tr("About &Qt"), this);
      m_pcAboutQTAction->setStatusTip(tr("Show the Qt library's About box"));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateExperimentToolBar() {
      m_pcExperimentToolBar = addToolBar(tr("Experiment"));
      m_pcExperimentToolBar->setObjectName("ExperimentToolBar");
      m_pcExperimentToolBar->setIconSize(QSize(32,32));
      m_pcCurrentStepLCD = new QLCDNumber(m_pcExperimentToolBar);
      m_pcCurrentStepLCD->setToolTip(tr("Current step"));
      m_pcCurrentStepLCD->setNumDigits(6);
      m_pcCurrentStepLCD->setSegmentStyle(QLCDNumber::Flat);
      m_pcExperimentToolBar->addWidget(m_pcCurrentStepLCD);
      m_pcExperimentToolBar->addSeparator();
      if(! CSimulator::GetInstance().IsRealTimeClock()) {
         m_pcExperimentToolBar->addAction(m_pcStepAction);
      }
      m_pcExperimentToolBar->addAction(m_pcPlayAction);
      if(! CSimulator::GetInstance().IsRealTimeClock()) {
         m_pcExperimentToolBar->addAction(m_pcPauseAction);
         m_pcExperimentToolBar->addAction(m_pcFastForwardAction);
         m_pcDrawFrameEvery = new QSpinBox(m_pcExperimentToolBar);
         m_pcDrawFrameEvery->setToolTip(tr("Draw frame every X steps when in fast-forward"));
         m_pcDrawFrameEvery->setMinimum(1);
         m_pcDrawFrameEvery->setMaximum(999);
         m_pcDrawFrameEvery->setValue(1);
         m_pcExperimentToolBar->addWidget(m_pcDrawFrameEvery);
      }
      m_pcExperimentToolBar->addSeparator();
      m_pcExperimentToolBar->addAction(m_pcTerminateAction);
      m_pcExperimentToolBar->addAction(m_pcResetAction);
      m_pcExperimentToolBar->addSeparator();
      m_pcExperimentToolBar->addAction(m_pcCaptureAction);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateExperimentMenu() {
      m_pcExperimentMenu = menuBar()->addMenu(tr("&Experiment"));
      m_pcExperimentMenu->addAction(m_pcPlayAction);
      if(! CSimulator::GetInstance().IsRealTimeClock()) {
         m_pcExperimentMenu->addAction(m_pcPauseAction);
         m_pcExperimentMenu->addAction(m_pcFastForwardAction);
         m_pcExperimentMenu->addAction(m_pcStepAction);
      }
      m_pcExperimentMenu->addSeparator();
      m_pcExperimentMenu->addAction(m_pcTerminateAction);
      m_pcExperimentMenu->addAction(m_pcResetAction);
      m_pcExperimentMenu->addSeparator();
      m_pcExperimentMenu->addAction(m_pcCaptureAction);
      m_pcExperimentMenu->addSeparator();
      m_pcExperimentMenu->addAction(m_pcQuitAction);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateCameraToolBar() {
      m_pcCameraToolBar = new QToolBar(tr("Camera"));
      m_pcCameraToolBar->setAllowedAreas(Qt::LeftToolBarArea |
                                         Qt::RightToolBarArea |
                                         Qt::BottomToolBarArea);
      m_pcCameraToolBar->setObjectName("CameraToolBar");
      m_pcCameraToolBar->setIconSize(QSize(32,32));
      m_pcCameraToolBar->addActions(m_pcSwitchCameraActions);
      m_pcCameraToolBar->addSeparator();
      m_pcFocalLength = new QDoubleSpinBox(m_pcCameraToolBar);
      m_pcFocalLength->setToolTip(tr("Set the focal length of the current camera"));
      m_pcFocalLength->setSuffix("mm");
      m_pcFocalLength->setDecimals(1);
      m_pcFocalLength->setSingleStep(1.0f);
      m_pcFocalLength->setRange(1.0f, 999.0f);
      m_pcFocalLength->setValue(m_pcOpenGLWidget->GetCamera().GetSetting(0).LensFocalLength * 1000.0f);
      m_pcCameraToolBar->addWidget(m_pcFocalLength);
      addToolBar(Qt::LeftToolBarArea, m_pcCameraToolBar);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateCameraMenu() {
      m_pcCameraMenu = menuBar()->addMenu(tr("&Camera"));
      m_pcCameraMenu->addActions(m_pcSwitchCameraActions);
      m_pcCameraMenu->addAction(m_pcShowCameraXMLAction);
      m_pcCameraMenu->addSeparator();
      m_pcCameraMenu->addAction(m_pcToggleAntiAliasingAction);
   }

   /****************************************/
   /****************************************/

   // void CQTOpenGLMainWindow::CreatePOVRayMenu() {
   //    m_pcPOVRayMenu = menuBar()->addMenu(tr("&POVRay"));
   //    m_pcPOVRayMenu->addAction(m_pcPOVRayPreviewAction);
   //    m_pcPOVRayMenu->addAction(m_pcPOVRayXMLAction);
   // }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateHelpMenu() {
      m_pcHelpMenu = menuBar()->addMenu(tr("&?"));
      m_pcHelpMenu->addAction(m_pcAboutQTAction);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateOpenGLWidget(TConfigurationNode& t_tree) {
      /* Initialize OpenGL settings */
      QGLFormat cGLFormat;
      cGLFormat.setSampleBuffers(true);
      /* Create the widget */
      QWidget* pcPlaceHolder = new QWidget(this);
      m_pcOpenGLWidget = new CQTOpenGLWidget(cGLFormat, pcPlaceHolder, this, *m_pcUserFunctions);
      m_pcOpenGLWidget->setCursor(QCursor(Qt::OpenHandCursor));
      m_pcOpenGLWidget->GetCamera().Init(t_tree);
      m_pcOpenGLWidget->GetFrameGrabData().Init(t_tree);
      if(cGLFormat.sampleBuffers()) {
         /* Get OpenGL settings */
         QSettings cSettings;
         bool bAntiAliasing;
         cSettings.beginGroup("MainWindow");
         if(cSettings.contains("anti-aliasing")) {
            bAntiAliasing = cSettings.value("anti-aliasing").toBool();
         }
         else {
            bAntiAliasing = true;
         }
         cSettings.endGroup();
         m_pcToggleAntiAliasingAction->setChecked(bAntiAliasing);
         m_pcOpenGLWidget->SetAntiAliasing(bAntiAliasing);
      }
      else {
         m_pcToggleAntiAliasingAction->setChecked(false);
         m_pcToggleAntiAliasingAction->setEnabled(false);
      }
      m_pcOpenGLWidget->initializeGL();
      /* Invert mouse controls? */
      bool bInvertMouse;
      GetNodeAttributeOrDefault(t_tree, "invert_mouse", bInvertMouse, false);
      m_pcOpenGLWidget->SetInvertMouse(bInvertMouse);
      /* Set the window as the central widget */
      CQTOpenGLLayout* pcQTOpenGLLayout = new CQTOpenGLLayout();
      pcQTOpenGLLayout->addWidget(m_pcOpenGLWidget);
      pcPlaceHolder->setLayout(pcQTOpenGLLayout);
      setCentralWidget(pcPlaceHolder);
      /* Initialize the user functions */
      if(NodeExists(t_tree, "user_functions")) {
         /* Use the passed user functions */
         /* Get data from XML */
         TConfigurationNode tNode = GetNode(t_tree, "user_functions");
         m_pcUserFunctions->Init(tNode);
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateLogMessageDock() {
      /* Store the log color flag to be able to restore at exit */
      m_bWasLogColored = LOG.IsColoredOutput();
      /* Create a dockable window */
      m_pcLogDock = new QDockWidget(tr("Log"), this);
      m_pcLogDock->setObjectName("LogDockWindow");
      m_pcLogDock->setFeatures(QDockWidget::DockWidgetMovable |
                               QDockWidget::DockWidgetFloatable);
      m_pcLogDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                                   Qt::RightDockWidgetArea |
                                   Qt::BottomDockWidgetArea);
      /* Create a textual window to be used as a buffer */
      m_pcDockLogBuffer = new QTextEdit();
      m_pcDockLogBuffer->setReadOnly(true);
      LOG.Flush(); /* Write all the pending stuff */
      LOG.DisableColoredOutput(); /* Colors are not necessary */
      m_pcDockLogBuffer->append("<b>[t=0]</b> Log started."); /* Write something in the buffer */
      /* Redirect stdout to the buffer */
      m_pcLogStream = new CQTOpenGLLogStream(LOG.GetStream(), m_pcDockLogBuffer);
      /* Add the dockable window to the main widget */
      m_pcLogDock->setWidget(m_pcDockLogBuffer);
      addDockWidget(Qt::RightDockWidgetArea, m_pcLogDock);
      /* Create a dockable window */
      m_pcLogErrDock = new QDockWidget(tr("LogErr"), this);
      m_pcLogErrDock->setObjectName("LogErrDockWindow");
      m_pcLogErrDock->setFeatures(QDockWidget::DockWidgetMovable |
                                  QDockWidget::DockWidgetFloatable);
      m_pcLogErrDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                                      Qt::RightDockWidgetArea |
                                      Qt::BottomDockWidgetArea);
      /* Create a textual window to be used as a buffer */
      m_pcDockLogErrBuffer = new QTextEdit();
      m_pcDockLogErrBuffer->setReadOnly(true);
      LOGERR.Flush(); /* Write all the pending stuff */
      LOGERR.DisableColoredOutput(); /* Colors are not necessary */
      m_pcDockLogErrBuffer->append("<b>[t=0]</b> LogErr started."); /* Write something in the buffer */
      /* Redirect stderr to the buffer */
      m_pcLogErrStream = new CQTOpenGLLogStream(LOGERR.GetStream(), m_pcDockLogErrBuffer);
      m_pcLogErrDock->setWidget(m_pcDockLogErrBuffer);
      /* Add the dockable window to the main widget */
      addDockWidget(Qt::RightDockWidgetArea, m_pcLogErrDock);

   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateConnections() {
      /* Play button pressed */
      connect(m_pcPlayAction, SIGNAL(triggered()),
              this, SLOT(PlayExperiment()));
      /* Reset button pressed */
      connect(m_pcResetAction, SIGNAL(triggered()),
              this, SLOT(ResetExperiment()));
      /* A experiment step has been completed */
      connect(m_pcOpenGLWidget, SIGNAL(StepDone(int)),
              m_pcCurrentStepLCD, SLOT(display(int)));
      /* The experiment has been completed */
      connect(m_pcOpenGLWidget, SIGNAL(ExperimentDone()),
              this, SLOT(TerminateExperiment()));
      /* The experiment has been completed */
      connect(m_pcTerminateAction, SIGNAL(triggered()),
              this, SLOT(TerminateExperiment()));
      if(! CSimulator::GetInstance().IsRealTimeClock()) {
         /* Pause button pressed */
         connect(m_pcPauseAction, SIGNAL(triggered()),
                 this, SLOT(PauseExperiment()));
         /* Step button pressed */
         connect(m_pcStepAction, SIGNAL(triggered()),
                 this, SLOT(StepExperiment()));
         /* Fast forward button pressed */
         connect(m_pcFastForwardAction, SIGNAL(triggered()),
                 this, SLOT(FastForwardExperiment()));
         /* 'Draw frame every' spin box value changed */
         connect(m_pcDrawFrameEvery, SIGNAL(valueChanged(int)),
                 m_pcOpenGLWidget, SLOT(SetDrawFrameEvery(int)));
      }
      // /* POV-Ray XML button pressed */
      // connect(m_pcPOVRayXMLAction, SIGNAL(triggered()),
      //         this, SLOT(POVRaySceneXMLPopUp()));
      // /* POV-Ray XML button pressed */
      // connect(m_pcPOVRayPreviewAction, SIGNAL(triggered()),
      //         this, SLOT(POVRayScenePreview()));
      /* Capture button toggled */
      connect(m_pcCaptureAction, SIGNAL(triggered(bool)),
              m_pcOpenGLWidget, SLOT(SetGrabFrame(bool)));
      /* Toggle antialiasing */
      connect(m_pcToggleAntiAliasingAction, SIGNAL(triggered(bool)),
              m_pcOpenGLWidget, SLOT(SetAntiAliasing(bool)));
      /* Quit the simulator */
      connect(m_pcQuitAction, SIGNAL(triggered()),
              qApp, SLOT(quit()));
      /* Show the 'About Qt' dialog */
      connect(m_pcAboutQTAction, SIGNAL(triggered()),
              qApp, SLOT(aboutQt()));
      /* Toggle the camera */
      connect(m_pcSwitchCameraActionGroup, SIGNAL(triggered(QAction*)),
              this, SLOT(SwitchCamera(QAction*)));
      connect(this, SIGNAL(CameraSwitched(int)),
              m_pcOpenGLWidget, SLOT(SetCamera(int)));
      /* Camera focal length */
      connect(m_pcFocalLength, SIGNAL(valueChanged(double)),
              m_pcOpenGLWidget, SLOT(SetCameraFocalLength(double)));
      /* POV-Ray XML button pressed */
      connect(m_pcShowCameraXMLAction, SIGNAL(triggered()),
              this, SLOT(CameraXMLPopUp()));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateUserFunctions(TConfigurationNode& t_tree) {
      /* Parse XML for user functions */
      if(NodeExists(t_tree, "user_functions")) {
         /* Use the passed user functions */
         /* Get data from XML */
         TConfigurationNode tNode = GetNode(t_tree, "user_functions");
         std::string strLabel, strLibrary;
         GetNodeAttribute(tNode, "label", strLabel);
         GetNodeAttributeOrDefault(tNode, "library", strLibrary, strLibrary);
         try {
            /* Load the library */
            if(strLibrary != "") {
               CDynamicLoading::LoadLibrary(strLibrary);
            }
            /* Create the user functions */
            m_pcUserFunctions = CFactory<CQTOpenGLUserFunctions>::New(strLabel);
            m_pcUserFunctions->SetMainWindow(*this);
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Failed opening QTOpenGL user function library", ex);
         }
      }
      else {
         /* Use standard (empty) user functions */
         m_pcUserFunctions = new CQTOpenGLUserFunctions;
         m_pcUserFunctions->SetMainWindow(*this);
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::closeEvent(QCloseEvent* pc_event) {
      m_pcUserFunctions->Destroy();
      WriteSettings();
      pc_event->accept();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::PlayExperiment() {
      /* Make sure we are in the right state */
      if(m_eExperimentState != EXPERIMENT_INITIALIZED &&
         m_eExperimentState != EXPERIMENT_PAUSED) {
         LOGERR << "[BUG] CQTOpenGLMainWindow::PlayExperiment() called in wrong state: "
                << m_eExperimentState
                << std::endl;
         LOGERR.Flush();
         return;
      }
      /* Toggle action states */
      m_pcPlayAction->setEnabled(false);
      m_pcResetAction->setEnabled(false);
      m_pcTerminateAction->setEnabled(true);
      if(! CSimulator::GetInstance().IsRealTimeClock()) {
         m_pcPauseAction->setEnabled(true);
         m_pcFastForwardAction->setEnabled(false);
         m_pcStepAction->setEnabled(false);
      }
      /* Call OpenGL widget */
      m_pcOpenGLWidget->PlayExperiment();
      /* Change state and emit signals */
      m_eExperimentState = EXPERIMENT_PLAYING;
      if(m_eExperimentState == EXPERIMENT_INITIALIZED) {
         /* The experiment has just been started */
         emit ExperimentStarted();
      }
      emit ExperimentPlaying();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::FastForwardExperiment() {
      /* Make sure we are in the right state */
      if(m_eExperimentState != EXPERIMENT_INITIALIZED &&
         m_eExperimentState != EXPERIMENT_PAUSED) {
         LOGERR << "[BUG] CQTOpenGLMainWindow::FastForwardExperiment() called in wrong state: "
                << m_eExperimentState
                << std::endl;
         LOGERR.Flush();
         return;
      }
      /* Toggle action states */
      m_pcPlayAction->setEnabled(false);
      m_pcPauseAction->setEnabled(true);
      m_pcResetAction->setEnabled(false);
      m_pcTerminateAction->setEnabled(true);
      m_pcFastForwardAction->setEnabled(false);
      m_pcStepAction->setEnabled(false);
      /* Call OpenGL widget */
      m_pcOpenGLWidget->FastForwardExperiment();
      /* Change state and emit signals */
      m_eExperimentState = EXPERIMENT_FAST_FORWARDING;
      if(m_eExperimentState == EXPERIMENT_INITIALIZED) {
         /* The experiment has just been started */
         emit ExperimentStarted();
      }
      emit ExperimentFastForwarding();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::StepExperiment() {
      /* Make sure we are in the right state */
      if(m_eExperimentState != EXPERIMENT_INITIALIZED &&
         m_eExperimentState != EXPERIMENT_PAUSED) {
         LOGERR << "[BUG] CQTOpenGLMainWindow::StepExperiment() called in wrong state: "
                << m_eExperimentState
                << std::endl;
         LOGERR.Flush();
         return;
      }
      /* Toggle action states */
      m_pcPlayAction->setEnabled(true);
      m_pcResetAction->setEnabled(false);
      m_pcTerminateAction->setEnabled(true);
      m_pcFastForwardAction->setEnabled(true);
      m_pcStepAction->setEnabled(true);
      /* Call OpenGL widget */
      m_pcOpenGLWidget->StepExperiment();
      /* Change state and emit signals */
      m_eExperimentState = EXPERIMENT_PAUSED;
      emit ExperimentPaused();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::PauseExperiment() {
      /* Make sure we are in the right state */
      if(m_eExperimentState != EXPERIMENT_PLAYING &&
         m_eExperimentState != EXPERIMENT_FAST_FORWARDING) {
         LOGERR << "[BUG] CQTOpenGLMainWindow::PauseExperiment() called in wrong state: "
                << m_eExperimentState
                << std::endl;
         LOGERR.Flush();
         return;
      }
      /* Toggle action states */
      m_pcPlayAction->setEnabled(true);
      m_pcResetAction->setEnabled(false);
      m_pcTerminateAction->setEnabled(true);
      if(! CSimulator::GetInstance().IsRealTimeClock()) {
         m_pcPauseAction->setEnabled(false);
         m_pcFastForwardAction->setEnabled(true);
         m_pcStepAction->setEnabled(true);
      }
      /* Call OpenGL widget */
      m_pcOpenGLWidget->PauseExperiment();
      /* Change state and emit signals */
      m_eExperimentState = EXPERIMENT_PAUSED;
      emit ExperimentPaused();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::TerminateExperiment() {
      /* Make sure we are in the right state */
      if(m_eExperimentState != EXPERIMENT_PLAYING &&
         m_eExperimentState != EXPERIMENT_PAUSED &&
         m_eExperimentState != EXPERIMENT_FAST_FORWARDING &&
         m_eExperimentState != EXPERIMENT_SUSPENDED) {
         LOGERR << "[BUG] CQTOpenGLMainWindow::TerminateExperiment() called in wrong state: "
                << m_eExperimentState
                << std::endl;
         LOGERR.Flush();
         return;
      }
      /* Call OpenGL widget */
      m_pcOpenGLWidget->PauseExperiment();
      /* Toggle action states */
      m_pcPlayAction->setEnabled(false);
      m_pcResetAction->setEnabled(true);
      m_pcTerminateAction->setEnabled(false);
      m_pcCaptureAction->setEnabled(false);
      m_pcCaptureAction->setChecked(false);
      if(! CSimulator::GetInstance().IsRealTimeClock()) {
         m_pcPauseAction->setEnabled(false);
         m_pcStepAction->setEnabled(false);
         m_pcFastForwardAction->setEnabled(false);
      }
      /* Call ARGoS to terminate the experiment */
      CSimulator::GetInstance().Terminate();
      CSimulator::GetInstance().GetLoopFunctions().PostExperiment();
      LOG.Flush();
      LOGERR.Flush();
      /* Change state and emit signal */
      m_eExperimentState = EXPERIMENT_DONE;
      emit ExperimentDone();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::ResetExperiment() {
      /* Make sure we are in the right state */
      if(m_eExperimentState != EXPERIMENT_SUSPENDED &&
         m_eExperimentState != EXPERIMENT_DONE) {
         LOGERR << "[BUG] CQTOpenGLMainWindow::ResetExperiment() called in wrong state: "
                << m_eExperimentState
                << std::endl;
         LOGERR.Flush();
         return;
      }
      /* Toggle action states */
      m_pcPlayAction->setEnabled(true);
      m_pcResetAction->setEnabled(false);
      m_pcTerminateAction->setEnabled(false);
      m_pcCaptureAction->setEnabled(true);
      m_pcCaptureAction->setChecked(false);
      if(! CSimulator::GetInstance().IsRealTimeClock()) {
         m_pcPauseAction->setEnabled(false);
         m_pcStepAction->setEnabled(true);
         m_pcFastForwardAction->setEnabled(true);
      }
      /* Reset step counter and log */
      m_pcCurrentStepLCD->display(0);
      m_pcDockLogBuffer->setHtml("<b>[t=0]</b> Log restarted.");
      m_pcDockLogErrBuffer->setHtml("<b>[t=0]</b> LogErr restarted.");
      /* Call OpenGL widget */
      m_pcOpenGLWidget->ResetExperiment();
      m_pcUserFunctions->Reset();
      /* Change state and emit signal */
      m_eExperimentState = EXPERIMENT_INITIALIZED;
      emit ExperimentReset();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::SuspendExperiment() {
      /* Toggle action states */
      m_pcPlayAction->setEnabled(false);
      m_pcResetAction->setEnabled(true);
      m_pcTerminateAction->setEnabled(true);
      m_pcCaptureAction->setEnabled(false);
      m_pcCaptureAction->setChecked(false);
      if(! CSimulator::GetInstance().IsRealTimeClock()) {
         m_pcPauseAction->setEnabled(false);
         m_pcStepAction->setEnabled(false);
         m_pcFastForwardAction->setEnabled(false);
      }
      /* Call OpenGL widget */
      m_pcOpenGLWidget->PauseExperiment();
      /* Change state and emit signal */
      m_eExperimentState = EXPERIMENT_SUSPENDED;
      emit ExperimentSuspended();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::ResumeExperiment() {
      /* Make sure we are in the right state */
      if(m_eExperimentState != EXPERIMENT_SUSPENDED) {
         LOGERR << "[BUG] CQTOpenGLMainWindow::ResumeExperiment() called in wrong state: "
                << m_eExperimentState
                << std::endl;
         LOGERR.Flush();
         return;
      }
      /* Toggle action states */
      m_pcPlayAction->setEnabled(true);
      m_pcResetAction->setEnabled(false);
      m_pcTerminateAction->setEnabled(false);
      m_pcCaptureAction->setEnabled(true);
      if(! CSimulator::GetInstance().IsRealTimeClock()) {
         m_pcStepAction->setEnabled(true);
         m_pcFastForwardAction->setEnabled(true);
      }
      /* Change state and emit signal */
      m_eExperimentState = EXPERIMENT_PAUSED;
      emit ExperimentResumed();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CameraXMLPopUp() {
      /* Set the text window up */
      QTextEdit* pcXMLOutput = new QTextEdit();
      /* Calculate the geometry of the window so that it's 1/4 of the main window
         and placed in the exact center of it */
      QRect cGeom = geometry();
      cGeom.setBottomRight(geometry().center());
      cGeom.moveCenter(geometry().center());
      pcXMLOutput->setGeometry(cGeom);
      /* This window steals all input */
      pcXMLOutput->setWindowModality(Qt::ApplicationModal);
      /* You can't modify its contents (but can copy-paste them) */
      pcXMLOutput->setReadOnly(true);
      /* Set nice document name and window title */
      pcXMLOutput->setDocumentTitle("ARGoS XML camera config");
      pcXMLOutput->setWindowTitle("ARGoS XML camera config");
      /* Set the actual text to visualize */
      pcXMLOutput->setPlainText(GetCameraXMLData());
      /* Finally, show the resulting window */
      pcXMLOutput->show();
   }

   /****************************************/
   /****************************************/

   // void CQTOpenGLMainWindow::POVRaySceneXMLPopUp() {
   //    /* Set the text window up */
   //    QTextEdit* pcPOVRayOutput = new QTextEdit();
   //    /* Calculate the geometry of the window so that it's 1/4 of the main window
   //       and placed in the exact center of it */
   //    QRect cGeom = geometry();
   //    cGeom.setBottomRight(geometry().center());
   //    cGeom.moveCenter(geometry().center());
   //    pcPOVRayOutput->setGeometry(cGeom);
   //    /* This window steals all input */
   //    pcPOVRayOutput->setWindowModality(Qt::ApplicationModal);
   //    /* You can't modify its contents (but can copy-paste them) */
   //    pcPOVRayOutput->setReadOnly(true);
   //    /* Set nice document name and window title */
   //    pcPOVRayOutput->setDocumentTitle("ARGoS-POVRay XML camera config");
   //    pcPOVRayOutput->setWindowTitle("ARGoS-POVRay XML camera config");
   //    /* Set the actual text to visualize */
   //    pcPOVRayOutput->setPlainText(GetPOVRaySceneXMLData());
   //    /* Finally, show the resulting window */
   //    pcPOVRayOutput->show();
   // }

   /****************************************/
   /****************************************/

   QString CQTOpenGLMainWindow::GetCameraXMLData() {
      QString strResult("<camera>\n");
      /* Get a reference to the camera */
      CQTOpenGLCamera& cCamera = m_pcOpenGLWidget->GetCamera();
      for(UInt32 i = 0; i < 12; ++i) {
         /* Get its position and target */
         const CQTOpenGLCamera::SSettings& sSettings = cCamera.GetSetting(i);
         const CVector3& cPos = sSettings.Position;
         const CVector3& cLookAt = sSettings.Target;
         const CVector3& cUp = sSettings.Up;
         strResult.append(
            QString("   <placement idx=\"%1\" position=\"%2,%3,%4\" look_at=\"%5,%6,%7\" up=\"%8,%9,%10\" lens_focal_length=\"%11\" />\n")
            .arg(i)
            .arg(cPos.GetX())
            .arg(cPos.GetY())
            .arg(cPos.GetZ())
            .arg(cLookAt.GetX())
            .arg(cLookAt.GetY())
            .arg(cLookAt.GetZ())
            .arg(cUp.GetX())
            .arg(cUp.GetY())
            .arg(cUp.GetZ())
            .arg(sSettings.LensFocalLength * 1000.0f));
      }
      strResult.append("</camera>\n");
      return strResult;
   }

   /****************************************/
   /****************************************/

   // QString CQTOpenGLMainWindow::GetPOVRaySceneXMLData() {
   //    /* Get the current experiment step */
   //    UInt32 unStep = CSimulator::GetInstance().GetSpace().GetExperimentClock();
   //    /* Get a reference to the camera */
   //    const CQTOpenGLCamera& cCamera = m_pcOpenGLWidget->GetCamera();
   //    /* Get its current position and target */
   //    const CVector3& cPos = cCamera.GetPosition();
   //    const CVector3& cLookAt = cCamera.GetTarget();
   //    /* Get the environment node and its contents from the 'povray_render', if defined */
   //    TConfigurationNode& tExperiment = CSimulator::GetInstance().GetConfigurationRoot();
   //    TConfigurationNode& tVisualization = GetNode(tExperiment, "visualization");
   //    QString strPOVRayEnvironment;
   //    if(NodeExists(tVisualization,"povray_render")) {
   //       TConfigurationNode& tPOVRayVisualization = GetNode(tVisualization, "povray_render");
   //       TConfigurationNode& tPOVRayEnvironment = GetNode(tPOVRayVisualization, "environment");
   //       std::string strPOVRayEnvironmentNodeContent = tPOVRayEnvironment.ToString(tPOVRayEnvironment);
   //       strPOVRayEnvironment = strPOVRayEnvironmentNodeContent.c_str();
   //    }

   //    /* Return the XML portion */
   //    return QString(
   //       "%1\n"
   //       "<scene step=\"%2\">\n"
   //       "   <camera type=\"normal\"\n"
   //       "           position=\"%3,%4,%5\"\n"
   //       "           look_at=\"%6,%7,%8\"\n"
   //       "           focal_length=\"%9\" />\n"
   //       "</scene>\n"
   //       )
   //       .arg(strPOVRayEnvironment)
   //       .arg(unStep)
   //       .arg(cPos.GetX())
   //       .arg(cPos.GetY())
   //       .arg(cPos.GetZ())
   //       .arg(cLookAt.GetX())
   //       .arg(cLookAt.GetY())
   //       .arg(cLookAt.GetZ())
   //       .arg(cCamera.GetLensFocalLength() * 1000.0f);
   // }

   /****************************************/
   /****************************************/

   // void CQTOpenGLMainWindow::POVRayScenePreview() {
   //    try {
   //       /* Initialize the POV-Ray working directory */
   //       QDir cDirectory(QDir::tempPath() + "/argos-povray");
   //       /* Erase it if it exists */
   //       if(cDirectory.exists()) {
   //          if(::system(QString("rm -rf %1").arg(cDirectory.absolutePath()).toAscii().data()) != 0) {
   //             THROW_ARGOSEXCEPTION("Could not remove directory \"" <<
   //                                  cDirectory.absolutePath().toAscii().data() << "\".");
   //          }
   //       }
   //       /* Create the directory */
   //       if(::system(QString("mkdir %1").arg(cDirectory.absolutePath()).toAscii().data()) != 0) {
   //          THROW_ARGOSEXCEPTION("Could not create directory \"" <<
   //                               cDirectory.absolutePath().toAscii().data() << "\".");
   //       }
   //       /* Now create the XML file that will contain the POV-Ray scene configuration */
   //       QFile cPOVRayXMLConf(cDirectory.absolutePath() + "/argos-povray.xml");
   //       cPOVRayXMLConf.open(QFile::WriteOnly | QFile::Truncate);
   //       /* Associate a text stream to perform writing to it */
   //       QTextStream cPOVRayXMLConfStream(&cPOVRayXMLConf);
   //       /* Write the configuration */
   //       cPOVRayXMLConfStream << "<povray_render id=\"pov\" output_folder=\"" << cDirectory.absolutePath() << "\">\n";
   //       cPOVRayXMLConfStream << GetPOVRaySceneXMLData();
   //       cPOVRayXMLConfStream << "</povray_render>\n";
   //       cPOVRayXMLConf.close();
   //       /* Now parse this file as an ARGoS TConfigurationTree */
   //       ticpp::Document tPOVRayXMLConfTree(cPOVRayXMLConf.fileName().toAscii().data());
   //       tPOVRayXMLConfTree.LoadFile();
   //       /* It's time to create the POV-Ray visualization */
   //       CPovrayRender cPOVRayRender;
   //       cPOVRayRender.Init(*tPOVRayXMLConfTree.FirstChildElement());
   //       /* Write the .pov frame file */
   //       cPOVRayRender.WriteOneFrame(cDirectory.absolutePath().append("/pov/frame.pov").toAscii().data());
   //       /* Eventually, call POV-Ray to render the file */
   //       if(::system(QString("cd %1 && ")
   //                   .arg(cDirectory.absolutePath())
   //                   .append("./render_single_frame_on_pc.sh pov/frame.pov")
   //                   .toAscii().data()) !=0) {
   //          THROW_ARGOSEXCEPTION("Could not create POV-Ray preview");
   //       }
   //    }
   //    catch(CARGoSException& ex) {
   //       QString strError = QString("Error creating POV-Ray preview\n%1").arg(QString(ex.what()));
   //       QMessageBox::critical(this,
   //                             tr("ARGoS v2.0"),
   //                             strError,
   //                             QMessageBox::Ok);
   //    }
   // }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::SwitchCamera(QAction* pc_action) {
      emit CameraSwitched(pc_action->data().toInt());
      m_pcFocalLength->setValue(m_pcOpenGLWidget->GetCamera().GetActiveSettings().LensFocalLength * 1000.0f);
   }

   /****************************************/
   /****************************************/

}
