#ifndef DEFINES_H
#define DEFINES_H

#include <stdexcept>

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

#ifndef FILENAME
    #define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

#ifdef _MSC_VER    /// MSVCを使う場合
    #define FUNCNAME __FUNCTION__
#else
    #define FUNCNAME __func__
#endif

#define VAROUT(x) "[" << #x << ":" << x << "],"
#define VARNAME(x) #x
#define CURRENT_DATETIME QDateTime::currentDateTime().toString("[yyyy.MM.dd hh.mm.ss:zzz]").toUtf8().constData()
#ifdef USER_RELEASE
    #define DETAIL_LOG_SET FUNCNAME
#else
    #define DETAIL_LOG_SET CURRENT_DATETIME << "," << FILENAME << ", L:" << __LINE__ << "," << FUNCNAME << ","
#endif
#define MYLOG qDebug() << "[DEBUG]" << DETAIL_LOG_SET
#define MYINFOLOG qInfo() << "[INFO ]" << DETAIL_LOG_SET
#define MYWARNLOG qWarning() << "[WARN ]" << DETAIL_LOG_SET
#define MYFATALLOG qCritical() << "[FATAL]" << DETAIL_LOG_SET
#ifdef USER_RELEASE
    #define TMPLOG QStringList()    /// Dummy list
#else
    #define TMPLOG MYLOG
#endif
#define CALLOUT TMPLOG << "<<<<<<<<<<<<<<<<<<<<"

#ifdef DEBUG
    #define FAILASSERT(what)                                                                                           \
        Q_ASSERT_X(false, FUNCNAME, qPrintable(what));                                                                 \
        MYFATALLOG << what
#else
    #define FAILASSERT(what)                                                                                           \
        QMessageBox::critical(nullptr, "Fatal Error",                                                                  \
                              QString("%1\n%2\n\nPlease report to Presto developer team.").arg(FUNCNAME, what));       \
        MYFATALLOG << what
#endif

#define MY_ASSERT(cond, what)                                                                                          \
    if (!(cond)) {                                                                                                     \
        FAILASSERT(what);                                                                                              \
        throw std::runtime_error(qPrintable(what));                                                                    \
    }

#endif    // IRDROP_DEFINES_H
