
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
         sRule.Pattern = QRegularExpression(cPattern);
         sRule.Format = m_cKeywordFormat;
         m_vecHighlightingRules.append(sRule);
      }

      m_cQuotationFormat.setForeground(Qt::darkGreen);
      sRule.Pattern = QRegularExpression("\".*\"");
      sRule.Format = m_cQuotationFormat;
      m_vecHighlightingRules.append(sRule);

      m_cSingleLineCommentFormat.setForeground(Qt::darkGray);
      m_cSingleLineCommentFormat.setFontItalic(true);
      sRule.Pattern = QRegularExpression("--[^[\n]*");
      sRule.Format = m_cSingleLineCommentFormat;
      m_vecHighlightingRules.append(sRule);

      m_cMultiLineCommentFormat.setForeground(Qt::darkGray);
      m_cMultiLineCommentFormat.setFontItalic(true);
      m_cCommentStartExpression = QRegularExpression("--\\[\\[");
      m_cCommentEndExpression = QRegularExpression("\\]\\]");      
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaSyntaxHighlighter::highlightBlock(const QString& str_text) {
      /*
       * Apply normal rules
       */
      foreach (const SHighlightingRule& sRule, m_vecHighlightingRules) {
         QRegularExpression cExpression(sRule.Pattern);
         QRegularExpressionMatchIterator cMatchIt = cExpression.globalMatch(str_text);
         while(cMatchIt.hasNext()) {
            QRegularExpressionMatch cMatch = cMatchIt.next();
            setFormat(cMatch.capturedStart(), cMatch.capturedLength(), sRule.Format);
         }
      }
      /*
       * Apply multi-line comment rules
       */
      setCurrentBlockState(NOT_MULTILINE_COMMENT);
      int nStartIndex = 0;
      if (previousBlockState() != MULTILINE_COMMENT) {
         nStartIndex = str_text.indexOf(m_cCommentStartExpression);
      }
      while(nStartIndex >= 0) {
         QRegularExpressionMatch cEndMatch;
         int nEndIndex = str_text.indexOf(m_cCommentEndExpression, nStartIndex, &cEndMatch);
         int nCommentLength;
         if (nEndIndex == -1) {
            setCurrentBlockState(MULTILINE_COMMENT);
            nCommentLength = str_text.length() - nStartIndex;
         } else {
            nCommentLength = nEndIndex - nStartIndex + cEndMatch.capturedLength();
         }
         setFormat(nStartIndex, nCommentLength, m_cMultiLineCommentFormat);
         nStartIndex = str_text.indexOf(m_cCommentStartExpression, nStartIndex + nCommentLength);
      }
   }

   /****************************************/
   /****************************************/

}
