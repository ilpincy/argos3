/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_bot_entity_dialog_layout.cpp>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#include "qtopengl_bot_entity_dialog_layout.h"

#include <QGroupBox>
#include <QLabel>
#include <QStringList>

namespace argos {

    /****************************************/
    /****************************************/

    CQTOpenGLBotEntityDialogLayout::CQTOpenGLBotEntityDialogLayout(QWidget* pc_parent_widget)
        : CQTOpenGLEmbodiedEntityDialogLayout(pc_parent_widget)
    {
        this->Init();
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLBotEntityDialogLayout::Init() 
    {
        // initialize controller id input field
        m_pcEnityControllerIdComboBox = new QComboBox;
        std::vector<std::string> vecControllerIds = CSimulator::GetInstance().GetControllerIds();
        QStringList cControllerIdsStringList;
        for(size_t i=0; i<vecControllerIds.size(); i++) {
            cControllerIdsStringList.append(vecControllerIds[i].c_str());
        }
        m_pcEnityControllerIdComboBox->addItems(cControllerIdsStringList);

        // add controller id input fields to group box
        QGroupBox* pcEntityControllerIdGroupBox = new QGroupBox(tr("Controller id*"));
        QHBoxLayout *pcEntityControllerIdGroupBoxLayout = new QHBoxLayout;
        pcEntityControllerIdGroupBoxLayout->addWidget(m_pcEnityControllerIdComboBox);
        pcEntityControllerIdGroupBox->setLayout(pcEntityControllerIdGroupBoxLayout);

        // add group box to layout
        this->insertWidget(0, pcEntityControllerIdGroupBox);
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLBotEntityDialogLayout::CheckInput() 
    {
        CQTOpenGLEmbodiedEntityDialogLayout::CheckInput();

        // check controller id
        if(this->GetControllerId() == "") {
            THROW_ARGOSEXCEPTION("Please define a controller for the entity.");
        }
    }

    /****************************************/
    /****************************************/

    CEntity& CQTOpenGLBotEntityDialogLayout::GetEntity()
    {
        THROW_ARGOSEXCEPTION("In CQTOpenGLBotEntityDialogLayout::GetEntity(), " 
            << "there is no entity to get.");
    }

    /****************************************/
    /****************************************/

}
