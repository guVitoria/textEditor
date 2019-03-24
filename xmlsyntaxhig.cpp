#include "xmlsyntaxhig.h"
#include <QStringList>
#include <QColor>

XMLSyntaxHig::XMLSyntaxHig(QTextDocument *parent, QString p_keyWords, QColor p_color) : QSyntaxHighlighter(parent)
  , regy(p_keyWords), color(p_color)
{
    QStringList listOfKeyWords = regy.split(" ");

    HighlightingRule rule;
    if(color == nullptr){
        color = Qt::darkCyan;
    }
    keywordFormat.setForeground(color);
    keywordFormat.setFontWeight(QFont::Bold);
    //QStringList keywordPatterns;

    foreach (const QString &pattern, listOfKeyWords){
        rule.pattern = QRegExp("\\b" + pattern + "\\b");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }


}

void XMLSyntaxHig::ChangeRegy(QString ptr, QColor p_color){
    regy = ptr;
    QStringList listOfKeyWords = regy.split(" ");

    HighlightingRule rule;

    if(color == nullptr){
        color = Qt::darkCyan;
    } else{
        color = p_color;
    }

    keywordFormat.setForeground(color);
    keywordFormat.setFontWeight(QFont::Bold);
    //QStringList keywordPatterns;

    foreach (const QString &pattern, listOfKeyWords){
        rule.pattern = QRegExp("\\b" + pattern + "\\b");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
}

void XMLSyntaxHig::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);
}
