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

      ENABLE_VTABLE();

   public:

      CWheeledEntity(CComposableEntity* pc_parent) :
         CEntity(pc_parent) {
         ::memset(m_cWheelSpeed, 0, NUM_WHEELS * sizeof(Real));
      }

      CWheeledEntity(CComposableEntity* pc_parent,
                     const std::string& str_id) :
         CEntity(pc_parent, str_id) {
         ::memset(m_cWheelSpeed, 0, NUM_WHEELS * sizeof(Real));
      }

      virtual void Reset() {
         ::memset(m_cWheelSpeed, 0, NUM_WHEELS * sizeof(Real));
      }

      void GetSpeed(Real* f_speed) {
         ::memcpy(f_speed, m_cWheelSpeed, NUM_WHEELS * sizeof(Real));
      }

      void SetSpeed(Real f_speed[NUM_WHEELS]) {
         ::memcpy(m_cWheelSpeed, f_speed, NUM_WHEELS * sizeof(Real));
      }

      const CVector3& GetWheelPosition(UInt32 un_index) const {
         return m_cWheelPosition[un_index];
      }

      void SetWheelPosition(UInt32 un_index,
                            const CVector3& c_position) {
         m_cWheelPosition[un_index] = c_position;
      }

      virtual std::string GetTypeDescription() const {
         return "wheeled_entity<" + ToString(NUM_WHEELS) + ">";
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

      ENABLE_VTABLE();

   public:

      CWheeledEntity(CComposableEntity* pc_parent) :
         CEntity(pc_parent) {
         m_cWheelSpeed[0] = 0.0f;
         m_cWheelSpeed[1] = 0.0f;
      }

      CWheeledEntity(CComposableEntity* pc_parent,
                     const std::string& str_id) :
         CEntity(pc_parent, str_id) {
         m_cWheelSpeed[0] = 0.0f;
         m_cWheelSpeed[1] = 0.0f;
      }

      virtual void Reset() {
         m_cWheelSpeed[0] = 0.0f;
         m_cWheelSpeed[1] = 0.0f;
      }

      void GetSpeed(Real* f_speed) {
         f_speed[0] = m_cWheelSpeed[0];
         f_speed[1] = m_cWheelSpeed[1];
      }

      void SetSpeed(Real f_speed[2]) {
         m_cWheelSpeed[0] = f_speed[0];
         m_cWheelSpeed[1] = f_speed[1];
      }

      const CVector3& GetWheelPosition(UInt32 un_index) const {
         return m_cWheelPosition[un_index];
      }

      void SetWheelPosition(UInt32 un_index,
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
