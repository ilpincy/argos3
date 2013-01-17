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

      CPositionalEntity(CEntity* pc_parent) :
         CEntity(pc_parent) {}
      virtual ~CPositionalEntity() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();

      inline virtual const CVector3& GetPosition() const {
         return m_cPosition;
      }

      inline virtual const CVector3& GetOffsetPosition() const {
         return m_cOffsetPosition;
      }

      inline virtual const CVector3& GetInitPosition() const {
         return m_cInitPosition;
      }

      inline virtual const CVector3& GetInitOffsetPosition() const {
         return m_cInitOffsetPosition;
      }

      inline virtual void SetPosition(const CVector3& c_position) {
         m_cPosition = c_position;
      }

      inline virtual void SetOffsetPosition(const CVector3& c_position) {
         m_cOffsetPosition = c_position;
      }

      inline virtual void SetInitPosition(const CVector3& c_position) {
         m_cInitPosition = c_position;
      }

      inline virtual void SetInitOffsetPosition(const CVector3& c_position) {
         m_cInitOffsetPosition = c_position;
      }

      inline virtual const CQuaternion& GetOrientation() const {
         return m_cOrientation;
      }

      inline virtual const CQuaternion& GetOffsetOrientation() const {
         return m_cOffsetOrientation;
      }

      inline virtual const CQuaternion& GetInitOrientation() const {
         return m_cInitOrientation;
      }

      inline virtual const CQuaternion& GetInitOffsetOrientation() const {
         return m_cInitOffsetOrientation;
      }

      inline virtual void SetOrientation(const CQuaternion c_orientation) {
         m_cOrientation = c_orientation;
      }

      inline virtual void SetOffsetOrientation(const CQuaternion c_orientation) {
         m_cOffsetOrientation = c_orientation;
      }

      inline virtual void SetInitOrientation(const CQuaternion c_orientation) {
         m_cInitOrientation = c_orientation;
      }

      inline virtual void SetInitOffsetOrientation(const CQuaternion c_orientation) {
         m_cInitOffsetOrientation = c_orientation;
      }

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      inline virtual std::string GetTypeDescription() const {
         return "positional_entity";
      }

   private:
      
      CVector3 m_cPosition;
      CQuaternion m_cOrientation;

      CVector3 m_cOffsetPosition;
      CQuaternion m_cOffsetOrientation;

      CVector3 m_cInitPosition;
      CQuaternion m_cInitOrientation;

      CVector3 m_cInitOffsetPosition;
      CQuaternion m_cInitOffsetOrientation;

   };
}

#endif
