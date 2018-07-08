/**
 * @file <argos3/plugins/simulator/entities/battery_equipped_entity.h>
 *
 * @author Adhavan Jayabalan <jadhu94@gmail.com>
 */

#ifndef BATTERY_EQUIPPED_ENTITY_H
#define BATTERY_EQUIPPED_ENTITY_H

namespace argos {
   class CBatteryEquippedEntity;
   class CBatteryDischargeModel;
}

#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <map>

namespace argos {

   /****************************************/
   /****************************************/
   
   /**
    * The battery entity.
    *
    * This entity is used to model the fact that robots have
    * batteries. This entity allows the user to specify the initial
    * level of the battery, its maximum charge, and the discharge
    * model. A few discharge models are offered by default.
    */
   class CBatteryEquippedEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      CBatteryEquippedEntity(CComposableEntity* pc_parent);

      CBatteryEquippedEntity(CComposableEntity *pc_parent,
                             const std::string& str_id,
                             CBatteryDischargeModel* pc_discharge_model = NULL,
                             Real f_start_charge = 1.0,
                             Real f_full_charge = 1.0);

      CBatteryEquippedEntity(CComposableEntity *pc_parent,
                             const std::string& str_id,
                             const std::string& str_discharge_model,
                             Real f_start_charge = 1.0,
                             Real f_full_charge = 1.0);

      virtual ~CBatteryEquippedEntity();

      virtual void Init(TConfigurationNode& t_tree);

      virtual std::string GetTypeDescription() const {
         return "battery";
      }

      virtual void Update();

      Real GetFullCharge() const {
         return m_fFullCharge;
      }

      void SetFullCharge(Real f_full_charge) {
         m_fFullCharge = f_full_charge;
      }

      Real GetAvailableCharge() const {
         return m_fAvailableCharge;
      }

      void SetAvailableCharge(Real f_available_charge) {
         m_fAvailableCharge = f_available_charge;
      }

      void SetDischargeModel(CBatteryDischargeModel* pc_model);

      void SetDischargeModel(const std::string& str_model);

   protected:

      /** Full charge */
      Real m_fFullCharge;
      /** Available charge */
      Real m_fAvailableCharge;
      /** Discharge model */
      CBatteryDischargeModel* m_pcDischargeModel;
   };

   /****************************************/
   /****************************************/
   
   /**
    * The discharge model dictates how the battery discharges over
    * time.
    */
   class CBatteryDischargeModel : public CBaseConfigurableResource {
         
   public:
         
      CBatteryDischargeModel();

      virtual ~CBatteryDischargeModel();

      virtual void Init(TConfigurationNode& t_tree) {}

      virtual void Reset() {}

      virtual void Destroy() {}

      virtual void SetBattery(CBatteryEquippedEntity* pc_battery);
         
      virtual void operator()() = 0;
         
   protected:
         
      CBatteryEquippedEntity* m_pcBattery;
   };

   /****************************************/
   /****************************************/
   
   /**
    * For dynamic loading of battery discharge models.
    */
   typedef CFactory<CBatteryDischargeModel> TFactoryBatteryDischargeModel;
   
#define REGISTER_BATTERY_DISCHARGE_MODEL(CLASSNAME, LABEL)  \
   REGISTER_SYMBOL(CBatteryDischargeModel,                  \
                   CLASSNAME,                               \
                   LABEL,                                   \
                   "undefined",                             \
                   "undefined",                             \
                   "undefined",                             \
                   "undefined",                             \
                   "undefined")

   /****************************************/
   /****************************************/

   /**
    * A battery discharge model based only on time.
    *
    * In this model, the charge is calculated as follows:
    *
    * new charge = old charge - delta
    */
   class CBatteryDischargeModelTime : public CBatteryDischargeModel {
      
   public:

      CBatteryDischargeModelTime() :
         m_fDelta(1e-5) {}

      virtual void Init(TConfigurationNode& t_tree);
      
      void SetDelta(Real f_delta) {
         m_fDelta = f_delta;
      }
      
      virtual void operator()();
      
   protected:
      
      const SAnchor* m_psAnchor;
      CVector3 m_cOldPosition;
      Real m_fDelta;
   };

   /****************************************/
   /****************************************/

   /**
    * A battery discharge model based only on motion.
    *
    * In this model, the charge is calculated as follows:
    *
    * new charge = old charge - pos_factor * (delta position) - orient_factor * (delta orientation)
    */
   class CBatteryDischargeModelMotion : public CBatteryDischargeModel {
      
   public:
      
      CBatteryDischargeModelMotion() :
         m_psAnchor(NULL),
         m_fPosFactor(1e-3),
         m_fOrientFactor(1e-3) {}

      virtual void Init(TConfigurationNode& t_tree);

      void SetPosFactor(Real f_factor) {
         m_fPosFactor = f_factor;
      }
      
      void SetOrientFactor(Real f_factor) {
         m_fOrientFactor = f_factor;
      }
      
      virtual void SetBattery(CBatteryEquippedEntity* pc_battery);
      
      virtual void operator()();
      
   protected:
      
      const SAnchor* m_psAnchor;
      CVector3 m_cOldPosition;
      CQuaternion m_cOldOrientation;
      Real m_fPosFactor;
      Real m_fOrientFactor;
   };

   /****************************************/
   /****************************************/
   
   /**
    * A battery discharge model in which the charge decreases with both time and motion.
    *
    * In this model, the charge is calculated as follows:
    *
    * new charge = old charge - delta - pos_factor * (delta position) - orient_factor * (delta orientation)
    */
   class CBatteryDischargeModelTimeMotion : public CBatteryDischargeModel {
      
   public:
      
      CBatteryDischargeModelTimeMotion() :
         m_psAnchor(NULL),
         m_fDelta(1e-5),
         m_fPosFactor(1e-3),
         m_fOrientFactor(1e-3) {}

      virtual void Init(TConfigurationNode& t_tree);
      
      void SetDelta(Real f_delta) {
         m_fDelta = f_delta;
      }
      
      void SetPosFactor(Real f_factor) {
         m_fPosFactor = f_factor;
      }
      
      void SetOrientFactor(Real f_factor) {
         m_fOrientFactor = f_factor;
      }
      
      virtual void SetBattery(CBatteryEquippedEntity* pc_battery);
      
      virtual void operator()();
      
   protected:
      
      const SAnchor* m_psAnchor;
      CVector3 m_cOldPosition;
      CQuaternion m_cOldOrientation;
      Real m_fDelta;
      Real m_fPosFactor;
      Real m_fOrientFactor;
   };

   /****************************************/
   /****************************************/
   
}

#endif
