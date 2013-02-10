/**
 * @file <argos3/core/simulator/entity/controllable_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CONTROLLABLE_ENTITY_H
#define CONTROLLABLE_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CSimulatedActuator;
   class CSimulatedSensor;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/math/ray3.h>

namespace argos {

   class CControllableEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      typedef std::vector<CControllableEntity*> TVector;
      typedef std::map<std::string, CControllableEntity*> TMap;

   public:

      CControllableEntity(CComposableEntity* pc_parent);

      CControllableEntity(CComposableEntity* pc_parent,
                          const std::string& str_id,
                          const std::string& str_controller_id);

      CControllableEntity(CComposableEntity* pc_parent,
                          const std::string& str_id,
                          const std::string& str_controller_id,
                          TConfigurationNode& t_controller_config);

      virtual ~CControllableEntity();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      inline CCI_Controller& GetController() {
         return *m_pcController;
      }

      /**
       * Creates and assigns a controller with the given id.
       */
      void SetController(const std::string& str_controller_id);

      /**
       * Creates and assigns a controller with the given id and the given XML parameters.
       */
      void SetController(const std::string& str_controller_id,
                         TConfigurationNode& t_parameters);

      virtual void Sense();

      void ControlStep();

      virtual void Act();

      virtual std::string GetTypeDescription() const {
         return "controller";
      }

      inline void AddCheckedRay(bool b_obstructed,
                                const CRay3& c_ray) {
         m_vecCheckedRays.push_back(std::make_pair(b_obstructed, c_ray));
      }

      inline void AddIntersectionPoint(const CRay3& c_ray,
                                       Real f_t_on_ray) {
         CVector3 cPoint;
         c_ray.GetPoint(cPoint, f_t_on_ray);
         m_vecIntersectionPoints.push_back(cPoint);
      }

      inline std::vector<std::pair<bool, CRay3> >& GetCheckedRays() {
         return m_vecCheckedRays;
      }

      inline std::vector<CVector3>& GetIntersectionPoints() {
         return m_vecIntersectionPoints;
      }

   protected:

      CCI_Controller* m_pcController;

      std::map<std::string, CSimulatedActuator*> m_mapActuators;
      std::map<std::string, CSimulatedSensor*> m_mapSensors;

      std::vector<std::pair<bool, CRay3> > m_vecCheckedRays;
      std::vector<CVector3> m_vecIntersectionPoints;

   };

}

#endif
