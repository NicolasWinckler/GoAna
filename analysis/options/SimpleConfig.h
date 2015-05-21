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
    void UseConfigFile(bool cfg=true)
    {
        fCmdline_options.add_options()
            ("config,c", po::value<std::string>(&fConfigFile)->required(), "Path to configuration file")
            ;// todo get ridd of this line in mother class constructor
        fconfigfile_flag=cfg;
    }
    
protected:
   
    po::options_description fObservable_options;
    int InitOptionDescription();
    
    bool fconfigfile_flag;
};

#endif	/* SIMPLECONFIG_H */

