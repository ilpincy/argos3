#include "qtopengl_lua_main_window.h"
#include "qtopengl_lua_editor.h"
#include "qtopengl_lua_syntax_highlighter.h"
#include "qtopengl_main_window.h"
#include "qtopengl_widget.h"

#include <argos3/core/wrappers/lua/lua_controller.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>

#include <QApplication>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLLuaMainWindow::CQTOpenGLLuaMainWindow(CQTOpenGLMainWindow* pc_parent) :
      QMainWindow(pc_parent),
      m_pcMainWindow(pc_parent) {
      /* Add a status bar */
      m_pcStatusbar = new QStatusBar(this);
      setStatusBar(m_pcStatusbar);
      /* Populate list of Lua controllers */
      PopulateLuaControllers();
      /* Create editor */
      CreateCodeEditor();
      /* Create actions */
      CreateFileActions();
      CreateEditActions();
      CreateCodeActions();
      /* Set empty file */
      SetCurrentFile("");
      /* Connect text modification signal to modification slot */
      connect(m_pcCodeEditor->document(), SIGNAL(contentsChanged()),
              this, SLOT(CodeModified()));
      connect(&(m_pcMainWindow->GetOpenGLWidget()), SIGNAL(StepDone(int)),
              this, SLOT(CheckLuaStatus(int)));
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
         m_pcCodeEditor->clear();
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

   void CQTOpenGLLuaMainWindow::Execute() {
      if(MaybeSave()) {
         QApplication::setOverrideCursor(Qt::WaitCursor);
         for(size_t i = 0; i < m_vecControllers.size(); ++i) {
            m_vecControllers[i]->SetLuaScript(m_strFileName.toStdString());
         }
         QApplication::restoreOverrideCursor();
         statusBar()->showMessage(tr("Execution started"), 2000);
      }
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
         tReply = QMessageBox::warning(this, tr("ARGoS v3.0 - Lua Editor"),
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
      CSpace& cSpace = CSimulator::GetInstance().GetSpace();
      CSpace::TMapPerType& tControllables = cSpace.GetEntitiesByType("controller");
      for(CSpace::TMapPerType::iterator it = tControllables.begin();
          it != tControllables.end();
          ++it) {
         CControllableEntity* pcControllable = any_cast<CControllableEntity*>(it->second);
         CLuaController* pcLuaController = dynamic_cast<CLuaController*>(&(pcControllable->GetController()));
         if(pcLuaController) {
            m_vecControllers.push_back(pcLuaController);
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
      QFont cFont;
      cFont.setFamily("Courier");
      cFont.setFixedPitch(true);
      cFont.setPointSize(10);
      m_pcCodeEditor = new CQTOpenGLLuaEditor(this);
      m_pcCodeEditor->setFont(cFont);
      new CQTOpenGLLuaSyntaxHighlighter(m_pcCodeEditor->document());
      setCentralWidget(m_pcCodeEditor);
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
      QToolBar* pcToolBar = addToolBar(tr("File"));
      pcToolBar->addAction(m_pcFileNewAction);
      pcToolBar->addAction(m_pcFileOpenAction);
      pcToolBar->addAction(m_pcFileSaveAction);
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
      QMenu* pcMenu = menuBar()->addMenu(tr("&Edit"));
      pcMenu->addAction(m_pcEditUndoAction);
      pcMenu->addAction(m_pcEditRedoAction);
      pcMenu->addSeparator();
      pcMenu->addAction(m_pcEditCopyAction);
      pcMenu->addAction(m_pcEditCutAction);
      pcMenu->addAction(m_pcEditPasteAction);
      QToolBar* pcToolBar = addToolBar(tr("Edit"));
      pcToolBar->addAction(m_pcEditUndoAction);
      pcToolBar->addAction(m_pcEditRedoAction);
      pcToolBar->addSeparator();
      pcToolBar->addAction(m_pcEditCopyAction);
      pcToolBar->addAction(m_pcEditCutAction);
      pcToolBar->addAction(m_pcEditPasteAction);
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
      pcToolBar->addAction(m_pcCodeExecuteAction);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::OpenFile(const QString& str_path) {
      QFile cFile(str_path);
      if(! cFile.open(QFile::ReadOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("ARGoS v3.0 - Lua Editor"),
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
         QMessageBox::warning(this, tr("ARGoS v3.0 - Lua Editor"),
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
      m_pcCodeEditor->document()->setModified(false);
      setWindowModified(false);
      QString strShownName;
      if (m_strFileName.isEmpty()) {
         strShownName = "untitled";
      }
      else {
         strShownName = QFileInfo(m_strFileName).fileName();
      }
      setWindowTitle(tr("%1[*] - ARGoS v3.0 - Lua Editor").arg(strShownName));
   }
   
   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::CheckLuaStatus(int n_step) {
      for(size_t i = 0; i < m_vecControllers.size(); ++i) {
         if(! m_vecControllers[i]->IsOK()) {
            printf("[t=%d] [%s] %s\n",
                   n_step,
                   m_vecControllers[i]->GetId().c_str(),
                   m_vecControllers[i]->GetErrorMessage().c_str());
         }
      }
   }
   
   /****************************************/
   /****************************************/

}
