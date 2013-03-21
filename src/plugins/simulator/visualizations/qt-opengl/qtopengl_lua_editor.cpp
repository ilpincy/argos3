
/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_lua_editor.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "qtopengl_lua_editor.h"
#include "qtopengl_lua_syntax_highlighter.h"

#include <QPainter>
#include <QTextBlock>

namespace argos {

   /****************************************/
   /****************************************/

   CQTOpenGLLuaEditor::CQTOpenGLLuaEditor(QWidget* pc_parent) :
      QPlainTextEdit(pc_parent) {
      /* Set font */
      QFont cFont;
      cFont.setFamily("Monospace");
      cFont.setStyleHint(QFont::Monospace);
      cFont.setFixedPitch(true);
      setFont(cFont);
      /* Set tab width to 3 */
      QFontMetrics cFontMetrics(cFont);
      setTabStopWidth(3 * cFontMetrics.width(' '));
      /* Set syntax highlighting */
      new CQTOpenGLLuaSyntaxHighlighter(document());
      /* Set line numbering */
      m_pcLineNumberArea = new CLineNumberArea(this);
      /* Connect signals */
      connect(this, SIGNAL(blockCountChanged(int)),
              this, SLOT(UpdateLineNumberAreaWidth(int)));
      connect(this, SIGNAL(updateRequest(const QRect&, int)),
              this, SLOT(UpdateLineNumberArea(const QRect&, int)));
      connect(this, SIGNAL(cursorPositionChanged()),
              this, SLOT(HighlightCurrentLine()));
      /* Final touches */
      UpdateLineNumberAreaWidth(0);
      HighlightCurrentLine();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::LineNumberAreaPaintEvent(QPaintEvent* pc_event) {
      QPainter cPainter(m_pcLineNumberArea);
      cPainter.fillRect(pc_event->rect(), Qt::lightGray);


      QTextBlock cBlock = firstVisibleBlock();
      int nBlockNumber = cBlock.blockNumber();
      int nTop = (int) blockBoundingGeometry(cBlock).translated(contentOffset()).top();
      int nBottom = nTop + (int) blockBoundingRect(cBlock).height();

      while (cBlock.isValid() && nTop <= pc_event->rect().bottom()) {
         if (cBlock.isVisible() && nBottom >= pc_event->rect().top()) {
            QString strNumber = QString::number(nBlockNumber + 1);
            cPainter.setPen(Qt::black);
            cPainter.drawText(0, nTop,
                              m_pcLineNumberArea->width(), fontMetrics().height(),
                              Qt::AlignRight, strNumber);
         }

         cBlock = cBlock.next();
         nTop = nBottom;
         nBottom = nTop + (int) blockBoundingRect(cBlock).height();
         ++nBlockNumber;
      }
   }

   /****************************************/
   /****************************************/

   int CQTOpenGLLuaEditor::LineNumberAreaWidth() {
      int nDigits = 1;
      int nMax = qMax(1, blockCount());
      while (nMax >= 10) {
         nMax /= 10;
         ++nDigits;
      }
      int nSpace = 3 + fontMetrics().width(QLatin1Char('9')) * nDigits;
      return nSpace;
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::resizeEvent(QResizeEvent* pc_event) {
      QPlainTextEdit::resizeEvent(pc_event);
      QRect cRect = contentsRect();
      m_pcLineNumberArea->setGeometry(QRect(cRect.left(), cRect.top(),
                                            LineNumberAreaWidth(), cRect.height()));
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::UpdateLineNumberAreaWidth(int) {
      setViewportMargins(LineNumberAreaWidth(), 0, 0, 0);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::HighlightCurrentLine() {
      QList<QTextEdit::ExtraSelection> cListExtraSel;

      if (!isReadOnly()) {
         QTextEdit::ExtraSelection cSel;
         QColor cLineColor = QColor(Qt::yellow).lighter(160);
         cSel.format.setBackground(cLineColor);
         cSel.format.setProperty(QTextFormat::FullWidthSelection, true);
         cSel.cursor = textCursor();
         cSel.cursor.clearSelection();
         cListExtraSel.append(cSel);
      }
      setExtraSelections(cListExtraSel);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLLuaEditor::UpdateLineNumberArea(const QRect& c_rect,
                                                 int n_dy) {
      if(n_dy) {
         m_pcLineNumberArea->scroll(0, n_dy);
      }
      else {
         m_pcLineNumberArea->update(0, c_rect.y(),
                                    m_pcLineNumberArea->width(), c_rect.height());
      }
      if(c_rect.contains(viewport()->rect())) {
         UpdateLineNumberAreaWidth(0);
      }
   }
   
   /****************************************/
   /****************************************/
   
}
