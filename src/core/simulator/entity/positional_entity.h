/**
 * @file <argos3/core/simulator/entity/positional_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef POSITIONAL_ENTITY_H
#define POSITIONAL_ENTITY_H

namespace argos {
   class CPositionalEntity;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CPositionalEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      CPositionalEntity(CComposableEntity* pc_parent);

      CPositionalEntity(CComposableEntity* pc_parent,
                        const std::string& str_id,
                        const CVector3& c_position = CVector3(),
                        const CQuaternion& c_orientation = CQuaternion());

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();

      inline const CVector3& GetPosition() const {
         return m_cPosition;
      }

      inline const CVector3& GetInitPosition() const {
         return m_cInitPosition;
      }

      inline void SetPosition(const CVector3& c_position) {
         m_cPosition = c_position;
      }

      inline void SetInitPosition(const CVector3& c_position) {
         m_cInitPosition = c_position;
      }

      inline const CQuaternion& GetOrientation() const {
         return m_cOrientation;
      }

      inline const CQuaternion& GetInitOrientation() const {
         return m_cInitOrientation;
      }

      inline void SetOrientation(const CQuaternion c_orientation) {
         m_cOrientation = c_orientation;
      }

      inline void SetInitOrientation(const CQuaternion c_orientation) {
         m_cInitOrientation = c_orientation;
      }

      virtual void MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation);

      virtual std::string GetTypeDescription() const {
         return "position";
      }

   private:
      
      CVector3 m_cPosition;
      CVector3 m_cInitPosition;
      CQuaternion m_cOrientation;
      CQuaternion m_cInitOrientation;
   };
}

#endif
