
/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_syntax_highlighter.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef QTOPENGL_LUA_SYNTAX_HIGHLIGHTER_H
#define QTOPENGL_LUA_SYNTAX_HIGHLIGHTER_H

namespace argos {
   class CQTOpenGLLuaSyntaxHighlighter;
}

#include <QHash>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QTextDocument;

namespace argos {

   class CQTOpenGLLuaSyntaxHighlighter : public QSyntaxHighlighter {

      Q_OBJECT

   public:

      CQTOpenGLLuaSyntaxHighlighter(QTextDocument* pc_text);
      virtual ~CQTOpenGLLuaSyntaxHighlighter() {}

   protected:

      void highlightBlock(const QString& str_text);

   private:
      
      struct SHighlightingRule
      {
         QRegularExpression Pattern;
         QTextCharFormat Format;
      };
      QVector<SHighlightingRule> m_vecHighlightingRules;

      QRegularExpression m_cCommentStartExpression;
      QRegularExpression m_cCommentEndExpression;

      QTextCharFormat m_cKeywordFormat;
      QTextCharFormat m_cSingleLineCommentFormat;
      QTextCharFormat m_cMultiLineCommentFormat;
      QTextCharFormat m_cQuotationFormat;
      QTextCharFormat m_cFunctionFormat;
   };

}

#endif
