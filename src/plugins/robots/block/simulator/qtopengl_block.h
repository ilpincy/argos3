/**
 * @file <argos3/plugins/robots/block/simulator/qtopengl_block.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef QTOPENGL_BLOCK_H
#define QTOPENGL_BLOCK_H

namespace argos {
   class CQTOpenGLBlock;
   class CBlockEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_obj_model.h>

namespace argos {

   /****************************************/
   /****************************************/

   class CQTOpenGLBlock {

   public:

      CQTOpenGLBlock();

      virtual ~CQTOpenGLBlock();

      virtual void Draw(CBlockEntity& c_entity);

   private:
      /* OBJ model */
      CQTOpenGLObjModel m_cBlockModel;
      /* Pointer to the LED material within the OBJ model */
      std::array<CQTOpenGLObjModel::SMaterial*, 24> m_arrLEDs;
      /* The tag texture */
      const std::array<std::array<GLfloat, 3>, 100> m_arrTagTexture;
      /* Precompiled list for drawing tags */
      GLuint m_unTagList;
   };

   /****************************************/
   /****************************************/

}

#endif
