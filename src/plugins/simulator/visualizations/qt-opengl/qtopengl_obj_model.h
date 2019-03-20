/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_obj_body.h>
 *
 * @author Majd Kassawat - <majd.kassawat@gmail.com>
 * @author Michael Allwright - <allsey87@gmail.com>
 *
 * This class was inspired by the OpenGL OBJ Viewer Demo from dhpoware. Details
 * can be found on the following page: http://www.dhpoware.com/demos/glObjViewer.html
 */

#ifndef QTOPENGL_OBJ_MODEL_H
#define QTOPENGL_OBJ_MODEL_H

class QTextStream;
class QString;

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/vector2.h>

#include <algorithm>
#include <array>
#include <map>
#include <string>
#include <vector>

#ifdef __APPLE__
#include <glu.h>
#else
#include <GL/glu.h>
#endif

namespace argos {

   class CQTOpenGLObjModel {

   public:

      struct SMaterial {
         /* OpenGL material arrays */
         std::array<GLfloat, 4> Ambient = {{0.0f, 0.0f, 0.0f, 1.0f}};
         std::array<GLfloat, 4> Diffuse = {{0.0f, 0.0f, 0.0f, 1.0f}};
         std::array<GLfloat, 4> Emission = {{0.0f, 0.0f, 0.0f, 1.0f}};
         std::array<GLfloat, 4> Specular = {{0.0f, 0.0f, 0.0f, 1.0f}};
         std::array<GLfloat, 1> Shininess = {{0.0f}};
         /* material transparency */
         GLfloat Alpha = 1.0f;
         /* enable the transparency effect for this material? */
         bool EnableTransparency = false;
         /* convenience typedef for an iterator over a std::map of SMaterial */
         typedef std::map<std::string, SMaterial>::iterator TMapIterator;
      };

   public:

      CQTOpenGLObjModel(const std::string& str_model);

      SMaterial& GetMaterial(const std::string& str_material);

      void Draw() const;

   private:

      struct SVertex {
         SVertex(const CVector3& c_position = CVector3::ZERO,
                 const CVector3& c_normal = CVector3::ZERO,
                 const CVector2& c_texture = CVector2::ZERO) :
            Position(c_position),
            Normal(c_normal),
            Texture(c_texture) {}

         CVector3 Position;
         CVector3 Normal;
         CVector2 Texture;
      };

      struct STriangle {
         STriangle(const SMaterial::TMapIterator& it_material,
                   const std::array<GLuint, 3>& arr_indices) :
            Material(it_material),
            Indices(arr_indices) {}

         SMaterial::TMapIterator Material;
         std::array<GLuint, 3> Indices;
      };

      struct SMesh {
         SMesh(SMaterial::TMapIterator t_material) :
            Material(t_material) {}

         std::vector<GLuint> Triangles;
         SMaterial::TMapIterator Material;
      };

   private:

      const QString& GetModelDir() const;

      void ImportGeometry(QTextStream& c_text_stream);

      void ImportMaterials(QTextStream& c_text_stream);

      GLuint AddVertex(SInt32 n_key,
                       const CVector3& c_position,
                       const CVector3& c_normal,
                       const CVector2& c_texture);

      void BuildMeshes();

      void GenerateOpenGLVectors();

   private:
      /* parsed data */
      std::vector<CVector3> m_vecVertexCoords;
      std::vector<CVector2> m_vecTextureCoords;
      std::vector<CVector3> m_vecNormals;
      /* high-level primitives */
      std::map<std::string, SMaterial> m_mapMaterials;
      std::vector<SVertex> m_vecVertices;
      std::vector<STriangle> m_vecTriangles;
      std::vector<SMesh> m_vecMeshes;
      std::multimap<SInt32, GLuint> m_mapVertexCache;
      /* low-level primitives */
      std::vector<GLfloat> m_vecOpenGLPositions;
      std::vector<GLfloat> m_vecOpenGLNormals;
   };

}

#endif

