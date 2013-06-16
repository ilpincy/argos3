/**
 * @file <argos3/plugins/simulator/entities/omnidirectional_camera_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef OMNIDIRECTIONAL_CAMERA_EQUIPPED_ENTITY_H
#define OMNIDIRECTIONAL_CAMERA_EQUIPPED_ENTITY_H

namespace argos {
   class COmnidirectionalCameraEquippedEntity;
   class CEmbodiedEntity;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class COmnidirectionalCameraEquippedEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      /**
       * Class constructor.
       * This constructor is meant to be used with the Init() method.
       * @param pc_parent The parent of this entity.
       */
      COmnidirectionalCameraEquippedEntity(CComposableEntity* pc_parent);

      /**
       * Class constructor.
       * This constructor is meant to be standalone.
       * You should not call Init() after using this constructor, or
       * memory leaks are likely to happen.
       * @param pc_parent The parent of this entity.
       * @param str_id The id of this entity.
       * @param c_aperture The aperture of the visibility cone
       * @param c_offset The positional offset of this omnidirectionalcamera with respect to the robot reference point.
       */
      COmnidirectionalCameraEquippedEntity(CComposableEntity* pc_parent,
                                           const std::string& str_id,
                                           const CRadians& c_aperture,
                                           const CVector3& c_offset);

      /**
       * Initializes the state of the entity from the XML configuration tree.
       * @throws CARGoSException if a parse error occurs
       */
      virtual void Init(TConfigurationNode& t_tree);

      /**
       * Returns the offset of the omnidirectional camera with respect to the reference point.
       * @return The offset of the omnidirectional camera with respect to the reference point.
       */
      inline const CVector3& GetOffset() const {
         return m_cOffset;
      }

      /**
       * Sets the offset of the omnidirectionalcamera with respect to the reference point.
       * @param c_offset The offset of the omnidirectionalcamera with respect to the reference point.
       */
      inline void SetOffset(const CVector3& c_offset) {
         m_cOffset = c_offset;
      }

      /**
       * Returns the aperture of the visibility cone of the omnidirectional camera.
       * @return The aperture of the visibility cone of the omnidirectional camera.
       */
      inline const CRadians& GetAperture() const {
         return m_cAperture;
      }

      /**
       * Sets the aperture of the visibility cone of the omnidirectional camera.
       * @param c_aperture The aperture of the visibility cone of the omnidirectional camera.
       */
      inline void SetAperture(const CRadians& c_aperture) {
         m_cAperture = c_aperture;
      }

      virtual std::string GetTypeDescription() const {
         return "omnidirectional_camera";
      }

   private:

      /** The aperture of the visibility cone */
      CRadians m_cAperture;

      /** The positional offset of this omnidirectionalcamera with respect to the robot reference point */
      CVector3 m_cOffset;

   };
}

#endif
