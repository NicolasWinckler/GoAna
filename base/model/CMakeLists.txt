 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "SIDSData" 

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
)

If (BAT_FOUND)
    set( SYSTEM_INCLUDE_DIRECTORIES
        ${SYSTEM_INCLUDE_DIRECTORIES}
        ${CUBA_INCLUDE_DIR}
        ${BAT_INCLUDE_DIR}
    )

    If(FAIRROOT_FOUND)
        set( SYSTEM_INCLUDE_DIRECTORIES
        ${SYSTEM_INCLUDE_DIRECTORIES}
        ${ZMQ_INCLUDE_DIR}
        )
    EndIf(FAIRROOT_FOUND)

Else(BAT_FOUND)
    Set(SYSTEM_INCLUDE_DIRECTORIES
        ${SYSTEM_INCLUDE_DIRECTORIES}
    )
EndIf (BAT_FOUND)


include_directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

If (BAT_FOUND)
    set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${Boost_LIBRARY_DIRS}
    ${BAT_LIBRARY_DIR}
    #${CUBA_LIBRARY_DIR}
    ) 
Else(BAT_FOUND)
    set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${Boost_LIBRARY_DIRS}
    ) 
EndIf (BAT_FOUND)

link_directories(${LINK_DIRECTORIES})

Set(SRCS
    RooOscModel.cxx
    #data/OscMCPoint.cxx
)

If (BAT_FOUND)
    Set(NO_DICT_SRCS
        OscModel.cxx
        ExpModel.cxx
        )
EndIf (BAT_FOUND)

Set(LINKDEF SIDSModelLinkDef.h)
Set(LIBRARY_NAME SIDSModel)

Set(DEPENDENCIES 
    SIDSData
    boost_thread boost_system boost_serialization boost_program_options 
    RooFitCore RooFit Foam MathMore 
)


If (BAT_FOUND)
  Set(DEPENDENCIES ${DEPENDENCIES}
    ${BAT_LIBRARIES_SHARED}
    )
EndIf (BAT_FOUND)



GENERATE_LIBRARY()
