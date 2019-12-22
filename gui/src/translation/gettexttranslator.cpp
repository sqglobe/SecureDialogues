#include "gettexttranslator.h"
#include <libintl.h>

GettextTranslator::GettextTranslator()
{

}

QString GettextTranslator::translate(const char */*context*/,
                                     const char *sourceText,
                                     const char */*disambiguation*/, int /*n*/) const
{
    return gettext(sourceText);
}
