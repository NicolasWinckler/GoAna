/* 
 * File:   ex1Options.h
 * Author: winckler
 *
 * Created on April 25, 2015, 6:36 PM
 */

#ifndef EX1OPTIONS_H
#define	EX1OPTIONS_H

#include "FairProgOptions.h"

class ex1Options : public FairProgOptions
{
public:
    ex1Options();
    virtual ~ex1Options();
    
    virtual int ParseAll(const int argc, char** argv, bool AllowUnregistered=false);
    

private:

};

#endif	/* EX1OPTIONS_H */

