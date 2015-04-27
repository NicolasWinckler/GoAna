/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/**
 * SIDSLog.h
 *
 * @since 2012-12-04
 * @author D. Klein, A. Rybalchenko, N. Winckler
 */

#ifndef SIDSLOG_H
#define	SIDSLOG_H

#include <sstream>
#include <sys/time.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <stdio.h>

class SIDSLog 
{
public:
    enum Level{DEBUG, INFO, SUMMARY, WARNING, ERROR, NOLOG};
    SIDSLog();
    virtual ~SIDSLog();
    
    std::ostringstream& Log(int type);
    
    static void SetLogLevel(int loglevel)
    { 
        SIDSLog::fMinLogLevel = loglevel; 
    }
    
    
private:
    std::ostringstream os;
    int fLogLevel;
    static int fMinLogLevel;
};

typedef unsigned long long timestamp_t;
timestamp_t get_timestamp();

#define LOG(type) SIDSLog().Log(SIDSLog::type)
#define SLOG(type) SIDSLog().Log(SIDSLog::type)
#define SET_LOG_LEVEL(loglevel) SIDSLog::SetLogLevel(SIDSLog::loglevel)

#endif	/* SIDSLOG_H */

