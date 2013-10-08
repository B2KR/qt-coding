#ifndef VERION_H
#define VERION_H

#include "QLogger.h"

using namespace QLogger;

#define VERSION_VS "0.1"

#ifdef Q_CC_MSVC
    #ifndef and
        #define and &&
    #endif

    #ifndef or
        #define or ||
    #endif
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <qmath.h>
#else
#include <QtMath>
#endif

#endif // VERION_H
