 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "SIDSData" 


set(INCLUDE_DIRECTORIES
${CMAKE_SOURCE_DIR}/base
${CMAKE_SOURCE_DIR}/base/data
${CMAKE_SOURCE_DIR}/base/data/io
${CMAKE_SOURCE_DIR}/base/tools
)

Set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
)


include_directories(${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
) 

link_directories(${LINK_DIRECTORIES})

Set(SRCS 
    EsrSMSEvent.cxx
    EsrDecayEvent.cxx
    EsrBinDecayEvent.cxx
    EsrInjData.cxx
    EsrTree.cxx
    OscMCPoint.cxx
)

Set(NO_DICT_SRCS
    io/SIDSFileManager.cxx
  )


Set(LINKDEF SIDSDataLinkDef.h)
Set(LIBRARY_NAME SIDSData)

  Set(DEPENDENCIES
    SIDSTools
    boost_thread boost_system boost_serialization boost_filesystem 
    Tree Hist RIO Cint Core
    Minuit RooFitCore RooFit RooStats Foam MathMore
    Gui Spectrum)

GENERATE_LIBRARY()
