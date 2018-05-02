/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_joint_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PROTOTYPE_JOINT_ENTITY_H
#define PROTOTYPE_JOINT_ENTITY_H

namespace argos {
   class CPrototypeLinkEntity;
   class CPrototypeJointEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CPrototypeJointEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

      using TVector = std::vector<CPrototypeJointEntity*>;
      using TVectorIterator = std::vector<CPrototypeJointEntity*>::iterator;
      using TVectorConstIterator = std::vector<CPrototypeJointEntity*>::const_iterator;

   public:
      enum class EType {
         FIXED,
         PRISMATIC,
         REVOLUTE,
         SPHERICAL,
         PLANAR,
      };

      union ULimit {
         ULimit() {}
         CRange<Real> Prismatic;
         CRange<CRadians> Revolute;
      };

      struct SActuator {
         enum class EMode {
            DISABLED,
            POSITION,
            VELOCITY,
         };
         Real Target = 0.0f;
         Real MaxImpulse = 0.0f;
         EMode Mode = EMode::DISABLED;
      };

      struct SSensor {
         enum class EMode {
            DISABLED,
            POSITION,
            VELOCITY,
         };
         Real Value = 0.0f;
         EMode Mode = EMode::DISABLED;
      };

   public:

      CPrototypeJointEntity(CComposableEntity* pc_parent);

      virtual ~CPrototypeJointEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual std::string GetTypeDescription() const {
         return "joint";
      }

      inline CPrototypeLinkEntity& GetParentLink() {
         return *m_pcParentLink;
      }

      inline const CPrototypeLinkEntity& GetParentLink() const {
         return *m_pcParentLink;
      }

      inline const CVector3& GetParentLinkJointPosition() const {
         return m_cParentLinkJointPosition;
      }

      inline const CQuaternion& GetParentLinkJointOrientation() const {
         return m_cParentLinkJointOrientation;
      }

      inline CPrototypeLinkEntity& GetChildLink() {
         return *m_pcChildLink;
      }

      inline const CPrototypeLinkEntity& GetChildLink() const {
         return *m_pcChildLink;
      }

      inline const CVector3& GetChildLinkJointPosition() const {
         return m_cChildLinkJointPosition;
      }

      inline const CQuaternion& GetChildLinkJointOrientation() const {
         return m_cChildLinkJointOrientation;
      }

      inline bool GetDisableCollision() const {
         return m_bDisableCollision;
      }

      inline EType GetType() const {
         return m_eType;
      }

      inline const CVector3& GetJointAxis() const {
         return m_cJointAxis;
      }

      bool HasLimit() {
         return m_bHasLimit;
      }

      const ULimit& GetLimit() const {
         ARGOS_ASSERT(m_bHasLimit == true,
                   "CPrototypeJointEntity::GetLimit(), id=\"" <<
                   GetId() <<
                   "\": joint was not initialized with a limit");
         return m_uLimit;
      }

      SSensor& GetSensor() {
         return m_cSensor;
      }

      SActuator& GetActuator() {
         return m_cActuator;
      }

   private:

      CPrototypeLinkEntity* m_pcParentLink;
      CVector3 m_cParentLinkJointPosition;
      CQuaternion m_cParentLinkJointOrientation;

      CPrototypeLinkEntity* m_pcChildLink;
      CVector3 m_cChildLinkJointPosition;
      CQuaternion m_cChildLinkJointOrientation;

      bool m_bDisableCollision;
      EType m_eType;
      CVector3 m_cJointAxis;
      bool m_bHasLimit;
      ULimit m_uLimit;
      SSensor m_cSensor;
      SActuator m_cActuator;
   };

}

#endif
