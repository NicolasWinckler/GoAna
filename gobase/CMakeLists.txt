 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "gobase" 

set(INCLUDE_DIRECTORIES
${CMAKE_SOURCE_DIR}/gobase/tools
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${SYSTEM_INCLUDE_DIRECTORIES}
)

include_directories(${INCLUDE_DIRECTORIES})
include_directories(${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
#${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
) 

link_directories(${LINK_DIRECTORIES})

Set(SRCS)

Set(NO_DICT_SRCS
      tools/FairProgOptions.cxx
      tools/FairMQLogger.cxx
    )

#Set(LINKDEF gobaseLinkDef.h)
Set(LIBRARY_NAME gobase)

If (Boost_FOUND)
  Set(DEPENDENCIES
    boost_system boost_program_options boost_log
    )
EndIf (Boost_FOUND)

GENERATE_LIBRARY()
