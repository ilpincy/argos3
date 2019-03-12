/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_epuck_entity_dialog_layout.cpp>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#include "qtopengl_epuck_entity_dialog_layout.h"

#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

#include <QGroupBox>
#include <QLabel>

namespace argos {

    /****************************************/
    /****************************************/

    CQTOpenGLEPuckEntityDialogLayout::CQTOpenGLEPuckEntityDialogLayout(QWidget* pc_parent_widget) 
        : CQTOpenGLBotEntityDialogLayout(pc_parent_widget)
    { 
        this->Init();
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLEPuckEntityDialogLayout::Init() 
    {
        // initialize rab input fields
        QLabel* pcEnityRabRangeLabel = new QLabel(tr("Range"));
        QLabel* pcEnityRabDataSizeLabel = new QLabel(tr("Data size"));
        m_pcEnityRabRangeLineEdit = new QLineEdit;
        m_pcEnityRabDataSizeLineEdit = new QLineEdit;
        m_pcEnityRabRangeLineEdit->setPlaceholderText("real number");
        m_pcEnityRabDataSizeLineEdit->setPlaceholderText("natural number");

        // add rab input fields to group box
        QGroupBox* pcEntityRabGroupBox = new QGroupBox(tr("Range and bearing"));
        QHBoxLayout *pcEntityRabGroupBoxLayout = new QHBoxLayout;
        pcEntityRabGroupBoxLayout->addWidget(pcEnityRabRangeLabel);
        pcEntityRabGroupBoxLayout->addWidget(m_pcEnityRabRangeLineEdit);
        pcEntityRabGroupBoxLayout->addWidget(pcEnityRabDataSizeLabel);
        pcEntityRabGroupBoxLayout->addWidget(m_pcEnityRabDataSizeLineEdit);
        pcEntityRabGroupBox->setLayout(pcEntityRabGroupBoxLayout);

        // add group box to layout
        this->addWidget(pcEntityRabGroupBox);
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLEPuckEntityDialogLayout::CheckInput() 
    {
        CQTOpenGLEmbodiedEntityDialogLayout::CheckInput();

        // check range and bearing range
        if(this->GetRabRange() < 0){
            THROW_ARGOSEXCEPTION("Please fill in a positive rab range of the entity.");
        }

        // check range and bearing data size
        if(this->GetRabDataSize() < 0){
            THROW_ARGOSEXCEPTION("Please fill in a positive rab data size of the entity.");
        }
    }

    /****************************************/
    /****************************************/

    CEntity& CQTOpenGLEPuckEntityDialogLayout::GetEntity()
    {
        this->CheckInput();

        // calculate range and bearing range
        Real fEntityRabRange = 0.8f;
        if(m_pcEnityRabRangeLineEdit->text().toStdString() != ""){
            fEntityRabRange = this->GetRabRange();
        }

        // calculate range and bearing data size
        size_t fEntityRabDataSize = 2;
        if(m_pcEnityRabDataSizeLineEdit->text().toStdString() != "") {
            fEntityRabDataSize = this->GetRabDataSize();
        }

        // create new entity
        CEPuckEntity* cEPuckEnt = new CEPuckEntity(
            this->GetId(),
            this->GetControllerId(),
            this->GetPositition(),
            this->GetOrientation(),
            fEntityRabRange,
            fEntityRabDataSize
            );

        return *cEPuckEnt;

    }

    /****************************************/
    /****************************************/

}