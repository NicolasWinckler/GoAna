/* 
 * File:   ex1Options.cxx
 * Author: winckler
 * 
 * Created on April 25, 2015, 6:36 PM
 */

#include "ex1Options.h"

ex1Options::ex1Options() : FairProgOptions()
{
    fCmdline_options.add(fGenericDesc);
    fVisible_options.add(fCmdline_options);
}

ex1Options::~ex1Options() 
{
}

int ex1Options::ParseAll(const int argc, char** argv, bool AllowUnregistered)
{
    if(ParseCmdLine(argc,argv,fCmdline_options,fvarmap,AllowUnregistered))
        return 1;
}
