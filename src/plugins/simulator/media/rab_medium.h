#ifndef RAB_MEDIUM_H
#define RAB_MEDIUM_H

#include <argos3/core/simulator/medium/medium.h>

namespace argos {

   class CRABMedium : public CMedium {

   public:

      CRABMedium();

      CRABMedium(const std::string& str_id);

      virtual ~CRABMedium();

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Destroy();

      virtual void Update();

   };

}

#endif
