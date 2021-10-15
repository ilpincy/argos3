/**
 * @file <argos3/plugins/robots/block/simulator/dynamics3d_block_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_BLOCK_MODEL_H
#define DYNAMICS3D_BLOCK_MODEL_H

namespace argos {
   class CDynamics3DBlockModel;
   class CBlockEntity;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>

namespace argos {

   class CDynamics3DBlockModel : public CDynamics3DSingleBodyObjectModel {

   public:     

      CDynamics3DBlockModel(CDynamics3DEngine& c_engine,
                            CBlockEntity& c_block);

      virtual ~CDynamics3DBlockModel() {}

      std::shared_ptr<CBody>& GetBody() {
         return m_ptrBody;
      }

   private:
      std::shared_ptr<CBody> m_ptrBody;

      static const Real m_fBlockSideLength;
      static const Real m_fBlockMass;
   };
}

#endif
