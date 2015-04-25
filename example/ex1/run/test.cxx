
#include "ex1Options.h"

/// std
#include <iostream>
#include <string>

/// boost
#include "boost/program_options.hpp"

#include "FairMQLogger.h"


int main(int argc, char** argv)
{
    
    
    ex1Options* config = new ex1Options();
    config->ParseAll(argc,argv);
    int verbose=0;
    MQLOG(INFO)<<"verbose = "<<verbose;
    config->SetValue<int>("verbose",verbose);
    MQLOG(INFO)<<"verbose = "<<verbose;
    
    return 0;
}