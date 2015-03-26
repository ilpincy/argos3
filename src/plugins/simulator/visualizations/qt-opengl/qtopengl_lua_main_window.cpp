/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_main_window.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "qtopengl_lua_main_window.h"
#include "qtopengl_lua_editor.h"
#include "qtopengl_lua_find_dialog.h"
#include "qtopengl_lua_statetree_model.h"
#include "qtopengl_main_window.h"
#include "qtopengl_widget.h"

#include <argos3/core/config.h>
#include <argos3/core/wrappers/lua/lua_controller.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/wrappers/lua/lua_utility.h>

#include <QApplication>
#include <QDockWidget>
#include <QFileDialog>
#include <QHeaderView>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QStatusBar>
#include <QTemporaryFile>
#include <QTextStream>
#include <QToolBar>
#include <QTableWidget>
#include <QTreeView>

namespace argos {

   /****************************************/
   /****************************************/

   static QString SCRIPT_TEMPLATE =
      "-- Use Shift + Click to select a robot\n"
      "-- When a robot is selected, its variables appear in this editor\n\n"
      "-- Use Ctrl + Click (Cmd + Click on Mac) to move a selected robot to a different location\n\n\n\n"
      "-- Put your global variables here\n\n\n\n"
      "--[[ This function is executed every time you press the 'execute' button ]]\n"
      "function init()\n"
      "   -- put your code here\n"
      "end\n\n\n\n"
      "--[[ This function is executed at each time step\n"
      "     It must contain the logic of your controller ]]\n"
      "function step()\n"
      "   -- put your code here\n"
      "end\n\n\n\n"
      "--[[ This function is executed every time you press the 'reset'\n"
      "     button in the GUI. It is supposed to restore the state\n"
      "     of the controller to whatever it was right after init() was\n"
      "     called. The state of sensors and actuators is reset\n"
      "     automatically by ARGoS. ]]\n"
      "function reset()\n"
      "   -- put your code here\n"
      "end\n\n\n\n"
      "--[[ This function is executed only once, when the robot is removed\n"
      "     from the simulation ]]\n"
      "function destroy()\n"
      "   -- put your code here\n"
      "end\n";

   /****************************************/
   /****************************************/

   CQTOpenGLLuaMainWindow::CQTOpenGLLuaMainWindow(CQTOpenGLMainWindow* pc_parent) :
      QMainWindow(pc_parent),
      m_pcMainWindow(pc_parent),
      m_pcStatusbar(NULL),
      m_pcCodeEditor(NULL),
      m_pcFindDialog(NULL),
      m_pcLuaMessageTable(NULL) {
      /* Add a status bar */
      m_pcStatusbar = new QStatusBar(this);
      setStatusBar(m_pcStatusbar);
      /* Create the Lua message table */
      CreateLuaMessageTable();
      /* Populate list of Lua controllers */
      PopulateLuaControllers();
      /* Create the Lua state docks */
      CreateLuaStateDocks();
      /* Create editor */
      CreateCodeEditor();
      /* Create actions */
      CreateFileActions();
      CreateEditActions();
      CreateCodeActions();
      /* Set empty file */
      SetCurrentFile("");
      /* Read settings */
      ReadSettings();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLLuaMainWindow::~CQTOpenGLLuaMainWindow() {
      WriteSettings();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::New() {
      if(MaybeSave()) {
         m_pcCodeEditor->setPlainText(SCRIPT_TEMPLATE);
         SetCurrentFile("");
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::Open() {
      if(MaybeSave()) {
         QString strNewFileName =
            QFileDialog::getOpenFileName(this,
                                         tr("Open File"),
                                         "",
                                         "Lua Files (*.lua)");
         if (!strNewFileName.isEmpty()) {
            OpenFile(strNewFileName);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::OpenRecentFile() {
      QAction* pcAction = qobject_cast<QAction*>(sender());
      if (pcAction) {
         OpenFile(pcAction->data().toString());
      }
   }

   /****************************************/
   /****************************************/

   bool CQTOpenGLLuaMainWindow::Save() {
      if (m_strFileName.isEmpty()) {
         return SaveAs();
      } else {
         return SaveFile(m_strFileName);
      }
   }

   /****************************************/
   /****************************************/

   bool CQTOpenGLLuaMainWindow::SaveAs() {
      QString strNewFileName =
         QFileDialog::getSaveFileName(this,
                                      tr("Save File"),
                                      "",
                                      "Lua Files (*.lua)");
      if (strNewFileName.isEmpty())
         return false;
      return SaveFile(strNewFileName);
   }

   /****************************************/
   /****************************************/

   static QString DetectLuaC() {
      QProcess cLuaCompiler;
      cLuaCompiler.start("luac", QStringList() << "-v");
      /* First, try to execute luac */
      if(!cLuaCompiler.waitForStarted()) {
         /* luac is not installed */
         return "";
      }
      /* luac is installed, but is it the right version? */
      cLuaCompiler.waitForFinished();
      if(QString(cLuaCompiler.readAllStandardOutput()).mid(4,3) == "5.2") {
         return "luac";
      }
      cLuaCompiler.start("luac5.2", QStringList() << "-v");
      if(!cLuaCompiler.waitForStarted()) {
         /* luac52 is not installed */
         return "";
      }
      else {
         /* luac52 is installed */
         cLuaCompiler.waitForFinished();
         return "luac5.2";
      }
   }

   void CQTOpenGLLuaMainWindow::Execute() {
      /* Save script */
      Save();
      /* Change cursor */
      QApplication::setOverrideCursor(Qt::WaitCursor);
      /* Stop simulation */
      m_pcMainWindow->SuspendExperiment();
      /* Clear the message table */
      m_pcLuaMessageTable->clearContents();
      m_pcLuaMessageTable->setRowCount(1);
      /* Create temporary file to contain the bytecode */
      QTemporaryFile cByteCode;
      if(! cByteCode.open()) {
         SetMessage(0, "ALL", "Can't create bytecode file.");
         QApplication::restoreOverrideCursor();
         return;
      }
      /*
       * Compile script
       */
      /* Check for luac 5.2 */
      QString cLuaC = DetectLuaC();
      if(cLuaC == "") {
         /* luac 5.2 not found, fall back to sending the script directly to robots */
         for(size_t i = 0; i < m_vecControllers.size(); ++i) {
            m_vecControllers[i]->SetLuaScript(m_strFileName.toStdString());
         }
      }
      else {
         QProcess cLuaCompiler;
         cLuaCompiler.start(cLuaC, QStringList() << "-o" << cByteCode.fileName() << m_strFileName);
         if(! cLuaCompiler.waitForFinished()) {
            SetMessage(0, "ALL", QString(cLuaCompiler.readAllStandardError()));
            QApplication::restoreOverrideCursor();
            return;
         }
         if(cLuaCompiler.exitCode() != 0) {
            SetMessage(0, "ALL", QString(cLuaCompiler.readAllStandardError()));
            QApplication::restoreOverrideCursor();
            return;
         }
         SetMessage(0, "ALL", "Compilation successful.");
         /* Set the script for all the robots */
         for(size_t i = 0; i < m_vecControllers.size(); ++i) {
            m_vecControllers[i]->SetLuaScript(cByteCode.fileName().toStdString());
         }
      }
      /* Update Lua state if visible */
      if(m_pcLuaVariableDock->isVisible()) {
         static_cast<CQTOpenGLLuaStateTreeModel*>(m_pcLuaVariableTree->model())->SetLuaState(
            m_vecControllers[m_unSelectedRobot]->GetLuaState());
      }
      if(m_pcLuaFunctionDock->isVisible()) {
         static_cast<CQTOpenGLLuaStateTreeModel*>(m_pcLuaFunctionTree->model())->SetLuaState(
            m_vecControllers[m_unSelectedRobot]->GetLuaState());
      }
      /* Resume simulation */
      m_pcMainWindow->ResumeExperiment();
      QApplication::restoreOverrideCursor();
      statusBar()->showMessage(tr("Execution started"), 2000);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::Find() {
      if(! m_pcFindDialog) {
         m_pcFindDialog = new CQTOpenGLLuaFindDialog(this);
      }
      m_pcFindDialog->show();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::CodeModified() {
      setWindowModified(m_pcCodeEditor->document()->isModified());
   }

   /****************************************/
   /****************************************/

   bool CQTOpenGLLuaMainWindow::MaybeSave() {
      if(m_pcCodeEditor->document()->isModified()) {
         QMessageBox::StandardButton tReply;
         tReply = QMessageBox::warning(this, tr("ARGoS v" ARGOS_VERSION "-" ARGOS_RELEASE " - Lua Editor"),
                                       tr("The document has been modified.\n"
                                          "Do you want to save your changes?"),
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
         if (tReply == QMessageBox::Save)
            return Save();
         else if (tReply == QMessageBox::Cancel)
            return false;
      }
      return true;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::PopulateLuaControllers() {
      /* Get list of controllable entities */
      CSpace& cSpace = CSimulator::GetInstance().GetSpace();
      CSpace::TMapPerType& tControllables = cSpace.GetEntitiesByType("controller");
      /* Go through them and keep a pointer to each Lua controller */
      for(CSpace::TMapPerType::iterator it = tControllables.begin();
          it != tControllables.end();
          ++it) {
         /* Try to convert the controller into a Lua controller */
         CControllableEntity* pcControllable = any_cast<CControllableEntity*>(it->second);
         CLuaController* pcLuaController = dynamic_cast<CLuaController*>(&(pcControllable->GetController()));
         if(pcLuaController) {
            /* Conversion succeeded, add to indices */
            m_vecControllers.push_back(pcLuaController);
            m_vecRobots.push_back(&pcControllable->GetParent());
         }
         else {
            LOGERR << "[WARNING] Entity \""
                   << pcControllable->GetParent().GetId()
                   << "\" does not have a Lua controller associated"
                   << std::endl;
         }
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::ReadSettings() {
      QSettings cSettings;
      cSettings.beginGroup("LuaEditor");
      resize(cSettings.value("size", QSize(640,480)).toSize());
      move(cSettings.value("position", QPoint(0,0)).toPoint());
      cSettings.endGroup();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::WriteSettings() {
      QSettings cSettings;
      cSettings.beginGroup("LuaEditor");
      cSettings.setValue("size", size());
      cSettings.setValue("position", pos());
      cSettings.endGroup();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::CreateCodeEditor() {
      /* Create code editor */
      m_pcCodeEditor = new CQTOpenGLLuaEditor(this);
      setCentralWidget(m_pcCodeEditor);
      m_pcCodeEditor->setPlainText(SCRIPT_TEMPLATE);
      /* Connect stuff */
      connect(m_pcCodeEditor->document(), SIGNAL(contentsChanged()),
              this, SLOT(CodeModified()));
      connect(&(m_pcMainWindow->GetOpenGLWidget()), SIGNAL(StepDone(int)),
              this, SLOT(CheckLuaStatus(int)));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::CreateLuaMessageTable() {
      m_pcLuaMsgDock = new QDockWidget(tr("Messages"), this);
      m_pcLuaMsgDock->setObjectName("LuaMessageDock");
      m_pcLuaMsgDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
      m_pcLuaMsgDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
      m_pcLuaMessageTable = new QTableWidget();
      m_pcLuaMessageTable->setColumnCount(3);
      QStringList listHeaders;
      listHeaders << tr("Robot")
                  << tr("Line")
                  << tr("Message");
      m_pcLuaMessageTable->setHorizontalHeaderLabels(listHeaders);
      m_pcLuaMessageTable->horizontalHeader()->setStretchLastSection(true);
      m_pcLuaMessageTable->setSelectionBehavior(QAbstractItemView::SelectRows);
      m_pcLuaMessageTable->setSelectionMode(QAbstractItemView::SingleSelection);
      m_pcLuaMsgDock->setWidget(m_pcLuaMessageTable);
      addDockWidget(Qt::BottomDockWidgetArea, m_pcLuaMsgDock);
      connect(m_pcLuaMessageTable, SIGNAL(itemSelectionChanged()),
              this, SLOT(HandleMsgTableSelection()));
      m_pcLuaMsgDock->hide();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::CreateLuaStateDocks() {
      /* Variable tree dock */
      m_pcLuaVariableDock = new QDockWidget(tr("Variables"), this);
      m_pcLuaVariableDock->setObjectName("LuaVariableDock");
      m_pcLuaVariableDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
      m_pcLuaVariableDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
      m_pcLuaVariableTree = new QTreeView();
      m_pcLuaVariableDock->setWidget(m_pcLuaVariableTree);
      addDockWidget(Qt::LeftDockWidgetArea, m_pcLuaVariableDock);
      m_pcLuaVariableDock->hide();
      /* Function tree dock */
      m_pcLuaFunctionDock = new QDockWidget(tr("Functions"), this);
      m_pcLuaFunctionDock->setObjectName("LuaFunctionDock");
      m_pcLuaFunctionDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
      m_pcLuaFunctionDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
      m_pcLuaFunctionTree = new QTreeView();
      m_pcLuaFunctionDock->setWidget(m_pcLuaFunctionTree);
      addDockWidget(Qt::LeftDockWidgetArea, m_pcLuaFunctionDock);
      m_pcLuaFunctionDock->hide();
      /* Connect stuff */
      connect(&(m_pcMainWindow->GetOpenGLWidget()), SIGNAL(EntitySelected(size_t)),
              this, SLOT(HandleEntitySelection(size_t)));
      connect(&(m_pcMainWindow->GetOpenGLWidget()), SIGNAL(EntityDeselected(size_t)),
              this, SLOT(HandleEntityDeselection(size_t)));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::CreateFileActions() {
      QIcon cFileNewIcon;
      cFileNewIcon.addPixmap(QPixmap(m_pcMainWindow->GetIconDir() + "/new.png"));
      m_pcFileNewAction = new QAction(cFileNewIcon, tr("&New"), this);
      m_pcFileNewAction->setToolTip(tr("Create a new file"));
      m_pcFileNewAction->setStatusTip(tr("Create a new file"));
      m_pcFileNewAction->setShortcut(QKeySequence::New);
      connect(m_pcFileNewAction, SIGNAL(triggered()),
              this, SLOT(New()));
      QIcon cFileOpenIcon;
      cFileOpenIcon.addPixmap(QPixmap(m_pcMainWindow->GetIconDir() + "/open.png"));
      m_pcFileOpenAction = new QAction(cFileOpenIcon, tr("&Open..."), this);
      m_pcFileOpenAction->setToolTip(tr("Open a file"));
      m_pcFileOpenAction->setStatusTip(tr("Open a file"));
      m_pcFileOpenAction->setShortcut(QKeySequence::Open);
      connect(m_pcFileOpenAction, SIGNAL(triggered()),
              this, SLOT(Open()));
      for (int i = 0; i < MAX_RECENT_FILES; ++i) {
         m_pcFileOpenRecentAction[i] = new QAction(this);
         m_pcFileOpenRecentAction[i]->setVisible(false);
         connect(m_pcFileOpenRecentAction[i], SIGNAL(triggered()),
                 this, SLOT(OpenRecentFile()));
      }
      QIcon cFileSaveIcon;
      cFileSaveIcon.addPixmap(QPixmap(m_pcMainWindow->GetIconDir() + "/save.png"));
      m_pcFileSaveAction = new QAction(cFileSaveIcon, tr("&Save"), this);
      m_pcFileSaveAction->setToolTip(tr("Save the current file"));
      m_pcFileSaveAction->setStatusTip(tr("Save the current file"));
      m_pcFileSaveAction->setShortcut(QKeySequence::Save);
      connect(m_pcFileSaveAction, SIGNAL(triggered()),
              this, SLOT(Save()));
      QIcon cFileSaveAsIcon;
      cFileSaveAsIcon.addPixmap(QPixmap(m_pcMainWindow->GetIconDir() + "/saveas.png"));
      m_pcFileSaveAsAction = new QAction(cFileSaveAsIcon, tr("S&ave as..."), this);
      m_pcFileSaveAsAction->setToolTip(tr("Save the current file under a new name"));
      m_pcFileSaveAsAction->setStatusTip(tr("Save the current file under a new name"));
      m_pcFileSaveAsAction->setShortcut(QKeySequence::SaveAs);
      connect(m_pcFileSaveAsAction, SIGNAL(triggered()),
              this, SLOT(SaveAs()));
      QMenu* pcMenu = menuBar()->addMenu(tr("&File"));
      pcMenu->addAction(m_pcFileNewAction);
      pcMenu->addSeparator();
      pcMenu->addAction(m_pcFileOpenAction);
      pcMenu->addSeparator();
      pcMenu->addAction(m_pcFileSaveAction);
      pcMenu->addAction(m_pcFileSaveAsAction);
      m_pcFileSeparateRecentAction = pcMenu->addSeparator();
      for (int i = 0; i < MAX_RECENT_FILES; ++i) {
         pcMenu->addAction(m_pcFileOpenRecentAction[i]);
      }
      QToolBar* pcToolBar = addToolBar(tr("File"));
      pcToolBar->setObjectName("FileToolBar");
      pcToolBar->setIconSize(QSize(32,32));
      pcToolBar->addAction(m_pcFileNewAction);
      pcToolBar->addAction(m_pcFileOpenAction);
      pcToolBar->addAction(m_pcFileSaveAction);
      UpdateRecentFiles();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::CreateEditActions() {
      QIcon cEditUndoIcon;
      cEditUndoIcon.addPixmap(QPixmap(m_pcMainWindow->GetIconDir() + "/undo.png"));
      m_pcEditUndoAction = new QAction(cEditUndoIcon, tr("&Undo"), this);
      m_pcEditUndoAction->setToolTip(tr("Undo last operation"));
      m_pcEditUndoAction->setStatusTip(tr("Undo last operation"));
      m_pcEditUndoAction->setShortcut(QKeySequence::Undo);
      connect(m_pcEditUndoAction, SIGNAL(triggered()),
              m_pcCodeEditor, SLOT(undo()));
      QIcon cEditRedoIcon;
      cEditRedoIcon.addPixmap(QPixmap(m_pcMainWindow->GetIconDir() + "/redo.png"));
      m_pcEditRedoAction = new QAction(cEditRedoIcon, tr("&Redo"), this);
      m_pcEditRedoAction->setToolTip(tr("Redo last operation"));
      m_pcEditRedoAction->setStatusTip(tr("Redo last operation"));
      m_pcEditRedoAction->setShortcut(QKeySequence::Redo);
      connect(m_pcEditRedoAction, SIGNAL(triggered()),
              m_pcCodeEditor, SLOT(redo()));
      QIcon cEditCopyIcon;
      cEditCopyIcon.addPixmap(QPixmap(m_pcMainWindow->GetIconDir() + "/copy.png"));
      m_pcEditCopyAction = new QAction(cEditCopyIcon, tr("&Copy"), this);
      m_pcEditCopyAction->setToolTip(tr("Copy selected text into clipboard"));
      m_pcEditCopyAction->setStatusTip(tr("Copy selected text into clipboard"));
      m_pcEditCopyAction->setShortcut(QKeySequence::Copy);
      connect(m_pcEditCopyAction, SIGNAL(triggered()),
              m_pcCodeEditor, SLOT(copy()));
      QIcon cEditCutIcon;
      cEditCutIcon.addPixmap(QPixmap(m_pcMainWindow->GetIconDir() + "/cut.png"));
      m_pcEditCutAction = new QAction(cEditCutIcon, tr("&Cut"), this);
      m_pcEditCutAction->setToolTip(tr("Move selected text into clipboard"));
      m_pcEditCutAction->setStatusTip(tr("Move selected text into clipboard"));
      m_pcEditCutAction->setShortcut(QKeySequence::Cut);
      connect(m_pcEditCutAction, SIGNAL(triggered()),
              m_pcCodeEditor, SLOT(cut()));
      QIcon cEditPasteIcon;
      cEditPasteIcon.addPixmap(QPixmap(m_pcMainWindow->GetIconDir() + "/paste.png"));
      m_pcEditPasteAction = new QAction(cEditPasteIcon, tr("&Paste"), this);
      m_pcEditPasteAction->setToolTip(tr("Paste text from clipboard"));
      m_pcEditPasteAction->setStatusTip(tr("Paste text from clipboard"));
      m_pcEditPasteAction->setShortcut(QKeySequence::Paste);
      connect(m_pcEditPasteAction, SIGNAL(triggered()),
              m_pcCodeEditor, SLOT(paste()));
      // QIcon cEditFindIcon;
      // cEditFindIcon.addPixmap(QPixmap(m_pcMainWindow->GetIconDir() + "/find.png"));
      // m_pcEditFindAction = new QAction(cEditFindIcon, tr("&Find/Replace"), this);
      // m_pcEditFindAction->setToolTip(tr("Find/replace text"));
      // m_pcEditFindAction->setStatusTip(tr("Find/replace text"));
      // m_pcEditFindAction->setShortcut(QKeySequence::Find);
      // connect(m_pcEditFindAction, SIGNAL(triggered()),
      //         this, SLOT(Find()));
      QMenu* pcMenu = menuBar()->addMenu(tr("&Edit"));
      pcMenu->addAction(m_pcEditUndoAction);
      pcMenu->addAction(m_pcEditRedoAction);
      pcMenu->addSeparator();
      pcMenu->addAction(m_pcEditCopyAction);
      pcMenu->addAction(m_pcEditCutAction);
      pcMenu->addAction(m_pcEditPasteAction);
      // pcMenu->addSeparator();
      // pcMenu->addAction(m_pcEditFindAction);
      QToolBar* pcToolBar = addToolBar(tr("Edit"));
      pcToolBar->setObjectName("EditToolBar");
      pcToolBar->setIconSize(QSize(32,32));
      pcToolBar->addAction(m_pcEditUndoAction);
      pcToolBar->addAction(m_pcEditRedoAction);
      pcToolBar->addSeparator();
      pcToolBar->addAction(m_pcEditCopyAction);
      pcToolBar->addAction(m_pcEditCutAction);
      pcToolBar->addAction(m_pcEditPasteAction);
      // pcToolBar->addAction(m_pcEditFindAction);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::CreateCodeActions() {
      QIcon cCodeExecuteIcon;
      cCodeExecuteIcon.addPixmap(QPixmap(m_pcMainWindow->GetIconDir() + "/execute.png"));
      m_pcCodeExecuteAction = new QAction(cCodeExecuteIcon, tr("&Execute"), this);
      m_pcCodeExecuteAction->setToolTip(tr("Execute code"));
      m_pcCodeExecuteAction->setStatusTip(tr("Execute code"));
      m_pcCodeExecuteAction->setShortcut(tr("Ctrl+E"));
      connect(m_pcCodeExecuteAction, SIGNAL(triggered()),
              this, SLOT(Execute()));
      QMenu* pcMenu = menuBar()->addMenu(tr("&Code"));
      pcMenu->addAction(m_pcCodeExecuteAction);
      QToolBar* pcToolBar = addToolBar(tr("Code"));
      pcToolBar->setObjectName("CodeToolBar");
      pcToolBar->setIconSize(QSize(32,32));
      pcToolBar->addAction(m_pcCodeExecuteAction);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::OpenFile(const QString& str_path) {
      QFile cFile(str_path);
      if(! cFile.open(QFile::ReadOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("ARGoS v" ARGOS_VERSION "-" ARGOS_RELEASE " - Lua Editor"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(str_path)
                              .arg(cFile.errorString()));
         return;
      }
      QApplication::setOverrideCursor(Qt::WaitCursor);
      m_pcCodeEditor->setPlainText(cFile.readAll());
      QApplication::restoreOverrideCursor();
      SetCurrentFile(str_path);
      statusBar()->showMessage(tr("File loaded"), 2000);
   }

   /****************************************/
   /****************************************/

   bool CQTOpenGLLuaMainWindow::SaveFile(const QString& str_path) {
      QFile cFile(str_path);
      if(! cFile.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("ARGoS v" ARGOS_VERSION "-" ARGOS_RELEASE " - Lua Editor"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(str_path)
                              .arg(cFile.errorString()));
         return false;
      }
      QTextStream cOut(&cFile);
      QApplication::setOverrideCursor(Qt::WaitCursor);
      cOut << m_pcCodeEditor->toPlainText();
      QApplication::restoreOverrideCursor();
      SetCurrentFile(str_path);
      statusBar()->showMessage(tr("File saved"), 2000);
      return true;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::SetCurrentFile(const QString& str_path) {
      m_strFileName = str_path;
      QString strShownName;
      if(m_strFileName.isEmpty()) {
         strShownName = "untitled";
      }
      else {
         strShownName = StrippedFileName(m_strFileName);
      }
      setWindowTitle(tr("%1[*] - ARGoS v" ARGOS_VERSION "-" ARGOS_RELEASE " - Lua Editor").arg(strShownName));
      if(!m_strFileName.isEmpty()) {
         m_pcCodeEditor->document()->setModified(false);
         setWindowModified(false);
         QSettings cSettings;
         cSettings.beginGroup("LuaEditor");
         QStringList listFiles = cSettings.value("recent_files").toStringList();
         listFiles.removeAll(m_strFileName);
         listFiles.prepend(m_strFileName);
         while(listFiles.size() > MAX_RECENT_FILES) {
            listFiles.removeLast();
         }
         cSettings.setValue("recent_files", listFiles);
         cSettings.endGroup();
         UpdateRecentFiles();
      }
      else {
         m_pcCodeEditor->document()->setModified(true);
         setWindowModified(true);
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::CheckLuaStatus(int n_step) {
      int nRow = 0;
      m_pcLuaMessageTable->clearContents();
      m_pcLuaMessageTable->setRowCount(m_vecControllers.size());
      for(size_t i = 0; i < m_vecControllers.size(); ++i) {
         if(! m_vecControllers[i]->IsOK()) {
            SetMessage(nRow,
                       QString::fromStdString(m_vecControllers[i]->GetId()),
                       QString::fromStdString(m_vecControllers[i]->GetErrorMessage()));
            ++nRow;
         }
      }
      m_pcLuaMessageTable->setRowCount(nRow);
      if(nRow > 0) {
         m_pcMainWindow->SuspendExperiment();
      }
      else {
         m_pcLuaMsgDock->hide();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::HandleMsgTableSelection() {
      QList<QTableWidgetItem*> listSel = m_pcLuaMessageTable->selectedItems();
      if(! listSel.empty()) {
         int nLine = listSel[1]->data(Qt::DisplayRole).toInt();
         QTextCursor cCursor = m_pcCodeEditor->textCursor();
         int nCurLine = cCursor.blockNumber();
         if(nCurLine < nLine) {
            cCursor.movePosition(QTextCursor::NextBlock,
                                 QTextCursor::MoveAnchor,
                                 nLine - nCurLine - 1);
         }
         else if(nCurLine > nLine) {
            cCursor.movePosition(QTextCursor::PreviousBlock,
                                 QTextCursor::MoveAnchor,
                                 nCurLine - nLine + 1);
         }
         cCursor.movePosition(QTextCursor::StartOfBlock);
         m_pcCodeEditor->setTextCursor(cCursor);
         m_pcCodeEditor->setFocus();
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::HandleEntitySelection(size_t un_index) {
      CComposableEntity* pcSelectedEntity = dynamic_cast<CComposableEntity*>(CSimulator::GetInstance().GetSpace().GetRootEntityVector()[un_index]);
      if(pcSelectedEntity != NULL) {
         bool bFound = false;
         m_unSelectedRobot = 0;
         while(!bFound && m_unSelectedRobot < m_vecRobots.size()) {
            if(m_vecRobots[m_unSelectedRobot] == pcSelectedEntity) {
               bFound = true;
            }
            else {
               ++m_unSelectedRobot;
            }
         }
         if(bFound &&
            m_vecControllers[m_unSelectedRobot]->GetLuaState() != NULL) {
            CQTOpenGLLuaStateTreeVariableModel* pcVarModel =
               new CQTOpenGLLuaStateTreeVariableModel(m_vecControllers[m_unSelectedRobot]->GetLuaState(),
                                                      false,
                                                      m_pcLuaVariableTree);
            pcVarModel->Refresh();
            connect(&(m_pcMainWindow->GetOpenGLWidget()), SIGNAL(StepDone(int)),
                    pcVarModel, SLOT(Refresh(int)));
            connect(m_pcMainWindow, SIGNAL(ExperimentReset()),
                    pcVarModel, SLOT(Refresh()));
            connect(pcVarModel, SIGNAL(modelReset()),
                    this, SLOT(VariableTreeChanged()),
                    Qt::QueuedConnection);
            m_pcLuaVariableTree->setModel(pcVarModel);
            m_pcLuaVariableTree->setRootIndex(pcVarModel->index(0, 0));
            m_pcLuaVariableTree->expandAll();
            m_pcLuaVariableDock->show();
            CQTOpenGLLuaStateTreeFunctionModel* pcFunModel =
               new CQTOpenGLLuaStateTreeFunctionModel(m_vecControllers[m_unSelectedRobot]->GetLuaState(),
                                                      true,
                                                      m_pcLuaFunctionTree);
            pcFunModel->Refresh();
            connect(&(m_pcMainWindow->GetOpenGLWidget()), SIGNAL(StepDone(int)),
                    pcFunModel, SLOT(Refresh(int)));
            connect(m_pcMainWindow, SIGNAL(ExperimentReset()),
                    pcFunModel, SLOT(Refresh()));
            connect(pcFunModel, SIGNAL(modelReset()),
                    this, SLOT(FunctionTreeChanged()),
                    Qt::QueuedConnection);
            m_pcLuaFunctionTree->setModel(pcFunModel);
            m_pcLuaFunctionTree->setRootIndex(pcFunModel->index(0, 0));
            m_pcLuaFunctionTree->expandAll();
            m_pcLuaFunctionDock->show();
         }
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::HandleEntityDeselection(size_t) {
      disconnect(&(m_pcMainWindow->GetOpenGLWidget()), SIGNAL(StepDone(int)),
                 m_pcLuaVariableTree->model(), SLOT(Refresh(int)));
      disconnect(m_pcMainWindow, SIGNAL(ExperimentReset()),
                 m_pcLuaVariableTree->model(), SLOT(Refresh()));
      disconnect(m_pcLuaVariableTree->model(), SIGNAL(modelReset()),
                 this, SLOT(VariableTreeChanged()));
      m_pcLuaVariableDock->hide();
      delete m_pcLuaVariableTree->model();
      m_pcLuaVariableTree->setModel(NULL);
      disconnect(&(m_pcMainWindow->GetOpenGLWidget()), SIGNAL(StepDone(int)),
                 m_pcLuaFunctionTree->model(), SLOT(Refresh(int)));
      disconnect(m_pcMainWindow, SIGNAL(ExperimentReset()),
                 m_pcLuaFunctionTree->model(), SLOT(Refresh()));
      disconnect(m_pcLuaFunctionTree->model(), SIGNAL(modelReset()),
                 this, SLOT(FunctionTreeChanged()));
      m_pcLuaFunctionDock->hide();
      delete m_pcLuaFunctionTree->model();
      m_pcLuaFunctionTree->setModel(NULL);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::VariableTreeChanged() {
      m_pcLuaVariableTree->setRootIndex(m_pcLuaVariableTree->model()->index(0, 0));
      m_pcLuaVariableTree->expandAll();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::FunctionTreeChanged() {
      m_pcLuaFunctionTree->setRootIndex(m_pcLuaFunctionTree->model()->index(0, 0));
      m_pcLuaFunctionTree->expandAll();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::UpdateRecentFiles() {
      QSettings cSettings;
      cSettings.beginGroup("LuaEditor");
      QStringList listFiles = cSettings.value("recent_files").toStringList();
      int nRecentFiles = qMin(listFiles.size(), (int)MAX_RECENT_FILES);
      for(int i = 0; i < nRecentFiles; ++i) {
         m_pcFileOpenRecentAction[i]->setText(tr("&%1 %2").arg(i+1).arg(StrippedFileName(listFiles[i])));
         m_pcFileOpenRecentAction[i]->setData(listFiles[i]);
         m_pcFileOpenRecentAction[i]->setVisible(true);
      }
      for(int i = nRecentFiles; i < MAX_RECENT_FILES; ++i) {
         m_pcFileOpenRecentAction[i]->setVisible(false);
      }
      m_pcFileSeparateRecentAction->setVisible(nRecentFiles > 0);
      cSettings.endGroup();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::SetMessage(int n_row,
                                           const QString& str_robot_id,
                                           const QString& str_message) {
      QStringList listFields = str_message.split(":",
                                                 QString::KeepEmptyParts,
                                                 Qt::CaseInsensitive);
      m_pcLuaMessageTable->setItem(
         n_row, 0,
         new QTableWidgetItem(str_robot_id));
      if(listFields.size() == 3) {
         m_pcLuaMessageTable->setItem(
            n_row, 1,
            new QTableWidgetItem(listFields[1]));
         m_pcLuaMessageTable->setItem(
            n_row, 2,
            new QTableWidgetItem(listFields[2]));
      }
      else if(listFields.size() == 4) {
         m_pcLuaMessageTable->setItem(
            n_row, 1,
            new QTableWidgetItem(listFields[2]));
         m_pcLuaMessageTable->setItem(
            n_row, 2,
            new QTableWidgetItem(listFields[3]));
      }
      else {
         m_pcLuaMessageTable->setItem(
            n_row, 2,
            new QTableWidgetItem(str_message));
      }
      m_pcLuaMsgDock->show();
   }

   /****************************************/
   /****************************************/

   QString CQTOpenGLLuaMainWindow::StrippedFileName(const QString& str_path) {
      return QFileInfo(str_path).fileName();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::closeEvent(QCloseEvent* pc_event) {
      pc_event->ignore();
   }

   /****************************************/
   /****************************************/

}
