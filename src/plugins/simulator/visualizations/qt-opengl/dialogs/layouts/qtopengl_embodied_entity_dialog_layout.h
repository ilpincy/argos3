/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_embodied_entity_dialog_layout.h>
 *
 * @author Ewout Merckx - <ewoutmerckx.pro@gmail.com>
 */

#ifndef QTOPENGL_EMBODIED_ENTITY_DIALOG_LAYOUT
#define QTOPENGL_EMBODIED_ENTITY_DIALOG_LAYOUT

namespace argos {
    class CQTOpenGLEmbodiedEntityDialogLayout;
}

#include <argos3/plugins/simulator/visualizations/qt-opengl/dialogs/layouts/qtopengl_entity_dialog_layout.h>

#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

    class CQTOpenGLEmbodiedEntityDialogLayout : public CQTOpenGLEntityDialogLayout {

        Q_OBJECT

        public:

            /**
             * Constructor.
             * @param pc_parent_widget The pointer to the parent widget
             */
            CQTOpenGLEmbodiedEntityDialogLayout(QWidget* pc_parent_widget = 0);
            
            /**
             * Creates and returns the entity using the specified values
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
             * Getter for the orientation
             * @returns The quaternion containing the specified w,x,y,z values
             */
            inline CQuaternion GetOrientation() const
            {
                return CQuaternion(
                    m_pcEnityOrientationWLineEdit->text().toDouble(),
                    m_pcEnityOrientationXLineEdit->text().toDouble(),
                    m_pcEnityOrientationYLineEdit->text().toDouble(),
                    m_pcEnityOrientationZLineEdit->text().toDouble()
                    );
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

            // orientation input fields
            QLineEdit* m_pcEnityOrientationWLineEdit;
            QLineEdit* m_pcEnityOrientationXLineEdit;
            QLineEdit* m_pcEnityOrientationYLineEdit;
            QLineEdit* m_pcEnityOrientationZLineEdit;
    };
}

#endif