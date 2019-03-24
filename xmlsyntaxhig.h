#ifndef XMLSYNTAXHIG_H
#define XMLSYNTAXHIG_H

#include <QSyntaxHighlighter>
class XMLSyntaxHig : public QSyntaxHighlighter
{

public:
    XMLSyntaxHig(QTextDocument *parent = nullptr, QString p_keyWords = nullptr, QColor color = nullptr);
    void highlightBlock(const QString &text);
    void ChangeRegy(QString ptr, QColor p_color = nullptr);

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
     };
    QString regy;
    QColor color;
private:
    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat keywordFormat;
};

#endif // XMLSYNTAXHIG_H
