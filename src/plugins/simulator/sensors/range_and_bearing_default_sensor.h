/**
 * @file <argos3/plugins/simulator/sensors/range_and_bearing_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef RANGE_AND_BEARING_DEFAULT_SENSOR_H
#define RANGE_AND_BEARING_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CRangeAndBearingDefaultSensor;
   class CControllableEntity;
}

#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/rab_equipped_entity.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>

namespace argos {

   class CRangeAndBearingDefaultSensor : public CSimulatedSensor,
                                         public CCI_RangeAndBearingSensor {

   public:

      CRangeAndBearingDefaultSensor();
      virtual ~CRangeAndBearingDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Update();
      virtual void Reset();

   private:

      CRABEquippedEntity*  m_pcRangeAndBearingEquippedEntity;
      CEmbodiedEntity*     m_pcEmbodiedEntity;
      CControllableEntity* m_pcControllableEntity;
      Real                 m_fDistanceNoiseStdDev;
      CRandom::CRNG*       m_pcRNG;
      bool                 m_bShowRays;
      TEmbodiedEntitySet   m_tIgnoreMe;

      CSpace&                            m_cSpace;
      CPositionalIndex<CEmbodiedEntity>& m_cEmbodiedEntityIndex;
      CSpaceHash<CRABEquippedEntity, CRABEquippedEntitySpaceHashUpdater>& m_cRABEquippedSpaceHash;

      SInt32 m_nBBoxMinI, m_nBBoxMinJ, m_nBBoxMinK;
      SInt32 m_nBBoxMaxI, m_nBBoxMaxJ, m_nBBoxMaxK;

   };

}

#endif
