/**
 * @file <argos3/core/simulator/entity/wheeled_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef WHEELED_ENTITY_H
#define WHEELED_ENTITY_H

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   template <SInt32 NUM_WHEELS> class CWheeledEntity : public CEntity {

   public:

      CWheeledEntity(CEntity* pc_parent) :
         CEntity(pc_parent) {
         ::bzero(m_cWheelSpeed, NUM_WHEELS * sizeof(Real));
      }
      virtual ~CWheeledEntity() {}

      virtual void Reset() {
         /* Reset speeds */
         ::bzero(m_cWheelSpeed, NUM_WHEELS * sizeof(Real));
      }

      virtual void Update() {}

      virtual void GetSpeed(Real* f_speed) {
         ::memcpy(f_speed, m_cWheelSpeed, NUM_WHEELS * sizeof(Real));
      }

      virtual void SetSpeed(Real f_speed[NUM_WHEELS]) {
         ::memcpy(m_cWheelSpeed, f_speed, NUM_WHEELS * sizeof(Real));
      }

      virtual const CVector3& GetWheelPosition(UInt32 un_index) const {
         return m_cWheelPosition[un_index];
      }

      virtual void SetWheelPosition(UInt32 un_index,
                                           const CVector3& c_position) {
         m_cWheelPosition[un_index] = c_position;
      }

      virtual std::string GetTypeDescription() const {
         return "wheeled_entity<" + ToString(NUM_WHEELS) +">";
      }

   private:

      CVector3 m_cWheelPosition[NUM_WHEELS];
      Real m_cWheelSpeed[NUM_WHEELS];

   };

   /****************************************/
   /****************************************/

   /*
    * This specialization is to optimize the case of robots with two wheels
    * (e.g., foot-bot, s-bot and e-puck)
    */
   template <> class CWheeledEntity<2> : public CEntity {

   public:

      CWheeledEntity(CEntity* pc_parent) :
         CEntity(pc_parent) {
         m_cWheelSpeed[0] = 0.0f;
         m_cWheelSpeed[1] = 0.0f;
      }
      virtual ~CWheeledEntity() {}

      virtual void Reset() {
         /* Reset speeds */
         m_cWheelSpeed[0] = 0.0f;
         m_cWheelSpeed[1] = 0.0f;
      }

      virtual void Update() {}

      virtual void GetSpeed(Real* f_speed) {
         f_speed[0] = m_cWheelSpeed[0];
         f_speed[1] = m_cWheelSpeed[1];
      }

      virtual void SetSpeed(Real f_speed[2]) {
         m_cWheelSpeed[0] = f_speed[0];
         m_cWheelSpeed[1] = f_speed[1];
      }

      virtual const CVector3& GetWheelPosition(UInt32 un_index) const {
         return m_cWheelPosition[un_index];
      }

      virtual void SetWheelPosition(UInt32 un_index,
                                           const CVector3& c_position) {
         m_cWheelPosition[un_index] = c_position;
      }

      virtual std::string GetTypeDescription() const {
         return "wheeled_entity<2>";
      }

   private:

      CVector3 m_cWheelPosition[2];
      Real m_cWheelSpeed[2];

   };

}

#endif
