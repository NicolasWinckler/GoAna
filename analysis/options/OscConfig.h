/* 
 * File:   OscConfig.h
 * Author: winckler
 *
 * Created on April 8, 2015, 3:27 PM
 */

#ifndef OSCCONFIG_H
#define	OSCCONFIG_H

#include "SIDSProgOptions.h"

class OscConfig : public SIDSProgOptions
{
public:
    OscConfig();
    virtual ~OscConfig();
    
    virtual int ParseAll(const int argc, char** argv, bool AllowUnregistered=false);
    
protected:
   
    po::options_description fObservable_options;
    po::options_description fModel_options;
    po::options_description fBAT_options;
    po::options_description fSim_options;
    int InitOptionDescription();
};

#endif	/* OSCCONFIG_H */

