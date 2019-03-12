/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_light_entity_dialog_layout.h>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#ifndef QTOPENGL_LIGHT_ENTITY_DIALOG_LAYOUT
#define QTOPENGL_LIGHT_ENTITY_DIALOG_LAYOUT

namespace argos {
    class CQTOpenGLLightEntityDialogLayout;
}

#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_entity_dialog_layout.h>

#include <QComboBox>

namespace argos {

    class CQTOpenGLLightEntityDialogLayout : public CQTOpenGLEntityDialogLayout {

        Q_OBJECT

        public:

            /**
             * Constructor.
             * @param pc_parent_widget The pointer to the parent widget
             */
            CQTOpenGLLightEntityDialogLayout(QWidget* pc_parent_widget = 0);

            /**
             * Creates and returns the light entity using the specified values
             * @returns The entity using the specified values
             */
            virtual CEntity& GetEntity();

            /**
             * Getter for the position
             * @returns The vector containing the specified x,y,z values
             */
            inline CVector3 GetPositition() const
            {
                return CVector3(
                    m_pcEnityPositionXLineEdit->text().toDouble(),
                    m_pcEnityPositionYLineEdit->text().toDouble(),
                    m_pcEnityPositionZLineEdit->text().toDouble()
                    );
            }

            /**
             * Getter for the color
             * @returns The CColor containing the specified color value
             */
            inline CColor GetColor() const
            {
                std::string strColor = m_pcEnityColorComboBox->currentText().toStdString();
                return CColor::GetColor(strColor);
            }

            /**
             * Getter for the intensity
             * @returns The real number containing the specified intensity value
             */
            inline Real GetIntensity() const
            {
                return m_pcEnityIntensityLineEdit->text().toDouble();
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
            // position input fields
            QLineEdit* m_pcEnityPositionXLineEdit;
            QLineEdit* m_pcEnityPositionYLineEdit;
            QLineEdit* m_pcEnityPositionZLineEdit;

            // color input field
            QComboBox* m_pcEnityColorComboBox;
            
            // intensity input field
            QLineEdit* m_pcEnityIntensityLineEdit;
    };
}

#endif