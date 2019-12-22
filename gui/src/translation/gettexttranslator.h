#ifndef GETTEXTTRANSLATOR_H
#define GETTEXTTRANSLATOR_H

#include <QTranslator>

class GettextTranslator: public QTranslator
{
public:
    GettextTranslator();

public:
    QString translate(const char *context,
                      const char *sourceText,
                      const char *disambiguation = nullptr, int n = -1) const override;
};

#endif // GETTEXTTRANSLATOR_H
