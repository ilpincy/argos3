#include "qtopengl_lua_editor.h"

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

   void CQTOpenGLLuaEditor::NewFile() {
      m_pcCodeEditor->clear();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::OpenFile(const QString& str_path) {
      m_strFileName = str_path;
      if (m_strFileName.isNull())
         m_strFileName = QFileDialog::getOpenFileName(this,
                                                      tr("Open File"),
                                                      "",
                                                      "Lua Files (*.lua)");
      if (!m_strFileName.isEmpty()) {
         QFile cFile(m_strFileName);
         if(cFile.open(QFile::ReadOnly | QFile::Text)) {
            m_pcCodeEditor->setPlainText(cFile.readAll());
            SetCurrentFile(m_strFileName);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::SaveFile(const QString& str_path) {
      QString strNewFileName = str_path;
      if (strNewFileName.isNull()) {
         strNewFileName = m_strFileName;
      }
      if (strNewFileName.isNull()) {
         strNewFileName = QFileDialog::getSaveFileName(this,
                                                       tr("Save File"),
                                                       "",
                                                       "Lua Files (*.lua)");
      }
      if (!strNewFileName.isEmpty()) {
         QFile cFile(m_strFileName);
         if(cFile.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream cOut(&cFile);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            cOut << m_pcCodeEditor->toPlainText();
            QApplication::restoreOverrideCursor();
            SetCurrentFile(strNewFileName);
            statusBar()->showMessage(tr("File saved"), 2000);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::Execute() {
      if(m_pcCodeEditor->document()->isModified()) {
         QMessageBox::StandardButton tReply;
         tReply = QMessageBox::warning(this, tr("ARGoS v3.0 - Lua Editor"),
                                       tr("The document has been modified.\n"
                                          "Do you want to save your changes?"),
                                       QMessageBox::Save | QMessageBox::Cancel);
         if(tReply == QMessageBox::Cancel) {
            return;
         }
      }
      QApplication::setOverrideCursor(Qt::WaitCursor);
      for(size_t i = 0; i < m_vecControllers.size(); ++i) {
         m_vecControllers[i]->SetLuaScript(m_strFileName.toStdString());
      }
      QApplication::restoreOverrideCursor();
      statusBar()->showMessage(tr("Execution started"), 2000);
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
      setCentralWidget(m_pcCodeEditor);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::CreateFileActions() {
      QMenu* pcFileMenu = new QMenu(tr("&File"), this);
      menuBar()->addMenu(pcFileMenu);
      pcFileMenu->addAction(tr("&New"), this, SLOT(NewFile()),
                            QKeySequence(tr("Ctrl+N",
                                            "File|New")));
      pcFileMenu->addAction(tr("&Open..."), this, SLOT(OpenFile()),
                            QKeySequence(tr("Ctrl+O",
                                            "File|Open")));
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
   
   void CQTOpenGLLuaEditor::CodeModified() {
      setWindowModified(m_pcCodeEditor->document()->isModified());
   }

   /****************************************/
   /****************************************/

}
