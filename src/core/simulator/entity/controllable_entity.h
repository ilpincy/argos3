/**
 * @file <argos3/core/simulator/entity/controllable_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CONTROLLABLE_ENTITY_H
#define CONTROLLABLE_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CActuator;
   class CSensor;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/math/ray.h>

namespace argos {

   class CControllableEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      typedef std::vector<CControllableEntity*> TVector;
      typedef std::map<std::string, CControllableEntity*> TMap;

   public:

      CControllableEntity(CComposableEntity* pc_parent) :
         CEntity(pc_parent),
         m_pcController(NULL) {}

      virtual ~CControllableEntity();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual void Update() {}

      inline const std::string& GetControllerId() const {
         return m_strControllerId;
      }

      inline CCI_Controller& GetController() {
         return *m_pcController;
      }
      void SetController(CCI_Controller& pc_controller);

      virtual void Sense();

      inline void ControlStep() {
         if(m_pcController != NULL) {
            m_pcController->ControlStep();
         }
         else {
            THROW_ARGOSEXCEPTION("Entity " << GetId() << " does not have any controller associated. Did you add it to the <arena_physics> section?");
         }
      }

      virtual void Act();

      virtual std::string GetTypeDescription() const {
         return "controllable_entity";
      }

      inline void AddCheckedRay(bool b_obstructed,
                                const CRay& c_ray) {
         m_vecCheckedRays.push_back(std::make_pair(b_obstructed, c_ray));
      }

      inline void AddIntersectionPoint(const CRay& c_ray,
                                       Real f_t_on_ray) {
         CVector3 cPoint;
         c_ray.GetPoint(cPoint, f_t_on_ray);
         m_vecIntersectionPoints.push_back(cPoint);
      }

      inline std::vector<std::pair<bool, CRay> >& GetCheckedRays() {
         return m_vecCheckedRays;
      }

      inline std::vector<CVector3>& GetIntersectionPoints() {
         return m_vecIntersectionPoints;
      }

   protected:

      std::string m_strControllerId;
      CCI_Controller* m_pcController;

      std::map<std::string, CActuator*> m_mapActuators;
      std::map<std::string, CSensor*> m_mapSensors;

      std::vector<std::pair<bool, CRay> > m_vecCheckedRays;
      std::vector<CVector3> m_vecIntersectionPoints;

   };

}

#endif
