/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_spiri_entity_dialog_layout.h>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#ifndef QTOPENGL_SPIRI_ENTITY_DIALOG_LAYOUT
#define QTOPENGL_SPIRI_ENTITY_DIALOG_LAYOUT

namespace argos {
    class CQTOpenGLSpiriEntityDialogLayout;
}

#include <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_bot_entity_dialog_layout.h>

#include <QCheckBox>

namespace argos {

    class CQTOpenGLSpiriEntityDialogLayout : public CQTOpenGLBotEntityDialogLayout {

        Q_OBJECT

        public:

            /**
             * Constructor.
             * @param pc_parent_widget The pointer to the parent widget
             */
            CQTOpenGLSpiriEntityDialogLayout(QWidget* pc_parent_widget = 0);

            /**
             * Creates and returns the spiri entity using the specified values
             * @returns The entity using the specified values
             */
            virtual CEntity& GetEntity();

            /**
             * Getter for the range and bearing range
             * @returns The real number containing the specified range and bearing range value
             */
            inline Real GetRabRange() const
            {
                return m_pcEnityRabRangeLineEdit->text().toDouble();
            }

            /**
             * Getter for the range and bearing data size
             * @returns The natural number containing the specified range and bearing data size value
             */
            inline size_t GetRabDataSize() const
            {
                return m_pcEnityRabDataSizeLineEdit->text().toUInt();
            }

            /**
             * Getter for the perspective camera aperture
             * @returns The real number containing the specified perspective camera aperture value
             */
            inline Real GetPerspCamAperture() const
            {
                return m_pcEnityPerspCamApertureLineEdit->text().toDouble();
            }

            /**
             * Getter for the perspective camera focal length
             * @returns The real number containing the specified perspective camera focal length value
             */
            inline Real GetPerspCamRange() const
            {
                return m_pcEnityPerspCamRangeLineEdit->text().toDouble();
            }

        protected:

            /**
             * Validates the filled in values
             */
            virtual void CheckInput();

        private:

            /**
             * Initializes the layout
             */
            virtual void Init();

        public slots:

        private:
            // rab input fields
            QLineEdit* m_pcEnityRabRangeLineEdit;
            QLineEdit* m_pcEnityRabDataSizeLineEdit;

            // perspcam input fields
            QLineEdit* m_pcEnityPerspCamApertureLineEdit;
            QLineEdit* m_pcEnityPerspCamRangeLineEdit;
    };
}

#endif