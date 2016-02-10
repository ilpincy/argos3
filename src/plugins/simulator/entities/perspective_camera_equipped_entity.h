/**
 * @file <argos3/plugins/simulator/entities/perspective_camera_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PERSPECTIVE_CAMERA_EQUIPPED_ENTITY_H
#define PERSPECTIVE_CAMERA_EQUIPPED_ENTITY_H

namespace argos {
   class CPerspectiveCameraEquippedEntity;
   class CEmbodiedEntity;
   struct SAnchor;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/physics_engine/physics_model.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CPerspectiveCameraEquippedEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      /**
       * Class constructor.
       * This constructor is meant to be used with the Init() method.
       * @param pc_parent The parent of this entity.
       */
      CPerspectiveCameraEquippedEntity(CComposableEntity* pc_parent);

      /**
       * Class constructor.
       * This constructor is meant to be standalone.
       * You should not call Init() after using this constructor, or
       * memory leaks are likely to happen.
       * @param pc_parent The parent of this entity.
       * @param str_id The id of this entity.
       * @param c_aperture The aperture of the visibility cone.
       * @param f_focal_length The camera focal length.
       * @param f_range The camera range.
       * @param n_width The image width in pixels.
       * @param n_height The image height in pixels.
       * @param s_anchor The anchor to which the camera is attached.
       */
      CPerspectiveCameraEquippedEntity(CComposableEntity* pc_parent,
                                       const std::string& str_id,
                                       const CRadians& c_aperture,
                                       Real f_focal_length,
                                       Real f_range,
                                       SInt32 n_width,
                                       SInt32 n_height,
                                       SAnchor& s_anchor);

      /**
       * Initializes the state of the entity from the XML configuration tree.
       * @throws CARGoSException if a parse error occurs
       */
      virtual void Init(TConfigurationNode& t_tree);

      virtual void Enable();

      virtual void Disable();

      /**
       * Returns the aperture of the visibility cone of the camera.
       * @return The aperture of the visibility cone of the camera.
       */
      inline const CRadians& GetAperture() const {
         return m_cAperture;
      }

      /**
       * Sets the aperture of the visibility cone of the camera.
       * @param c_aperture The aperture of the visibility cone of the camera.
       */
      inline void SetAperture(const CRadians& c_aperture) {
         m_cAperture = c_aperture;
      }

      /**
       * Returns the focal length of the camera.
       * @return The focal length of the camera.
       */
      inline Real GetFocalLength() const {
         return m_fFocalLength;
      }

      /**
       * Sets the focal length of the camera.
       * @param c_focallength The focal length of the camera.
       */
      inline void SetFocalLength(Real f_focal_length) {
         m_fFocalLength = f_focal_length;
      }

      /**
       * Returns the range of the camera.
       * @return The range of the camera.
       */
      inline Real GetRange() const {
         return m_fRange;
      }

      /**
       * Sets the range of the camera.
       * @param c_range The range of the camera.
       */
      inline void SetRange(Real f_range) {
         m_fRange = f_range;
      }

      /**
       * Returns the offset of the camera with respect to the reference point.
       * @return The offset of the camera with respect to the reference point.
       */
      inline const SAnchor& GetAnchor() const {
         return *m_psAnchor;
      }

      /**
       * Sets the anchor to which the camera is attached.
       * @param s_anchor anchor to which the camera is attached.
       */
      inline void SetAnchor(SAnchor& s_anchor) {
         m_psAnchor = &s_anchor;
      }

      /**
       * Returns the image width in pixel.
       * @return The image width in pixel.
       */
      inline SInt32 GetImagePxWidth() const {
         return m_nImagePxWidth;
      }

      /**
       * Returns the image height in pixel.
       * @return The image height in pixel.
       */
      inline SInt32 GetImagePxHeight() const {
         return m_nImagePxHeight;
      }

      /**
       * Sets the image size in pixel.
       * @param un_width The image width in pixel.
       * @param un_height The image height in pixel.
       */
      inline void SetImagePxSize(SInt32 n_width,
                                 SInt32 n_height) {
         m_nImagePxWidth  = n_width;
         m_nImagePxHeight = n_height;
         Real fAspectRatio =
            static_cast<Real>(m_nImagePxWidth) /
            static_cast<Real>(m_nImagePxHeight);
         m_fImageMtHeight =
            2.0 * m_fFocalLength * Tan(m_cAperture) /
            Sqrt(1.0 + fAspectRatio * fAspectRatio);
         m_fImageMtWidth = m_fImageMtHeight * fAspectRatio;
      }

      /**
       * Returns the image width in meters.
       * @return The image width in meters.
       */
      inline Real GetImageMtWidth() const {
         return m_fImageMtWidth;
      }

      /**
       * Returns the image height in meters.
       * @return The image height in meters.
       */
      inline Real GetImageMtHeight() const {
         return m_fImageMtHeight;
      }

      virtual std::string GetTypeDescription() const {
         return "perspective_camera";
      }

   private:

      /** The aperture of the visibility cone */
      CRadians m_cAperture;

      /** The focal length of the camera */
      Real m_fFocalLength;

      /** The camera range */
      Real m_fRange;

      /** Image width in pixel */
      SInt32 m_nImagePxWidth;

      /** Image height in pixel */
      SInt32 m_nImagePxHeight;

      /** Image width in meters */
      Real m_fImageMtWidth;

      /** Image height in meters */
      Real m_fImageMtHeight;

      /** The anchor to which the camera is attached */
      SAnchor* m_psAnchor;

   };
}

#endif
