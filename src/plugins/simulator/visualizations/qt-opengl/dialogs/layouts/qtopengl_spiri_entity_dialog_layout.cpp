/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_spiri_entity_dialog_layout.cpp>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#include "qtopengl_spiri_entity_dialog_layout.h"

#include <argos3/plugins/robots/spiri/simulator/spiri_entity.h>

#include <QGroupBox>
#include <QLabel>

namespace argos {

    /****************************************/
    /****************************************/

    CQTOpenGLSpiriEntityDialogLayout::CQTOpenGLSpiriEntityDialogLayout(QWidget* pc_parent_widget) 
        : CQTOpenGLBotEntityDialogLayout(pc_parent_widget)
    { 
        this->Init();
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLSpiriEntityDialogLayout::Init() 
    {
        // initialize rab input fields
        QLabel* pcEnityRabRangeLabel = new QLabel(tr("Range"));
        QLabel* pcEnityRabDataSizeLabel = new QLabel(tr("Data size"));
        m_pcEnityRabRangeLineEdit = new QLineEdit;
        m_pcEnityRabDataSizeLineEdit = new QLineEdit;
        m_pcEnityRabRangeLineEdit->setPlaceholderText("real number");
        m_pcEnityRabDataSizeLineEdit->setPlaceholderText("natural number");

        // initialize perspcam input fields
        QLabel* pcEnityPerspCamApertureLabel = new QLabel(tr("Aperture"));
        QLabel* pcEnityPerspCamRangeLabel = new QLabel(tr("Range"));
        m_pcEnityPerspCamApertureLineEdit = new QLineEdit;
        m_pcEnityPerspCamRangeLineEdit = new QLineEdit;
        m_pcEnityPerspCamApertureLineEdit->setPlaceholderText("real number (in degrees)");
        m_pcEnityPerspCamRangeLineEdit->setPlaceholderText("real number (in meters)");

        // add rab input fields to group box
        QGroupBox* pcEntityRabGroupBox = new QGroupBox(tr("Range and bearing"));
        QHBoxLayout *pcEntityRabGroupBoxLayout = new QHBoxLayout;
        pcEntityRabGroupBoxLayout->addWidget(pcEnityRabRangeLabel);
        pcEntityRabGroupBoxLayout->addWidget(m_pcEnityRabRangeLineEdit);
        pcEntityRabGroupBoxLayout->addWidget(pcEnityRabDataSizeLabel);
        pcEntityRabGroupBoxLayout->addWidget(m_pcEnityRabDataSizeLineEdit);
        pcEntityRabGroupBox->setLayout(pcEntityRabGroupBoxLayout);

        // add perspcam input fields to group box
        QGroupBox* pcEntityPerspCamGroupBox = new QGroupBox(tr("Perspective camera"));
        QHBoxLayout *pcEntityPerspCamGroupBoxLayout = new QHBoxLayout;
        pcEntityPerspCamGroupBoxLayout->addWidget(pcEnityPerspCamApertureLabel);
        pcEntityPerspCamGroupBoxLayout->addWidget(m_pcEnityPerspCamApertureLineEdit);
        pcEntityPerspCamGroupBoxLayout->addWidget(pcEnityPerspCamRangeLabel);
        pcEntityPerspCamGroupBoxLayout->addWidget(m_pcEnityPerspCamRangeLineEdit);
        pcEntityPerspCamGroupBox->setLayout(pcEntityPerspCamGroupBoxLayout);

        // add group boxes to layout
        this->addWidget(pcEntityRabGroupBox);
        this->addWidget(pcEntityPerspCamGroupBox);
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLSpiriEntityDialogLayout::CheckInput() 
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

        // check perspective camera range
        if(this->GetPerspCamRange() < 0){
            THROW_ARGOSEXCEPTION("Please fill in a positive range for the perspective camera of the entity.");
        }
    }

    /****************************************/
    /****************************************/

    CEntity& CQTOpenGLSpiriEntityDialogLayout::GetEntity()
    {
        this->CheckInput();

        // calculate range and bearing range
        Real fEntityRabRange = 3.0f;
        if(m_pcEnityRabRangeLineEdit->text().toStdString() != ""){
            fEntityRabRange = this->GetRabRange();
        }

        // calculate range and bearing data size
        size_t fEntityRabDataSize = 10;
        if(m_pcEnityRabDataSizeLineEdit->text().toStdString() != "") {
            fEntityRabDataSize = this->GetRabDataSize();
        }

        // calculate perspcam aperture
        Real fEntityPerspCamAperture = 30.0f;
        if(m_pcEnityPerspCamApertureLineEdit->text().toStdString() != ""){
            fEntityPerspCamAperture = this->GetPerspCamAperture();
        }
        CRadians fEntityPerspCamApertureInRadians = ToRadians(CDegrees(fEntityPerspCamAperture));

        // calculate perspcam range
        Real fEntityPerspCamRange = 10.0f;
        if(m_pcEnityPerspCamRangeLineEdit->text().toStdString() != ""){
            fEntityPerspCamRange = this->GetPerspCamRange();
        }

        // create new entity
        CSpiriEntity* cSpiriEnt = new CSpiriEntity(
            this->GetId(),
            this->GetControllerId(),
            this->GetPositition(),
            this->GetOrientation(),
            fEntityRabRange,
            fEntityRabDataSize,
            "",
            fEntityPerspCamApertureInRadians,
            fEntityPerspCamRange
            );

        return *cSpiriEnt;

    }

    /****************************************/
    /****************************************/

}