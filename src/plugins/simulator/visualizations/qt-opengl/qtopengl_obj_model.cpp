/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_obj_model.cpp>
 *
 * @author Majd Kassawat - <majd.kassawat@gmail.com>
 * @author Michael Allwright - <allsey87@gmail.com>
 *
 */

#include "qtopengl_obj_model.h"

#include <argos3/core/simulator/visualization/visualization.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_render.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_main_window.h>

#include <QFile>
#include <QTextStream>

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLObjModel::CQTOpenGLObjModel(const std::string& str_obj_file) {
      /* create a default material */
      m_mapMaterials["__default_material"];
      /* load the model */
      QFile cGeometryFile(GetModelDir() + QString::fromStdString(str_obj_file));
      if(!cGeometryFile.open(QIODevice::ReadOnly)) {
         THROW_ARGOSEXCEPTION("Error loading model \"" <<
                              cGeometryFile.fileName().toStdString() << "\": " <<
                              cGeometryFile.errorString().toStdString());
      }
      QTextStream cGeometryStream(&cGeometryFile);
      ImportGeometry(cGeometryStream);
      /* build the meshes */
      BuildMeshes();
      /* check if normals have been provided */
      if (m_vecNormals.empty()) {
         THROW_ARGOSEXCEPTION("Error loading model \"" << 
                              cGeometryFile.fileName().toStdString() <<
                              "\": model does not specify normals.")
      }
      /* generate the OpenGL vectors */
      GenerateOpenGLVectors();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLObjModel::SMaterial& CQTOpenGLObjModel::GetMaterial(const std::string& str_material) {
      SMaterial::TMapIterator tIter = m_mapMaterials.find(str_material);
      if(tIter == std::end(m_mapMaterials)) {
         THROW_ARGOSEXCEPTION("Material \"" << str_material << "\" is undefined");
      }
      return tIter->second;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLObjModel::Draw() const {
      for(const CQTOpenGLObjModel::SMesh& s_mesh : m_vecMeshes) {
         /* set material properties for the mesh */
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, s_mesh.Material->second.Ambient.data());
         glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, s_mesh.Material->second.Diffuse.data());
         glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, s_mesh.Material->second.Emission.data());
         glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, s_mesh.Material->second.Shininess.data());
         glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s_mesh.Material->second.Specular.data());
         /* enable blending for transparent materials */
         if(s_mesh.Material->second.EnableTransparency) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         }
         /* enable client states, set pointers */
         if (!m_vecOpenGLPositions.empty()) {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, m_vecOpenGLPositions.data());
         }
         if (!m_vecOpenGLNormals.empty()) {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, sizeof(GLfloat) * 3, m_vecOpenGLNormals.data());
         }
         /* draw the triangles */
         glDrawElements(GL_TRIANGLES, s_mesh.Triangles.size(), GL_UNSIGNED_INT, s_mesh.Triangles.data());
         /* disable client states */
         if (!m_vecOpenGLNormals.empty()) {
            glDisableClientState(GL_NORMAL_ARRAY);
         }
         if (!m_vecOpenGLPositions.empty()) {
            glDisableClientState(GL_VERTEX_ARRAY);
         }
         /* disable blending for transparent materials */
         if(s_mesh.Material->second.EnableTransparency) {
            glDisable(GL_BLEND);
         }
      }
   }

   /****************************************/
   /****************************************/

   const QString& CQTOpenGLObjModel::GetModelDir() const {
      /* get a reference to the visualization */
      CVisualization& cVisualization = CSimulator::GetInstance().GetVisualization();
      CQTOpenGLRender& cQtOpenGLRender = dynamic_cast<CQTOpenGLRender&>(cVisualization);
      /* return the model directory */
      return cQtOpenGLRender.GetMainWindow().GetModelDir();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLObjModel::ImportGeometry(QTextStream& c_text_stream) {
      /* select the default material */
      SMaterial::TMapIterator itSelectedMaterial =
         m_mapMaterials.find("__default_material");
      /* define a functor for adjusting negative indices found in the obj model */
      struct {
         void operator()(SInt32& n_index, SInt32 un_count) {
            n_index = (n_index < 0) ? (n_index + un_count) : (n_index - 1);
         }
      } AdjustIndex;
      /* create a buffer for read the file */
      QString strLineBuffer;
      /* loop over the lines from the text stream */
      while(c_text_stream.readLineInto(&strLineBuffer)) {
         const QStringList& cLineBufferSplit =
            strLineBuffer.split(QRegExp("\\s+"), QString::SkipEmptyParts);
         if(cLineBufferSplit.empty()) {
            /* skip blank lines */
            continue;
         }
         else if(cLineBufferSplit[0] == "mtllib") {
            QFile cMaterialsFile(GetModelDir() + cLineBufferSplit.value(1));
            cMaterialsFile.open(QIODevice::ReadOnly);
            QTextStream cMaterialsStream(&cMaterialsFile);
            ImportMaterials(cMaterialsStream);
         }
         else if(cLineBufferSplit[0] == "f") {
            /* detect the coordinate format by splitting on the '/' character */
            const QStringList& cCoordinateParts = cLineBufferSplit.value(1).split('/');
            /* f v v v ... */
            if(cCoordinateParts.count() == 1) {
               /* v0 */
               SInt32 nVertex0 = cLineBufferSplit.value(1).toInt();
               AdjustIndex(nVertex0, m_vecVertexCoords.size());
               /* v1 */
               SInt32 nVertex1 = cLineBufferSplit.value(2).toInt();
               AdjustIndex(nVertex1, m_vecVertexCoords.size());
               /* v2 ... vn */
               for(SInt32 n_index = 3; n_index < cLineBufferSplit.count(); n_index++) {
                  SInt32 nVertex2 = cLineBufferSplit.value(n_index).toInt();
                  AdjustIndex(nVertex2, m_vecVertexCoords.size());
                  /* add triangle */
                  m_vecTriangles.emplace_back(itSelectedMaterial, std::array<GLuint, 3> {
                     AddVertex(nVertex0, m_vecVertexCoords[nVertex0], CVector3::ZERO, CVector2::ZERO),
                     AddVertex(nVertex1, m_vecVertexCoords[nVertex1], CVector3::ZERO, CVector2::ZERO),
                     AddVertex(nVertex2, m_vecVertexCoords[nVertex2], CVector3::ZERO, CVector2::ZERO),
                  });
                  nVertex1 = nVertex2;
               }
            }
            /* f v/vt v/vt v/vt ... */
            else if(cCoordinateParts.count() == 2) {
               /* v0/vt0 */
               const QStringList& cVertex0 = cLineBufferSplit.value(1).split('/');
               SInt32 nVertex0 = cVertex0.value(0).toInt();
               AdjustIndex(nVertex0, m_vecVertexCoords.size());
               SInt32 nTexture0 = cVertex0.value(1).toInt();
               AdjustIndex(nTexture0, m_vecTextureCoords.size());
               /* v1/vt1 */
               const QStringList& cVertex1 = cLineBufferSplit.value(2).split('/');
               SInt32 nVertex1 = cVertex1.value(0).toInt();
               AdjustIndex(nVertex1, m_vecVertexCoords.size());
               SInt32 nTexture1 = cVertex1.value(1).toInt();
               AdjustIndex(nTexture1, m_vecTextureCoords.size());
               /* v2/vt2 ... vn/vtn */
               for(SInt32 n_index = 3; n_index < cLineBufferSplit.count(); n_index++) {
                  const QStringList& cVertex2 = cLineBufferSplit.value(n_index).split('/');
                  SInt32 nVertex2 = cVertex2.value(0).toInt();
                  AdjustIndex(nVertex2, m_vecVertexCoords.size());
                  SInt32 nTexture2 = cVertex2.value(1).toInt();
                  AdjustIndex(nTexture2, m_vecTextureCoords.size());
                  /* add triangle */
                  m_vecTriangles.emplace_back(itSelectedMaterial, std::array<GLuint, 3> {
                     AddVertex(nVertex0, m_vecVertexCoords[nVertex0], CVector3::ZERO, m_vecTextureCoords[nTexture0]),
                     AddVertex(nVertex1, m_vecVertexCoords[nVertex1], CVector3::ZERO, m_vecTextureCoords[nTexture1]),
                     AddVertex(nVertex2, m_vecVertexCoords[nVertex2], CVector3::ZERO, m_vecTextureCoords[nTexture2]),
                  });
                  /* prepare for next face */
                  nVertex1 = nVertex2;
                  nTexture1 = nTexture2;
               }
            }
            /* f v//vn v//vn v//vn ... */
            else if(cCoordinateParts.count() == 3 && cCoordinateParts[1].isEmpty()) {
               /* v0//vn0 */
               const QStringList& cVertex0 = cLineBufferSplit.value(1).split('/');
               SInt32 nVertex0 = cVertex0.value(0).toInt();
               AdjustIndex(nVertex0, m_vecVertexCoords.size());
               SInt32 nNormal0 = cVertex0.value(2).toInt();
               AdjustIndex(nNormal0, m_vecNormals.size());
               /* v1//vn1 */
               const QStringList& cVertex1 = cLineBufferSplit.value(2).split('/');
               SInt32 nVertex1 = cVertex1.value(0).toInt();
               AdjustIndex(nVertex1, m_vecVertexCoords.size());
               SInt32 nNormal1 = cVertex1.value(2).toInt();
               AdjustIndex(nNormal1, m_vecNormals.size());
               /* v2//vn2 ... vn//vnn */
               for(SInt32 n_index = 3; n_index < cLineBufferSplit.count(); n_index++) {
                  const QStringList& cVertex2 = cLineBufferSplit.value(n_index).split('/');
                  SInt32 nVertex2 = cVertex2.value(0).toInt();
                  AdjustIndex(nVertex2, m_vecVertexCoords.size());
                  SInt32 nNormal2 = cVertex2.value(2).toInt();
                  AdjustIndex(nNormal2, m_vecNormals.size());
                  /* add triangle */
                  m_vecTriangles.emplace_back(itSelectedMaterial, std::array<GLuint, 3> {
                     AddVertex(nVertex0, m_vecVertexCoords[nVertex0], m_vecNormals[nNormal0], CVector2::ZERO),
                     AddVertex(nVertex1, m_vecVertexCoords[nVertex1], m_vecNormals[nNormal1], CVector2::ZERO),
                     AddVertex(nVertex2, m_vecVertexCoords[nVertex2], m_vecNormals[nNormal2], CVector2::ZERO),
                  });
                  /* prepare for next face */
                  nVertex1 = nVertex2;
                  nNormal1 = nNormal2;
               }
            }
            /* f v/vt/vn v/vt/vn v/vt/vn ... */
            else if(cCoordinateParts.count() == 3) {
               /* v0/vt0/vn0 */
               const QStringList& cVertex0 = cLineBufferSplit.value(1).split('/');
               SInt32 nVertex0 = cVertex0.value(0).toInt();
               AdjustIndex(nVertex0, m_vecVertexCoords.size());
               SInt32 nTexture0 = cVertex0.value(1).toInt();
               AdjustIndex(nTexture0, m_vecTextureCoords.size());
               SInt32 nNormal0 = cVertex0.value(2).toInt();
               AdjustIndex(nNormal0, m_vecNormals.size());
               /* v1/vt1/vn1 */
               const QStringList& cVertex1 = cLineBufferSplit.value(2).split('/');
               SInt32 nVertex1 = cVertex1.value(0).toInt();
               AdjustIndex(nVertex1, m_vecVertexCoords.size());
               SInt32 nTexture1 = cVertex1.value(1).toInt();
               AdjustIndex(nTexture1, m_vecTextureCoords.size());
               SInt32 nNormal1 = cVertex1.value(2).toInt();
               AdjustIndex(nNormal1, m_vecNormals.size());
               /* v2/vt2/vn2 ... vn/vtn/vnn */
               for(SInt32 n_index = 3; n_index < cLineBufferSplit.count(); n_index++) {
                  const QStringList& cVertex2 = cLineBufferSplit.value(n_index).split('/');
                  SInt32 nVertex2 = cVertex2.value(0).toInt();
                  AdjustIndex(nVertex2, m_vecVertexCoords.size());
                  SInt32 nTexture2 = cVertex2.value(1).toInt();
                  AdjustIndex(nTexture2, m_vecTextureCoords.size());
                  SInt32 nNormal2 = cVertex2.value(2).toInt();
                  AdjustIndex(nNormal2, m_vecNormals.size());
                  /* add triangle */
                  m_vecTriangles.emplace_back(itSelectedMaterial, std::array<GLuint, 3> {
                     AddVertex(nVertex0, m_vecVertexCoords[nVertex0], m_vecNormals[nNormal0], m_vecTextureCoords[nTexture0]),
                     AddVertex(nVertex1, m_vecVertexCoords[nVertex1], m_vecNormals[nNormal1], m_vecTextureCoords[nTexture1]),
                     AddVertex(nVertex2, m_vecVertexCoords[nVertex2], m_vecNormals[nNormal2], m_vecTextureCoords[nTexture2]),
                  });
                  /* prepare for next face */
                  nVertex1 = nVertex2;
                  nNormal1 = nNormal2;
                  nTexture1 = nTexture2;
               }
            }
         }
         else if(cLineBufferSplit[0] == "usemtl") {
            std::string strName = cLineBufferSplit.value(1).toStdString();
            itSelectedMaterial = m_mapMaterials.find(strName);
            /* select the default material if the requested material is missing */
            if(itSelectedMaterial == std::end(m_mapMaterials)) {
               itSelectedMaterial = m_mapMaterials.find("__default_material");
            }
         }
         else if(cLineBufferSplit[0] == "v") {
            m_vecVertexCoords.emplace_back(cLineBufferSplit.value(1).toFloat(),
                                           cLineBufferSplit.value(2).toFloat(),
                                           cLineBufferSplit.value(3).toFloat());
         }
         else if(cLineBufferSplit[0] == "vn") {
            m_vecNormals.emplace_back(cLineBufferSplit.value(1).toFloat(),
                                      cLineBufferSplit.value(2).toFloat(),
                                      cLineBufferSplit.value(3).toFloat());
         }
         else if(cLineBufferSplit[0] == "vt") {
            m_vecTextureCoords.emplace_back(cLineBufferSplit.value(1).toFloat(),
                                            cLineBufferSplit.value(2).toFloat());
         }
      }
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLObjModel::ImportMaterials(QTextStream& c_text_stream) {
      /* select the default material */
      SMaterial::TMapIterator itMaterial =
         m_mapMaterials.find("__default_material");
      /* create a buffer for parsing the input */
      QString strLineBuffer;
      /* load the materials in the MTL file */
      while(c_text_stream.readLineInto(&strLineBuffer)) {
         const QStringList& cLineBufferSplit =
            strLineBuffer.split(QRegExp("\\s+"), QString::SkipEmptyParts);
         if(cLineBufferSplit.empty()) {
            /* skip blank lines */
            continue;
         }
         else if(cLineBufferSplit[0] == "newmtl") {
            const QString& strMaterialName = cLineBufferSplit.value(1);
            itMaterial = m_mapMaterials.find(strMaterialName.toStdString());
            if(itMaterial != std::end(m_mapMaterials)) {
               THROW_ARGOSEXCEPTION("Material \"" << strMaterialName.toStdString() <<
                                    "\" already defined");
            }
            itMaterial = m_mapMaterials.emplace(strMaterialName.toStdString(), SMaterial()).first;
         }
         else if(cLineBufferSplit[0] == "Ka") {
            itMaterial->second.Ambient[0] = cLineBufferSplit.value(1).toFloat();
            itMaterial->second.Ambient[1] = cLineBufferSplit.value(2).toFloat();
            itMaterial->second.Ambient[2] = cLineBufferSplit.value(3).toFloat();
         }
         else if(cLineBufferSplit[0] == "Kd") {
            itMaterial->second.Diffuse[0] = cLineBufferSplit.value(1).toFloat();
            itMaterial->second.Diffuse[1] = cLineBufferSplit.value(2).toFloat();
            itMaterial->second.Diffuse[2] = cLineBufferSplit.value(3).toFloat();
         }
         else if(cLineBufferSplit[0] == "Ks") {
            itMaterial->second.Specular[0] = cLineBufferSplit.value(1).toFloat();
            itMaterial->second.Specular[1] = cLineBufferSplit.value(2).toFloat();
            itMaterial->second.Specular[2] = cLineBufferSplit.value(3).toFloat();
         }
         else if(cLineBufferSplit[0] == "Tr") {
            GLfloat fAlpha = 1.0f - cLineBufferSplit.value(1).toFloat();
            if(fAlpha < 1.0f) {
               itMaterial->second.EnableTransparency = true;
               itMaterial->second.Ambient[3] = fAlpha;
               itMaterial->second.Diffuse[3] = fAlpha;
               itMaterial->second.Specular[3] = fAlpha;
            }
            itMaterial->second.Alpha = fAlpha;
         }
         else if(cLineBufferSplit[0] == "d") {
            GLfloat fAlpha = cLineBufferSplit.value(1).toFloat();
            if(fAlpha < 1.0f) {
               itMaterial->second.EnableTransparency = true;
               itMaterial->second.Ambient[3] = fAlpha;
               itMaterial->second.Diffuse[3] = fAlpha;
               itMaterial->second.Specular[3] = fAlpha;
            }
            itMaterial->second.Alpha = fAlpha;
         }
         else if(cLineBufferSplit[0] == "i" && cLineBufferSplit.value(1) == "1") {
            itMaterial->second.Specular = {
               0.0f, 0.0f, 0.0f, 1.0f
            };
         }
      }
   }

   /****************************************/
   /****************************************/

   GLuint CQTOpenGLObjModel::AddVertex(SInt32 n_key,
                                       const CVector3& c_position,
                                       const CVector3& c_normal,
                                       const CVector2& c_texture) {
      /* for readability */
      typedef std::multimap<SInt32, GLuint>::iterator TCacheIterator;
      typedef std::multimap<SInt32, GLuint>::value_type TCacheEntry;
      /* find cache entries matching the key */
      std::pair<TCacheIterator, TCacheIterator> cRange =
         m_mapVertexCache.equal_range(n_key);
      /* check if the vertex already exists in the cache */
      TCacheIterator itVertex =
         std::find_if(cRange.first,
                      cRange.second,
                      [this, &c_position, &c_normal, &c_texture] (const TCacheEntry& c_entry) {
            return (m_vecVertices[c_entry.second].Position == c_position &&
                    m_vecVertices[c_entry.second].Normal == c_normal &&
                    m_vecVertices[c_entry.second].Texture == c_texture);
      });
      /* if the vertex doesn't exist create it and put it in the cache */
      if(itVertex == cRange.second) {
         m_vecVertices.emplace_back(c_position, c_normal, c_texture);
         itVertex = m_mapVertexCache.emplace(n_key, m_vecVertices.size() - 1);
      }
      return itVertex->second;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLObjModel::BuildMeshes() {
      SMaterial::TMapIterator itCurrentMaterial = std::end(m_mapMaterials);
      for(const STriangle& s_triangle : m_vecTriangles) {
         if(s_triangle.Material != itCurrentMaterial) {
            /* select the current material */
            itCurrentMaterial = s_triangle.Material;
            /* create a new mesh */
            m_vecMeshes.emplace_back(itCurrentMaterial);
         }
         m_vecMeshes.back().Triangles.push_back(s_triangle.Indices[0]);
         m_vecMeshes.back().Triangles.push_back(s_triangle.Indices[1]);
         m_vecMeshes.back().Triangles.push_back(s_triangle.Indices[2]);
      }
      /* sort the meshes based on their material's alpha. Fully opaque meshes towards the
         front and fully transparent towards the back. */
      std::sort(m_vecMeshes.begin(),
                m_vecMeshes.end(),
                [] (const SMesh& s_left,
                    const SMesh& s_right) {
         return (s_left.Material->second.Alpha > s_right.Material->second.Alpha);
      });
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLObjModel::GenerateOpenGLVectors() {
      /* reserve memory */
      m_vecOpenGLPositions.reserve(m_vecVertices.size() * 3);
      m_vecOpenGLNormals.reserve(m_vecVertices.size() * 3);
      /* copy data */
      for(const SVertex& s_vertex : m_vecVertices) {
         /* positions */
         m_vecOpenGLPositions.push_back(s_vertex.Position.GetX());
         m_vecOpenGLPositions.push_back(s_vertex.Position.GetY());
         m_vecOpenGLPositions.push_back(s_vertex.Position.GetZ());
         /* normals */
         m_vecOpenGLNormals.push_back(s_vertex.Normal.GetX());
         m_vecOpenGLNormals.push_back(s_vertex.Normal.GetY());
         m_vecOpenGLNormals.push_back(s_vertex.Normal.GetZ());
      }
   }

   /****************************************/
   /****************************************/


}
