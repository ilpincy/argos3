/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_srocs_plugin.cpp>
 *
 * @author Weixu Zhu - <zhuweixu_harry@126.com>
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_srocs_plugin.h"

#include <argos3/plugins/robots/block/simulator/dynamics3d_block_model.h>
#include <argos3/plugins/robots/builderbot/simulator/dynamics3d_builderbot_model.h>
#include <algorithm>
#include <vector>

#define ELECTROMAGNET_FORCE_COEFFICIENT 2.5
#define ELECTROMAGNET_RANGE_COEFFICIENT 0.015
#define ELECTROMAGNET_RANGE_MAX 0.02

#define MAGNET_FORCE_COEFFICIENT 5.0
#define MAGNET_DISTANCE_CONTACT 0.01
#define MAGNET_DISTANCE_MAX 0.03

namespace argos {
 
   /****************************************/
   /****************************************/

   void CDynamics3DSRoCSPlugin::Init(TConfigurationNode& t_tree) {
      GetNodeAttributeOrDefault(t_tree,
                                "disable_block_magnets_during_manipulation",
                                m_bDisableBlockMagnetsDuringManipulation,
                                m_bDisableBlockMagnetsDuringManipulation);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSRoCSPlugin::RegisterModel(CDynamics3DModel& c_model) {
      /* If the model is a block, then add its body to m_vecBlocks */
      CDynamics3DBlockModel* pcBlockModel = dynamic_cast<CDynamics3DBlockModel*>(&c_model);
      if(pcBlockModel != nullptr) {
         m_vecBlocks.emplace_back(pcBlockModel->GetBody());
      }
      /* If the model is a BuilderBot, then add its end effector to m_vecEndEffectors */
      CDynamics3DBuilderBotModel* pcBuilderBotModel = dynamic_cast<CDynamics3DBuilderBotModel*>(&c_model);
      if(pcBuilderBotModel != nullptr) {
         m_vecEndEffectors.emplace_back(pcBuilderBotModel->GetEndEffectorLink());
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSRoCSPlugin::UnregisterModel(CDynamics3DModel& c_model) {
      /* If the model is a block, then remove its body from m_vecBlocks */
      CDynamics3DBlockModel* pcBlockModel = dynamic_cast<CDynamics3DBlockModel*>(&c_model);
      if(pcBlockModel != nullptr) {
         std::vector<SBlock>::iterator itBlockToErase =
            std::find_if(std::begin(m_vecBlocks),
                         std::end(m_vecBlocks),
                         [pcBlockModel] (const SBlock& s_block) {
                            return (pcBlockModel->GetBody() == s_block.Body);
                         });
         if(itBlockToErase != std::end(m_vecBlocks)) {
            m_vecBlocks.erase(itBlockToErase);
         }
      }
      /* If the model is a BuilderBot, then remove its end effector from m_vecEndEffectors */
      CDynamics3DBuilderBotModel* pcBuilderBotModel = dynamic_cast<CDynamics3DBuilderBotModel*>(&c_model);
      if(pcBuilderBotModel != nullptr) {
         std::vector<SEndEffector>::iterator itEndEffectorToErase =
            std::find_if(std::begin(m_vecEndEffectors),
                         std::end(m_vecEndEffectors),
                         [pcBuilderBotModel] (const SEndEffector& s_end_effector) {
                            return (pcBuilderBotModel->GetEndEffectorLink() == s_end_effector.Body);
                         });
         if(itEndEffectorToErase != std::end(m_vecEndEffectors)) {
            m_vecEndEffectors.erase(itEndEffectorToErase);
         }
      }
   }
   
   /****************************************/
   /****************************************/

   void CDynamics3DSRoCSPlugin::Update() {
      /* calculate the magnet offsets and positions for each block */
      for(SBlock& s_block : m_vecBlocks) {
         s_block.InteractingWithEndEffector = false;
         /* for each of the eight magnets, calculate rotated offsets and positions */
         for(UInt32 un_index = 0; un_index < 8; un_index++) {
            s_block.Magnets[un_index].RotatedOffset = 
               quatRotate(s_block.Body->GetTransform().getRotation(), m_arrMagnetOffsets[un_index]);
            s_block.Magnets[un_index].Position = 
               s_block.Magnets[un_index].RotatedOffset + s_block.Body->GetTransform().getOrigin();
         }
      }
      /* calculate the electromagnet offsets for each end effector */
      for(SEndEffector& s_end_effector : m_vecEndEffectors) {
         /* for each of the four electromagnets */
         for(UInt32 un_index = 0; un_index < 4; un_index++) {
            s_end_effector.Electromagnets[un_index].Position = 
               s_end_effector.Body->GetTransform().getOrigin();
            s_end_effector.Electromagnets[un_index].Position +=
               quatRotate(s_end_effector.Body->GetTransform().getRotation(), m_arrElectromagnetOffsets[un_index]);
         }
      }
      /* force between end effectors and blocks */
      for(SBlock& s_block : m_vecBlocks) {
         for(SEndEffector& s_end_effector : m_vecEndEffectors) {
            /* get the field */
            Real fElectromagnetField = s_end_effector.Body->GetData().Dipoles[0].GetField().getY();
            /* calculate the force */
            btScalar fForce = ELECTROMAGNET_FORCE_COEFFICIENT * s_block.Body->GetData().Mass;
            /* calculate the range */
            Real fRange = fElectromagnetField * ELECTROMAGNET_RANGE_COEFFICIENT;
            if(fRange > ELECTROMAGNET_RANGE_MAX)
               fRange = ELECTROMAGNET_RANGE_MAX;
            /* apply force only if there are four pairs of magnets in range of each other */
            UInt32 unInRangeCount = 0;
            for(const SBlock::SMagnet& s_magnet : s_block.Magnets) {
               for(const SEndEffector::SElectromagnet& s_electromagnet : s_end_effector.Electromagnets) {
                  if(s_magnet.Position.distance(s_electromagnet.Position) < fRange) {
                     unInRangeCount++;
                  }
               }
            }
            if(unInRangeCount < 4) {
               /* not enough points in range */
               continue;
            }
            else {
               /* apply force on the block */
               for(const SBlock::SMagnet& s_magnet : s_block.Magnets) {
                  for(const SEndEffector::SElectromagnet& s_electromagnet : s_end_effector.Electromagnets) {
                     btScalar fDistance = s_magnet.Position.distance(s_electromagnet.Position);
                     btVector3 cDirectionVector = (s_electromagnet.Position - s_magnet.Position).normalize();
                     if(fDistance < fRange) {
                        /* force and torque for box */
                        s_block.Body->ApplyForce(cDirectionVector * fForce,
                                                 s_magnet.RotatedOffset);
                        s_block.InteractingWithEndEffector = true;
                     }
                  }
               }
            }
         }
      }
      /* force between blocks */
      if(m_vecBlocks.size() < 2) {
         /* Nothing to do for blocks */
         return;
      }
      /* for each pair of blocks */
      for(std::vector<SBlock>::iterator itBlock0 = std::begin(m_vecBlocks);
          itBlock0 != (std::end(m_vecBlocks) - 1);
          ++itBlock0) {
         if(m_bDisableBlockMagnetsDuringManipulation &&
            itBlock0->InteractingWithEndEffector) {
            continue;
         }
         for(std::vector<SBlock>::iterator itBlock1 = std::next(itBlock0, 1);
             itBlock1 != std::end(m_vecBlocks);
             ++itBlock1) {
            if(m_bDisableBlockMagnetsDuringManipulation &&
               itBlock1->InteractingWithEndEffector) {
               continue;
            }
            /* if a box is not movable, its mass is zero by default, 
             * so if one is zero, use the mass of the other one */
            btScalar fForce = MAGNET_FORCE_COEFFICIENT * itBlock0->Body->GetData().Mass;
            if(fForce == 0)
               fForce = MAGNET_FORCE_COEFFICIENT * itBlock1->Body->GetData().Mass;
            /* apply force only if there are four pairs in range */
            UInt32 unInRangeCount = 0;
            for(const SBlock::SMagnet& s_magnet : itBlock0->Magnets) {
               const btVector3& cMagnetPosition0 = s_magnet.Position;
               for(const SBlock::SMagnet& s_magnet : itBlock1->Magnets) {
                  const btVector3& cMagnetPosition1 = s_magnet.Position;
                  if(cMagnetPosition0.distance(cMagnetPosition1) < MAGNET_DISTANCE_MAX) {
                     unInRangeCount++;
                  }
               }
            }
            if(unInRangeCount < 4) {
               continue;
            }
            /* apply force on both blocks */
            for(const SBlock::SMagnet& s_magnet_0 : itBlock0->Magnets) {
               const btVector3& cMagnetPosition0 = s_magnet_0.Position;
               for(const SBlock::SMagnet& s_magnet_1 : itBlock1->Magnets) {
                  const btVector3& cMagnetPosition1 = s_magnet_1.Position;
                  /* distance and relative vector */
                  btScalar fDistance = cMagnetPosition0.distance(cMagnetPosition1);
                  btVector3 cDirectionVector01 = (cMagnetPosition1 - cMagnetPosition0).normalize();
                  btVector3 cDirectionVector10 = -cDirectionVector01;
                  /* apply force with respect to distance */
                  if(fDistance < MAGNET_DISTANCE_MAX) {
                     if(fDistance > MAGNET_DISTANCE_CONTACT) {
                        fForce *= (MAGNET_DISTANCE_MAX - fDistance) /
                                  (MAGNET_DISTANCE_MAX - MAGNET_DISTANCE_CONTACT);
                     }
                     /* force and torque for magnet 0 */
                     itBlock0->Body->ApplyForce(cDirectionVector01 * fForce,
                                                s_magnet_0.RotatedOffset);
                     /* force and torque for magnet 1 */
                     itBlock1->Body->ApplyForce(cDirectionVector10 * fForce,
                                                s_magnet_1.RotatedOffset);
                  }
               }
            }
         }
      }
   }
   
   /****************************************/
   /****************************************/

   const std::array<btVector3, 8> CDynamics3DSRoCSPlugin::m_arrMagnetOffsets {
      btVector3( 0.023, -0.023, -0.023),  btVector3(-0.023, -0.023, -0.023),
      btVector3(-0.023, -0.023,  0.023),  btVector3( 0.023, -0.023,  0.023),
      btVector3( 0.023,  0.023, -0.023),  btVector3(-0.023,  0.023, -0.023),
      btVector3(-0.023,  0.023,  0.023),  btVector3( 0.023,  0.023,  0.023),
   };

   /****************************************/
   /****************************************/

   const std::array<btVector3, 4> CDynamics3DSRoCSPlugin::m_arrElectromagnetOffsets {
      btVector3( 0.0199375, -0.004,  0.023),
      btVector3(-0.0260625, -0.004,  0.023),
      btVector3( 0.0199375, -0.004, -0.023),
      btVector3(-0.0260625, -0.004, -0.023)
   };

   /****************************************/
   /****************************************/

   REGISTER_DYNAMICS3D_PLUGIN(CDynamics3DSRoCSPlugin,
                              "srocs",
                              "Weixu Zhu [zhuweixu_harry@126.com]",
                              "1.0",
                              "Applies a magnetic forces between Stigmergic Blocks and BuilderBot robots",
                              "For a description on how to use this plugin, please consult the documentation\n"
                              "for the dynamics3d physics engine plugin",
                              "Usable");

   /****************************************/
   /****************************************/
}
