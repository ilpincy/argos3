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
         /** What we are looking at in the global reference frame */
         CVector3 Target;
         /** The local Z axis of the camera in the global reference frame */
         CVector3 Up;
         /** The focal length of the lens (if this was a real camera) */
         Real LensFocalLength;
         /** The focal length of the camera */
         CDegrees YFieldOfView;
         /** Initialize from XML */
         void Init(TConfigurationNode& t_tree);
         /** Calculates the value of YFieldOfView */
         void CalculateYFieldOfView();
      };
 
      struct SKeyframe {
         SKeyframe(UInt32 un_step,
                   UInt32 un_placement_index,
                   bool b_interpolate_to_next) :
            Step(un_step),
            PlacementIndex(un_placement_index),
            InterpolateToNext(b_interpolate_to_next) {}
         /** The step at which this keyframe should be applied */
         UInt32 Step;
         /** An index into the camera placements array [0-12) */
         UInt32 PlacementIndex;
         /** Whether or not linear interpolation should be applied */
         bool InterpolateToNext;
      };

   public:

      CQTOpenGLCamera();

      ~CQTOpenGLCamera();

      void Init(TConfigurationNode& t_tree);

      void Reset();

      void Look();

      inline const CVector3& GetPosition() const {
         return m_sActivePlacement.Position;
      }

      inline const CVector3& GetTarget() const {
         return m_sActivePlacement.Target;
      }

      inline Real GetLensFocalLength() const {
         return m_sActivePlacement.LensFocalLength;
      }

      void Rotate(const QPoint& c_delta);

      void Rotate(const CRadians& c_up_down,
                  const CRadians& c_left_right);

      void Move(SInt32 n_forwards_backwards,
                SInt32 n_sideways,
                SInt32 n_up_down);

      void Interpolate(UInt32 un_start_placement,
                       UInt32 un_end_placement,
                       Real f_time_fraction);

      inline SPlacement& GetActivePlacement() {
         return m_sActivePlacement;
      }

      inline const SPlacement& GetActivePlacement() const {
         return m_sActivePlacement;
      }

      inline void SetActivePlacement(UInt32 n_index) {
         ARGOS_ASSERT(n_index < m_arrPlacements.size(),
            "CQTOpenGLCamera::SetActivePlacement():"
            " index out of bounds: n_index = " << n_index <<
            ", m_arrPlacements.size() = " << m_arrPlacements.size());
         m_sActivePlacement = m_arrPlacements[n_index];
      }

      inline SPlacement& GetPlacement(UInt32 n_index) {
         ARGOS_ASSERT(n_index < m_arrPlacements.size(),
            "CQTOpenGLCamera::GetPlacement():"
            " index out of bounds: n_index = " << n_index <<
            ", m_arrPlacements.size() = " << m_arrPlacements.size());
         return m_arrPlacements[n_index];
      }

      void UpdateTimeline();

   private:
      bool m_bEnableTimeline;
      bool m_bHasTimeline;
      UInt32 m_unLoop;
      SPlacement m_sActivePlacement;
      std::array<SPlacement, 12> m_arrPlacements;
      std::vector<SKeyframe> m_vecKeyframes;
   };

}

#endif

