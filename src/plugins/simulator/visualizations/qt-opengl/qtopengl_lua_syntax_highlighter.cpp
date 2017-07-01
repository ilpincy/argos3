
/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_syntax_highlighter.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "qtopengl_lua_syntax_highlighter.h"

namespace argos {

   /****************************************/
   /****************************************/

   static int NOT_MULTILINE_COMMENT = 0;
   static int MULTILINE_COMMENT = 1;

   /****************************************/
   /****************************************/

   CQTOpenGLLuaSyntaxHighlighter::CQTOpenGLLuaSyntaxHighlighter(QTextDocument* pc_text) :
      QSyntaxHighlighter(pc_text) {
      SHighlightingRule sRule;
      m_cKeywordFormat.setForeground(Qt::darkBlue);
      m_cKeywordFormat.setFontWeight(QFont::Bold);
      QStringList cKeywordPatterns;
      cKeywordPatterns << "\\band\\b"    << "\\bbreak\\b"  << "\\bdo\\b"   << "\\belse\\b"     << "\\belseif\\b"
                       << "\\bend\\b"    << "\\bfalse\\b"  << "\\bfor\\b"  << "\\bfunction\\b" << "\\bif\\b"
                       << "\\bin\\b"     << "\\blocal\\b"  << "\\bnil\\b"  << "\\bnot\\b"      << "\\bor\\b"
                       << "\\brepeat\\b" << "\\breturn\\b" << "\\bthen\\b" << "\\btrue\\b"     << "\\buntil\\b" << "\\bwhile\\b";
      foreach (const QString& cPattern, cKeywordPatterns) {
         sRule.Pattern = QRegExp(cPattern);
         sRule.Format = m_cKeywordFormat;
         m_vecHighlightingRules.append(sRule);
      }

      m_cQuotationFormat.setForeground(Qt::darkGreen);
      sRule.Pattern = QRegExp("\".*\"");
      sRule.Format = m_cQuotationFormat;
      m_vecHighlightingRules.append(sRule);

      m_cSingleLineCommentFormat.setForeground(Qt::darkGray);
      m_cSingleLineCommentFormat.setFontItalic(true);
      sRule.Pattern = QRegExp("--[^[\n]*");
      sRule.Format = m_cSingleLineCommentFormat;
      m_vecHighlightingRules.append(sRule);

      m_cMultiLineCommentFormat.setForeground(Qt::darkGray);
      m_cMultiLineCommentFormat.setFontItalic(true);
      m_cCommentStartExpression = QRegExp("--\\[\\[");
      m_cCommentEndExpression = QRegExp("\\]\\]");      
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaSyntaxHighlighter::highlightBlock(const QString& str_text) {
      /*
       * Apply normal rules
       */
      foreach (const SHighlightingRule& sRule, m_vecHighlightingRules) {
         QRegExp cExpression(sRule.Pattern);
         int i = cExpression.indexIn(str_text);
         while(i >= 0) {
            int nLength = cExpression.matchedLength();
            setFormat(i, nLength, sRule.Format);
            i = cExpression.indexIn(str_text, i + nLength);
         }
      }
      /*
       * Apply multi-line comment rules
       */
      setCurrentBlockState(NOT_MULTILINE_COMMENT);
      int nStartIndex = 0;
      if (previousBlockState() != MULTILINE_COMMENT) {
         nStartIndex = m_cCommentStartExpression.indexIn(str_text);
      }
      while(nStartIndex >= 0) {
         int nEndIndex = m_cCommentEndExpression.indexIn(str_text, nStartIndex);
         int nCommentLength;
         if (nEndIndex == -1) {
            setCurrentBlockState(MULTILINE_COMMENT);
            nCommentLength = str_text.length() - nStartIndex;
         } else {
            nCommentLength = nEndIndex - nStartIndex
               + m_cCommentEndExpression.matchedLength();
         }
         setFormat(nStartIndex, nCommentLength, m_cMultiLineCommentFormat);
         nStartIndex = m_cCommentStartExpression.indexIn(str_text, nStartIndex + nCommentLength);
      }
   }

   /****************************************/
   /****************************************/

}
