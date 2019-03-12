/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_footbot_entity_dialog_layout.cpp>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#include "qtopengl_footbot_entity_dialog_layout.h"

#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>

#include <QGroupBox>
#include <QLabel>

namespace argos {

    /****************************************/
    /****************************************/

    CQTOpenGLFootBotEntityDialogLayout::CQTOpenGLFootBotEntityDialogLayout(QWidget* pc_parent_widget) 
        : CQTOpenGLBotEntityDialogLayout(pc_parent_widget)
    { 
        this->Init();
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLFootBotEntityDialogLayout::Init() 
    {
        // initialize rab input fields
        QLabel* pcEnityRabRangeLabel = new QLabel(tr("Range"));
        QLabel* pcEnityRabDataSizeLabel = new QLabel(tr("Data size"));
        m_pcEnityRabRangeLineEdit = new QLineEdit;
        m_pcEnityRabDataSizeLineEdit = new QLineEdit;
        m_pcEnityRabRangeLineEdit->setPlaceholderText("real number");
        m_pcEnityRabDataSizeLineEdit->setPlaceholderText("natural number");

        // initialize omnicam aperture input field
        QLabel* pcEnityOmniCamApertureLabel = new QLabel(tr("Aperture"));
        m_pcEnityOmniCamApertureLineEdit = new QLineEdit;
        m_pcEnityOmniCamApertureLineEdit->setPlaceholderText("real number (in degrees)");

        // initialize perspcam input fields
        QLabel* pcEnityIsPerspCamFrontLabel = new QLabel(tr("In front"));
        QLabel* pcEnityPerspCamApertureLabel = new QLabel(tr("Aperture"));
        QLabel* pcEnityPerspCamFocalLengthLabel = new QLabel(tr("Focal length"));
        QLabel* pcEnityPerspCamRangeLabel = new QLabel(tr("Range"));
        m_pcEnityIsPerspCamFrontCheckBox = new QCheckBox;
        m_pcEnityPerspCamApertureLineEdit = new QLineEdit;
        m_pcEnityPerspCamFocalLengthLineEdit = new QLineEdit;
        m_pcEnityPerspCamRangeLineEdit = new QLineEdit;
        m_pcEnityIsPerspCamFrontCheckBox->setChecked(1);
        m_pcEnityPerspCamApertureLineEdit->setPlaceholderText("real number (in degrees)");
        m_pcEnityPerspCamFocalLengthLineEdit->setPlaceholderText("real number (in meters)");
        m_pcEnityPerspCamRangeLineEdit->setPlaceholderText("real number (in meters)");

        // add rab input fields to group box
        QGroupBox* pcEntityRabGroupBox = new QGroupBox(tr("Range and bearing"));
        QHBoxLayout *pcEntityRabGroupBoxLayout = new QHBoxLayout;
        pcEntityRabGroupBoxLayout->addWidget(pcEnityRabRangeLabel);
        pcEntityRabGroupBoxLayout->addWidget(m_pcEnityRabRangeLineEdit);
        pcEntityRabGroupBoxLayout->addWidget(pcEnityRabDataSizeLabel);
        pcEntityRabGroupBoxLayout->addWidget(m_pcEnityRabDataSizeLineEdit);
        pcEntityRabGroupBox->setLayout(pcEntityRabGroupBoxLayout);

        // add omnicam aperture input field to group box
        QGroupBox* pcEntityOmniApertureGroupBox = new QGroupBox(tr("Omnidirectional camera"));
        QHBoxLayout *pcEntityOmniApertureGroupBoxLayout = new QHBoxLayout;
        pcEntityOmniApertureGroupBoxLayout->addWidget(pcEnityOmniCamApertureLabel);
        pcEntityOmniApertureGroupBoxLayout->addWidget(m_pcEnityOmniCamApertureLineEdit);
        pcEntityOmniApertureGroupBox->setLayout(pcEntityOmniApertureGroupBoxLayout);

        // add perspcam input fields to group box
        QGroupBox* pcEntityPerspCamGroupBox = new QGroupBox(tr("Perspective camera"));
        QHBoxLayout *pcEntityPerspCamGroupBoxLayout = new QHBoxLayout;
        pcEntityPerspCamGroupBoxLayout->addWidget(pcEnityIsPerspCamFrontLabel);
        pcEntityPerspCamGroupBoxLayout->addWidget(m_pcEnityIsPerspCamFrontCheckBox);
        pcEntityPerspCamGroupBoxLayout->addWidget(pcEnityPerspCamApertureLabel);
        pcEntityPerspCamGroupBoxLayout->addWidget(m_pcEnityPerspCamApertureLineEdit);
        pcEntityPerspCamGroupBoxLayout->addWidget(pcEnityPerspCamFocalLengthLabel);
        pcEntityPerspCamGroupBoxLayout->addWidget(m_pcEnityPerspCamFocalLengthLineEdit);
        pcEntityPerspCamGroupBoxLayout->addWidget(pcEnityPerspCamRangeLabel);
        pcEntityPerspCamGroupBoxLayout->addWidget(m_pcEnityPerspCamRangeLineEdit);
        pcEntityPerspCamGroupBox->setLayout(pcEntityPerspCamGroupBoxLayout);

        // add group boxes to layout
        this->addWidget(pcEntityRabGroupBox);
        this->addWidget(pcEntityOmniApertureGroupBox);
        this->addWidget(pcEntityPerspCamGroupBox);
    }

    /****************************************/
    /****************************************/

    void CQTOpenGLFootBotEntityDialogLayout::CheckInput() 
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

        // check perspective camera focal length
        if(this->GetPerspCamFocalLength() < 0){
            THROW_ARGOSEXCEPTION("Please fill in a positive focal length for the perspective camera of the entity.");
        }

        // check perspective camera range
        if(this->GetPerspCamRange() < 0){
            THROW_ARGOSEXCEPTION("Please fill in a positive range for the perspective camera of the entity.");
        }
    }

    /****************************************/
    /****************************************/

    CEntity& CQTOpenGLFootBotEntityDialogLayout::GetEntity()
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

        // calculate omnicam aperture
        Real fEntityOmniCamAperture = 70.0f;
        if(m_pcEnityOmniCamApertureLineEdit->text().toStdString() != ""){
            fEntityOmniCamAperture = this->GetOmniCamAperture();
        }
        CRadians fEntityOmniCamApertureInRadians = ToRadians(CDegrees(fEntityOmniCamAperture));

        // calculate perspcam aperture
        Real fEntityPerspCamAperture = 30.0f;
        if(m_pcEnityPerspCamApertureLineEdit->text().toStdString() != ""){
            fEntityPerspCamAperture = this->GetPerspCamAperture();
        }
        CRadians fEntityPerspCamApertureInRadians = ToRadians(CDegrees(fEntityPerspCamAperture));

        // calculate perspcam focal length
        Real fEntityPerspCamFocalLength = 0.035f;
        if(m_pcEnityPerspCamFocalLengthLineEdit->text().toStdString() != ""){
            fEntityPerspCamFocalLength = this->GetPerspCamFocalLength();
        }

        // calculate perspcam range
        Real fEntityPerspCamRange = 2.0f;
        if(m_pcEnityPerspCamRangeLineEdit->text().toStdString() != ""){
            fEntityPerspCamRange = this->GetPerspCamRange();
        }

        // create new entity
        CFootBotEntity* cFootBotEnt = new CFootBotEntity(
            this->GetId(),
            this->GetControllerId(),
            this->GetPositition(),
            this->GetOrientation(),
            fEntityRabRange,
            fEntityRabDataSize,
            "",
            fEntityOmniCamApertureInRadians,
            this->IsPerspCamFront(),
            fEntityPerspCamApertureInRadians,
            fEntityPerspCamFocalLength,
            fEntityPerspCamRange
            );

        return *cFootBotEnt;

    }

    /****************************************/
    /****************************************/

}