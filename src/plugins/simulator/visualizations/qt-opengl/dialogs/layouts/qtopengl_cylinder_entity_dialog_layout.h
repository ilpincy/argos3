/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_cylinder_entity_dialog_layout.h>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#ifndef QTOPENGL_CYLINDER_ENTITY_DIALOG_LAYOUT
#define QTOPENGL_CYLINDER_ENTITY_DIALOG_LAYOUT

namespace argos {
    class CQTOpenGLCylinderEntityDialogLayout;
}

#include <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_embodied_entity_dialog_layout.h>

#include <QCheckBox>

namespace argos {

    class CQTOpenGLCylinderEntityDialogLayout : public CQTOpenGLEmbodiedEntityDialogLayout {

        Q_OBJECT

        public:

            /**
             * Constructor.
             * @param pc_parent_widget The pointer to the parent widget
             */
            CQTOpenGLCylinderEntityDialogLayout(QWidget* pc_parent_widget = 0);

            /**
             * Creates and returns the cylinder entity using the specified values
             * @returns The entity using the specified values
             */
            virtual CEntity& GetEntity();

            /**
             * Getter for the radius
             * @returns The real number containing the specified radius value
             */
            inline Real GetRadius() const
            {
                return m_pcEnitySizeRadiusLineEdit->text().toDouble();
            }

            /**
             * Getter for the height
             * @returns The real number containing the specified height value
             */
            inline Real GetHeight() const
            {
                return m_pcEnitySizeHeightLineEdit->text().toDouble();
            }

            /**
             * Getter for the movable input field
             * If the check-box is checked: true
             * If the check-box isn't checked: false
             * @returns The boolean for the movable attribute
             */
            inline bool IsMovable() const
            {
                return m_pcEnityIsMovableCheckBox->isChecked();
            }

            /**
             * Getter for the mass
             * @returns The real number containing the specified mass value
             */
            inline Real GetMass() const
            {
                return m_pcEnityMassLineEdit->text().toDouble();
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

            /**
             * This method is called when the check-box of the movable property
             * is checked or unchecked.
             * This will enable or disable the mass input field
             */
            void MovableCheckboxToggled(bool b_is_checked);

        private:
            // size input fields
            QLineEdit* m_pcEnitySizeRadiusLineEdit;
            QLineEdit* m_pcEnitySizeHeightLineEdit;

            // movable input field
            QCheckBox* m_pcEnityIsMovableCheckBox;

            // mass input field
            QLineEdit* m_pcEnityMassLineEdit;
    };
}

#endif