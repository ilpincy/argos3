/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_camera.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_CAMERA_H
#define QTOPENGL_CAMERA_H

namespace argos {
   class CQTOpenGLCamera;
}

class QPoint;

#include <QObject>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

#ifdef __APPLE__
#include <gl.h>
#include <glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

namespace argos {

   class CQTOpenGLCamera : public QObject {

      Q_OBJECT

   public:

      struct SPlacement {
         /** The position of the camera in the global reference frame */
         CVector3 Position;
         /** The local Y axis of the camera in the global reference frame */
         CVector3 Left;
         /** The local Z axis of the camera in the global reference frame */
         CVector3 Up;
         /** The local X axis of the camera in the global reference frame */
         CVector3 Forward;
         /** The direction of sight of the camera in the global reference frame */
         CVector3 Target;
         /** The focal length of the lens (if this was a real camera) */
         Real LensFocalLength;
         /** The focal length of the camera */
         CDegrees YFieldOfView;
         /** Motion sensitivity */
         Real MotionSensitivity;
         /** Rotation sensitivity */
         Real RotationSensitivity;

         SPlacement() :
            Position(-2.0f, 0.0f, 2.0f),
            Left(CVector3::Y),
            Up(CVector3(1.0f, 0.0f, 1.0f).Normalize()),
            Forward(CVector3(1.0f, 0.0f, -1.0f).Normalize()),
            Target(),
            LensFocalLength(0.02f),
            MotionSensitivity(0.005),
            RotationSensitivity(0.01) {
            CalculateYFieldOfView();
         }

         /** Initialize from XML */
         void Init(TConfigurationNode& t_tree);
         /** Rotation around the local Y axis */
         void RotateUpDown(const CRadians& c_angle);
         /** Rotation around the local Z axis */
         void RotateLeftRight(const CRadians& c_angle);
         /** Rotation around the global Z axis */
         void RotateLeftRight2(const CRadians& c_angle);
         /** c_delta is expressed in the camera local coordinates */
         void Translate(const CVector3& c_delta);
         /** Places this camera in the right position */
         void Do();
         /** Calculates the value of YFieldOfView */
         void CalculateYFieldOfView();
         /** Calculate the sensitivity of the camera */
         void CalculateSensitivity();
      };

      struct STimelineItem {
         /** Index of the placement */
         UInt32 Idx;
         /** Time step to switch to the indicated placement */
         UInt32 Step;
         /** Initialize from XML */
         void Init(TConfigurationNode& t_tree);
      };

   public:

      CQTOpenGLCamera();
      ~CQTOpenGLCamera();

      void Init(TConfigurationNode& t_tree);

      void Reset();

      inline void Look() {
         m_sPlacement[m_unActivePlacement].Do();
      }

      inline const CVector3& GetPosition() const {
         return m_sPlacement[m_unActivePlacement].Position;
      }

      inline const CVector3& GetTarget() const {
         return m_sPlacement[m_unActivePlacement].Target;
      }

      inline Real GetLensFocalLength() const {
         return m_sPlacement[m_unActivePlacement].LensFocalLength;
      }

      void Rotate(const QPoint& c_delta);

      void Move(SInt32 n_forwards_backwards,
                SInt32 n_sideways,
                SInt32 n_up_down);

      inline SPlacement& GetActivePlacement() {
         return m_sPlacement[m_unActivePlacement];
      }

      inline const SPlacement& GetActivePlacement() const {
         return m_sPlacement[m_unActivePlacement];
      }

      inline void SetActivePlacement(UInt32 un_placement) {
         m_unActivePlacement = un_placement;
      }

      inline SPlacement& GetPlacement(UInt32 n_index) {
         return m_sPlacement[n_index];
      }

      void UpdateTimeline();

   private:

      UInt32 m_unActivePlacement;
      SPlacement m_sPlacement[12];
      std::list<STimelineItem*> m_listTimeline;

   };

}

#endif

