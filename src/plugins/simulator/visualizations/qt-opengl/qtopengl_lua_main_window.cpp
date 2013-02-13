#include "qtopengl_lua_main_window.h"
#include "qtopengl_lua_editor.h"
#include "qtopengl_lua_syntax_highlighter.h"

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

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLLuaMainWindow::CQTOpenGLLuaMainWindow(QWidget* pc_parent) :
      QMainWindow(pc_parent) {
      /* Add a status bar */
      m_pcStatusbar = new QStatusBar(this);
      setStatusBar(m_pcStatusbar);
      /* Populate list of Lua controllers */
      PopulateLuaControllers();
      /* Create editor */
      CreateCodeEditor();
      /* Create actions */
      CreateFileActions();
      CreateLuaActions();
      /* Set empty file */
      SetCurrentFile("");
      /* Connect text modification signal to modification slot */
      connect(m_pcCodeEditor->document(), SIGNAL(contentsChanged()),
              this, SLOT(CodeModified()));
      ReadSettings();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLLuaMainWindow::~CQTOpenGLLuaMainWindow() {
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
      QMenu* pcFileMenu = new QMenu(tr("&File"), this);
      menuBar()->addMenu(pcFileMenu);
      pcFileMenu->addAction(tr("&New"), this, SLOT(New()),
                            QKeySequence(tr("Ctrl+N",
                                            "File|New")));
      pcFileMenu->addAction(tr("&Open..."), this, SLOT(Open()),
                            QKeySequence(tr("Ctrl+O",
                                            "File|Open")));
      pcFileMenu->addAction(tr("&Save"), this, SLOT(Save()),
                            QKeySequence(tr("Ctrl+S",
                                            "File|Save")));
      pcFileMenu->addAction(tr("S&ave As..."), this, SLOT(SaveAs()));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaMainWindow::CreateLuaActions() {
      QMenu* pcLuaMenu = new QMenu(tr("&Lua"), this);
      menuBar()->addMenu(pcLuaMenu);
      pcLuaMenu->addAction(tr("&Execute"), this, SLOT(Execute()),
                           QKeySequence(tr("Ctrl+E",
                                           "Lua|Execute")));
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
   
   void CQTOpenGLLuaMainWindow::closeEvent(QCloseEvent* pc_event) {
      WriteSettings();
      pc_event->accept();
   }

   /****************************************/
   /****************************************/

}
