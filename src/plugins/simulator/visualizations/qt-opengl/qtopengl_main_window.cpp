/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_main_window.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_widget.h"
#include "qtopengl_log_stream.h"
#include "qtopengl_user_functions.h"
#include "qtopengl_main_window.h"

#include <argos3/core/utility/plugins/dynamic_loading.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/simulator.h>

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
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QToolBar>
#include <QLayout>
#include <QMenuBar>

#include <cstdio>

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
      GetNodeAttributeOrDefault<std::string>(t_tree, "title", strTitle, "ARGoS v2.0");
      setWindowTitle(tr(strTitle.c_str()));
      /** Installation directory */
      std::string strIconDir = CSimulator::GetInstance().GetInstallationDirectory();
      strIconDir += "/simulator/visualizations/qt-opengl/icons/";
      m_strIconDir = strIconDir.c_str();
      /* Add a status bar */
      m_pcStatusbar = new QStatusBar(this);
      setStatusBar(m_pcStatusbar);
      /* Create actions */
      CreateSimulationActions();
      CreateCameraActions();
      // CreatePOVRayActions();
      CreateHelpActions();
      /* Restore settings, if any */
      ReadSettingsPreCreation();
      /* Create the central widget */
      CreateOpenGLWidget(t_tree);
      /* Create menus */
      CreateSimulationMenu();
      CreateCameraMenu();
      // CreatePOVRayMenu();
      CreateHelpMenu();
      /* Create toolbars */
      CreateSimulationToolBar();
      CreateCameraToolBar();
      /* Create the message dock window */
      CreateLogMessageDock();
      /* Restore settings, if any */
      ReadSettingsPostCreation();
      /* Creates the signal/slot connections */
      CreateConnections();
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
      m_pcToggleAntiAliasingAction->setChecked(cSettings.value("anti-aliasing").toBool());
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
      cSettings.endGroup();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateSimulationActions() {
      /* Add the play button */
      std::string strSTDBaseDirectory(CSimulator::GetInstance().GetInstallationDirectory());
      strSTDBaseDirectory += "/simulator/visualizations/qt-opengl/icons/";
      QString strBaseDirectory(strSTDBaseDirectory.c_str());
      QIcon cPlayIcon;
      cPlayIcon.addPixmap(QPixmap(strBaseDirectory + "play.png"));
      m_pcPlayAction = new QAction(cPlayIcon, tr("&Play"), this);
      m_pcPlayAction->setToolTip(tr("Play/pause simulation"));
      m_pcPlayAction->setStatusTip(tr("Play/pause simulation"));
      m_pcPlayAction->setCheckable(true);
      m_pcPlayAction->setShortcut(Qt::Key_P);
      /* Add the pause/step button */
      QIcon cStepIcon;
      cStepIcon.addPixmap(QPixmap(strBaseDirectory + "step.png"));
      m_pcStepAction = new QAction(cStepIcon, tr("&Step"), this);
      m_pcStepAction->setToolTip(tr("Step simulation"));
      m_pcStepAction->setStatusTip(tr("Step simulation"));
      m_pcStepAction->setShortcut(Qt::Key_S);
      /* Add the fast forward button */
      QIcon cFastForwardIcon;
      cFastForwardIcon.addPixmap(QPixmap(strBaseDirectory + "fast_forward.png"));
      m_pcFastForwardAction = new QAction(cFastForwardIcon, tr("&Fast Forward"), this);
      m_pcFastForwardAction->setToolTip(tr("Fast forward simulation"));
      m_pcFastForwardAction->setStatusTip(tr("Fast forward simulation"));
      m_pcFastForwardAction->setCheckable(true);
      m_pcFastForwardAction->setShortcut(Qt::Key_F);
      /* Add the reset button */
      QIcon cResetIcon;
      cResetIcon.addPixmap(QPixmap(strBaseDirectory + "reset.png"));
      m_pcResetAction = new QAction(cResetIcon, tr("&Reset"), this);
      m_pcResetAction->setToolTip(tr("Reset simulation"));
      m_pcResetAction->setStatusTip(tr("Reset simulation"));
      m_pcResetAction->setShortcut(Qt::Key_R);
      /* Add the capture button */
      QIcon cCaptureIcon;
      cCaptureIcon.addPixmap(QPixmap(strBaseDirectory + "record.png"));
      m_pcCaptureAction = new QAction(cCaptureIcon, tr("&Capture"), this);
      m_pcCaptureAction->setToolTip(tr("Capture frames"));
      m_pcCaptureAction->setStatusTip(tr("Capture frames"));
      m_pcCaptureAction->setCheckable(true);
      m_pcCaptureAction->setShortcut(Qt::Key_C);
      /* Add the quit button */
      m_pcQuitAction = new QAction(tr("&Quit"), this);
      m_pcQuitAction->setStatusTip(tr("Quit the simulator"));
      m_pcQuitAction->setShortcut(Qt::Key_Q);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateCameraActions() {
      /* Add the switch camera buttons */
      m_pcSwitchCameraActionGroup = new QActionGroup(this);
      QIcon cCameraIcon;
      cCameraIcon.addPixmap(QPixmap(m_strIconDir + "camera.png"));
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

   void CQTOpenGLMainWindow::CreateSimulationToolBar() {
      m_pcSimulationToolBar = addToolBar(tr("Simulation"));
      m_pcSimulationToolBar->setObjectName("SimulationToolBar");
      m_pcCurrentStepLCD = new QLCDNumber(m_pcSimulationToolBar);
      m_pcCurrentStepLCD->setToolTip(tr("Current step"));
      m_pcCurrentStepLCD->setNumDigits(6);
      m_pcCurrentStepLCD->setSegmentStyle(QLCDNumber::Flat);
      m_pcSimulationToolBar->addWidget(m_pcCurrentStepLCD);
      m_pcSimulationToolBar->addSeparator();
      m_pcSimulationToolBar->addAction(m_pcPlayAction);
      m_pcSimulationToolBar->addAction(m_pcStepAction);
      m_pcSimulationToolBar->addAction(m_pcFastForwardAction);
      m_pcDrawFrameEvery = new QSpinBox(m_pcSimulationToolBar);
      m_pcDrawFrameEvery->setToolTip(tr("Draw frame every X steps when in fast-forward"));
      m_pcDrawFrameEvery->setMinimum(1);
      m_pcDrawFrameEvery->setMaximum(999);
      m_pcDrawFrameEvery->setValue(1);
      m_pcSimulationToolBar->addWidget(m_pcDrawFrameEvery);
      m_pcSimulationToolBar->addSeparator();
      m_pcSimulationToolBar->addAction(m_pcResetAction);
      m_pcSimulationToolBar->addAction(m_pcCaptureAction);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateSimulationMenu() {
      m_pcSimulationMenu = menuBar()->addMenu(tr("&Simulation"));
      m_pcSimulationMenu->addAction(m_pcPlayAction);
      m_pcSimulationMenu->addAction(m_pcStepAction);
      m_pcSimulationMenu->addAction(m_pcFastForwardAction);
      m_pcSimulationMenu->addSeparator();
      m_pcSimulationMenu->addAction(m_pcResetAction);
      m_pcSimulationMenu->addAction(m_pcCaptureAction);
      m_pcSimulationMenu->addSeparator();
      m_pcSimulationMenu->addAction(m_pcQuitAction);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateCameraToolBar() {
      m_pcCameraToolBar = addToolBar(tr("Camera"));
      m_pcCameraToolBar->setObjectName("CameraToolBar");
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
      /* Create user functions */
      m_pcUserFunctions = CreateUserFunctions(t_tree);
      /* Set up OpenGL features */
      QGLFormat cGLFormat = QGLFormat::defaultFormat();
      cGLFormat.setSampleBuffers(m_pcToggleAntiAliasingAction->isChecked());
      cGLFormat.setStencil(false);
      QGLFormat::setDefaultFormat(cGLFormat);
      /* Create the widget */
      QWidget* pcPlaceHolder = new QWidget(this);
      m_pcOpenGLWidget = new CQTOpenGLWidget(pcPlaceHolder, *m_pcUserFunctions);
      m_pcOpenGLWidget->setCursor(QCursor(Qt::OpenHandCursor));
      m_pcOpenGLWidget->GetCamera().Init(t_tree);
      m_pcOpenGLWidget->GetFrameGrabData().Init(t_tree);
      m_pcToggleAntiAliasingAction->setChecked(cGLFormat.sampleBuffers());
      /* Invert mouse controls? */
      bool bInvertMouse;
      GetNodeAttributeOrDefault(t_tree, "invert_mouse", bInvertMouse, false);
      m_pcOpenGLWidget->SetInvertMouse(bInvertMouse);
      /* Set the window as the central widget */
      CQTOpenGLLayout* pcQTOpenGLLayout = new CQTOpenGLLayout();
      pcQTOpenGLLayout->addWidget(m_pcOpenGLWidget);
      pcPlaceHolder->setLayout(pcQTOpenGLLayout);
      setCentralWidget(pcPlaceHolder);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::CreateLogMessageDock() {
      /* Store the log color flag to be able to restore at exit */
      m_bWasLogColored = LOG.IsColoredOutput();
      /* Create a dockable window */
      m_pcLogDock = new QDockWidget(tr("Log"), this);
      m_pcLogDock->setObjectName("LogDockWindow");
      m_pcLogDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
      m_pcLogDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
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
      m_pcLogErrDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
      m_pcLogErrDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
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
      /* Play/pause button pressed */
      connect(m_pcPlayAction, SIGNAL(triggered(bool)),
              m_pcOpenGLWidget, SLOT(PlayPauseSimulation(bool)));
      /* Step button pressed */
      connect(m_pcStepAction, SIGNAL(triggered()),
              m_pcOpenGLWidget, SLOT(StepSimulation()));
      /* Fast forward button pressed */
      connect(m_pcFastForwardAction, SIGNAL(triggered(bool)),
              m_pcOpenGLWidget, SLOT(FastForwardPauseSimulation(bool)));
      /* Reset button pressed */
      connect(m_pcResetAction, SIGNAL(triggered()),
              m_pcOpenGLWidget, SLOT(ResetSimulation()));
      connect(m_pcResetAction, SIGNAL(triggered()),
              this, SLOT(SimulationReset()));
      /* Capture button toggled */
      connect(m_pcCaptureAction, SIGNAL(triggered(bool)),
              m_pcOpenGLWidget, SLOT(SetGrabFrame(bool)));
      // /* POV-Ray XML button pressed */
      // connect(m_pcPOVRayXMLAction, SIGNAL(triggered()),
      //         this, SLOT(POVRaySceneXMLPopUp()));
      // /* POV-Ray XML button pressed */
      // connect(m_pcPOVRayPreviewAction, SIGNAL(triggered()),
      //         this, SLOT(POVRayScenePreview()));
      /* 'Draw frame every' spin box value changed */
      connect(m_pcDrawFrameEvery, SIGNAL(valueChanged(int)),
              m_pcOpenGLWidget, SLOT(SetDrawFrameEvery(int)));
      /* A simulation step has been completed */
      connect(m_pcOpenGLWidget, SIGNAL(StepDone(int)),
              m_pcCurrentStepLCD, SLOT(display(int)));
      /* The simulation has been completed */
      connect(m_pcOpenGLWidget, SIGNAL(SimulationDone()),
              this, SLOT(SimulationDone()));
      /* Toggle antialiasing */
      connect(m_pcToggleAntiAliasingAction, SIGNAL(triggered(bool)),
              m_pcOpenGLWidget, SLOT(SetAntiAliasing(bool)));
      /* Quit the simulator */
      connect(m_pcQuitAction, SIGNAL(triggered()),
              qApp, SLOT(quit()));
      /* Quit the simulator */
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

   CQTOpenGLUserFunctions* CQTOpenGLMainWindow::CreateUserFunctions(TConfigurationNode& t_tree) {
      /* Parse XML for user functions */
      if(NodeExists(t_tree, "user_functions")) {
         /* Use the passed user functions */
         /* Get data from XML */
         TConfigurationNode tNode = GetNode(t_tree, "user_functions");
         std::string strLabel, strLibrary;
         GetNodeAttribute(tNode, "label", strLabel);
         GetNodeAttribute(tNode, "library", strLibrary);
         try {
            /* Load the library taking care of the $ARGOSINSTALLDIR variable */
            CDynamicLoading::LoadLibrary(strLibrary);
            /* Create the user functions */
            return CFactory<CQTOpenGLUserFunctions>::New(strLabel);
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Failed opening QTOpenGL user function library", ex);
         }
      }
      else {
         /* Use standard (empty) user functions */
         return new CQTOpenGLUserFunctions;
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::closeEvent(QCloseEvent* pc_event) {
      WriteSettings();
      pc_event->accept();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::SimulationDone() {
      m_pcPlayAction->setChecked(false);
      m_pcPlayAction->setEnabled(false);
      m_pcStepAction->setEnabled(false);
      m_pcFastForwardAction->setChecked(false);
      m_pcFastForwardAction->setEnabled(false);
      m_pcCaptureAction->setEnabled(false);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLMainWindow::SimulationReset() {
      m_pcPlayAction->setChecked(false);
      m_pcPlayAction->setEnabled(true);
      m_pcStepAction->setEnabled(true);
      m_pcFastForwardAction->setChecked(false);
      m_pcFastForwardAction->setEnabled(true);
      m_pcCaptureAction->setEnabled(true);
      m_pcCurrentStepLCD->display(0);
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
         strResult.append(
            QString("   <placement idx=\"%1\" position=\"%2,%3,%4\" look_at=\"%5,%6,%7\" lens_focal_length=\"%8\" />\n")
            .arg(i)
            .arg(cPos.GetX())
            .arg(cPos.GetY())
            .arg(cPos.GetZ())
            .arg(cLookAt.GetX())
            .arg(cLookAt.GetY())
            .arg(cLookAt.GetZ())
            .arg(sSettings.LensFocalLength * 1000.0f));
      }
      strResult.append("</camera>\n");
      return strResult;
   }

   /****************************************/
   /****************************************/

   // QString CQTOpenGLMainWindow::GetPOVRaySceneXMLData() {
   //    /* Get the current simulation step */
   //    UInt32 unStep = CSimulator::GetInstance().GetSpace().GetSimulationClock();
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
