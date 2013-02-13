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
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QTextStream>

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLLuaEditor::CQTOpenGLLuaEditor(CQTOpenGLMainWindow* pc_main_window) {
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
   }

   /****************************************/
   /****************************************/

   CQTOpenGLLuaEditor::~CQTOpenGLLuaEditor() {
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::New() {
      if(MaybeSave()) {
         m_pcCodeEditor->clear();
         SetCurrentFile("");
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::Open() {
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

   bool CQTOpenGLLuaEditor::Save() {
      if (m_strFileName.isEmpty()) {
         return SaveAs();
      } else {
         return SaveFile(m_strFileName);
      }
   }

   /****************************************/
   /****************************************/

   bool CQTOpenGLLuaEditor::SaveAs() {
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

   void CQTOpenGLLuaEditor::Execute() {
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

   void CQTOpenGLLuaEditor::CodeModified() {
      setWindowModified(m_pcCodeEditor->document()->isModified());
   }

   /****************************************/
   /****************************************/

   bool CQTOpenGLLuaEditor::MaybeSave() {
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

   void CQTOpenGLLuaEditor::PopulateLuaControllers() {
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

   void CQTOpenGLLuaEditor::CreateCodeEditor() {
      QFont cFont;
      cFont.setFamily("Courier");
      cFont.setFixedPitch(true);
      cFont.setPointSize(10);
      m_pcCodeEditor = new QPlainTextEdit(this);
      m_pcCodeEditor->setFont(cFont);
      CQTOpenGLLuaSyntaxHighlighter* pcSyntaxHighlighter = new CQTOpenGLLuaSyntaxHighlighter(m_pcCodeEditor->document());
      setCentralWidget(m_pcCodeEditor);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::CreateFileActions() {
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

   void CQTOpenGLLuaEditor::CreateLuaActions() {
      QMenu* pcLuaMenu = new QMenu(tr("&Lua"), this);
      menuBar()->addMenu(pcLuaMenu);
      pcLuaMenu->addAction(tr("&Execute"), this, SLOT(Execute()),
                           QKeySequence(tr("Ctrl+X",
                                           "Lua|Execute")));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::OpenFile(const QString& str_path) {
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

   bool CQTOpenGLLuaEditor::SaveFile(const QString& str_path) {
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

   void CQTOpenGLLuaEditor::SetCurrentFile(const QString& str_path) {
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
   
}
