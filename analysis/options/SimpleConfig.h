/* 
 * File:   SimpleConfig.h
 * Author: winckler
 *
 * Created on May 6, 2015, 11:37 AM
 */

#ifndef SIMPLECONFIG_H
#define	SIMPLECONFIG_H

#include "SIDSProgOptions.h"

class SimpleConfig : public SIDSProgOptions
{
public:
    SimpleConfig();
    virtual ~SimpleConfig();
    
    virtual int ParseAll(const int argc, char** argv, bool AllowUnregistered=false);
    
protected:
   
    po::options_description fObservable_options;
    int InitOptionDescription();
};

#endif	/* SIMPLECONFIG_H */

