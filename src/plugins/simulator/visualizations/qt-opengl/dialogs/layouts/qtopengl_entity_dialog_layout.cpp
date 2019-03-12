/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_entity_dialog_layout.cpp>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#include "qtopengl_entity_dialog_layout.h"

namespace argos {

    /****************************************/
    /****************************************/

    CQTOpenGLEntityDialogLayout::CQTOpenGLEntityDialogLayout(QWidget* pc_parent_widget) 
        : QVBoxLayout(pc_parent_widget)
    { 
        this->Init();
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLEntityDialogLayout::Init() 
    {
        // initialize id input field
        m_pcEnityIdLineEdit = new QLineEdit;
        m_pcEnityIdLineEdit->setPlaceholderText("string");

        // add id input field to group box
        std::string strEntityIdGroupBoxTitle = "Entity id*";
        QGroupBox* pcEntityIdGroupBox = 
            GetHorizontalGroupBox(strEntityIdGroupBoxTitle, m_pcEnityIdLineEdit);

        // add group box to layout
        this->addWidget(pcEntityIdGroupBox);
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLEntityDialogLayout::SetEntityIdLineEditText(const std::string& str_entity_id)
    {
        m_pcEnityIdLineEdit->setText(QString(str_entity_id.c_str()));
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLEntityDialogLayout::DisableEntityIdLineEdit()
    {
        m_pcEnityIdLineEdit->setDisabled(1);
    }
    
    /****************************************/
    /****************************************/

    void CQTOpenGLEntityDialogLayout::EnableEntityIdLineEdit()
    {
        m_pcEnityIdLineEdit->setEnabled(1);
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLEntityDialogLayout::CheckInput() 
    {
        // check id
        if(this->GetId() == ""){
            THROW_ARGOSEXCEPTION("Please fill in the id of the entity.");
        }
    }

    /****************************************/
    /****************************************/

    CEntity& CQTOpenGLEntityDialogLayout::GetEntity()
    {
        THROW_ARGOSEXCEPTION("In CQTOpenGLEntityDialogLayout::GetEntity(), " 
            << "there is no entity to get.");
    }

    /****************************************/
    /****************************************/

    QGroupBox* CQTOpenGLEntityDialogLayout::GetHorizontalGroupBox(std::string& str_title, 
        QLineEdit* c_line_edit) const
    {
        QGroupBox* pcGroupBox = new QGroupBox(str_title.c_str());
        QHBoxLayout *pcGroupBoxLayout = new QHBoxLayout;
        pcGroupBoxLayout->addWidget(c_line_edit);
        pcGroupBox->setLayout(pcGroupBoxLayout);

        return pcGroupBox;
    }

    /****************************************/
    /****************************************/    

    QGroupBox* CQTOpenGLEntityDialogLayout::GetHorizontalGroupBox(std::string& str_title, 
        QLabel* c_label, QLineEdit* c_line_edit) const
    {
        QGroupBox* pcGroupBox = new QGroupBox(str_title.c_str());
        QHBoxLayout *pcGroupBoxLayout = new QHBoxLayout;
        pcGroupBoxLayout->addWidget(c_label);
        pcGroupBoxLayout->addWidget(c_line_edit);
        pcGroupBox->setLayout(pcGroupBoxLayout);

        return pcGroupBox;
    }

    /****************************************/
    /****************************************/
}
