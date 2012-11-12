/**
 * @file core/simulator/loop_functions.h
 *
 * @brief This file provides the definition of the loop functions.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef LOOP_FUNCTIONS_H
#define LOOP_FUNCTIONS_H

namespace argos {
   class CLoopFunctions;
   class CFloorEntity;
}

#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>

namespace argos {

   class CLoopFunctions : public CBaseConfigurableResource {

   protected:

      CSimulator& m_cSimulator;
      CSpace& m_cSpace;

   public:

      struct SAdditionalLED {
         CVector3 Position;
         CColor Color;

         SAdditionalLED() {}
         
         SAdditionalLED(const CVector3& c_position,
                        const CColor& c_color) :
            Position(c_position),
            Color(c_color) {}
      };

   public:

      CLoopFunctions() :
         m_cSimulator(CSimulator::GetInstance()),
         m_cSpace(m_cSimulator.GetSpace()) {
      }
      virtual ~CLoopFunctions() {}

      inline virtual void Init(TConfigurationNode& t_tree) {}
      inline virtual void Reset() {}
      inline virtual void Destroy() {}

      inline virtual void PreStep() {}
      inline virtual void PostStep() {}

      inline virtual bool IsExperimentFinished() {
         return false;
      }

      inline virtual CColor GetFloorColor(const CVector2& c_position_on_plane) {
         return CColor::BLACK;
      }

   protected:

      inline virtual bool MoveEntity(CPositionalEntity& c_entity,
                                     const CVector3& c_position,
                                     const CQuaternion& c_orientation,
                                     bool b_check_only) {
         return c_entity.MoveTo(c_position, c_orientation, b_check_only);
      }

      inline virtual void RemoveEntity(const std::string& str_entity_id) {
         CEntity& entity = m_cSpace.GetEntity(str_entity_id);
         RemoveEntity(entity);
      }

      virtual void RemoveEntity(CEntity& c_entity);

   };
}

#endif
