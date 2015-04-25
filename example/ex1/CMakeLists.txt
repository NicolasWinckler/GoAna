 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "ex1" 

set(INCLUDE_DIRECTORIES
${CMAKE_SOURCE_DIR}/gobase/tools
${CMAKE_SOURCE_DIR}/example/ex1/lib
${CMAKE_SOURCE_DIR}/example/ex1/run
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
      lib/ex1Options.cxx
    )

#Set(LINKDEF ex1LinkDef.h)
Set(LIBRARY_NAME ex1)

If (Boost_FOUND)
  Set(DEPENDENCIES
    boost_system boost_program_options boost_log gobase
    )
EndIf (Boost_FOUND)

GENERATE_LIBRARY()

#Add_Subdirectory(macro)


If (Boost_FOUND)
  Set(Exe_Names
    runtest
  )



  set(Exe_Source
    
    run/test.cxx
    )

  List(LENGTH Exe_Names _length)
  Math(EXPR _length ${_length}-1)

  ForEach(_file RANGE 0 ${_length})
    List(GET Exe_Names ${_file} _name)
    List(GET Exe_Source ${_file} _src)
    Set(EXE_NAME ${_name})
    Set(SRCS ${_src})
    Set(DEPENDENCIES ex1)
    GENERATE_EXECUTABLE()
  EndForEach(_file RANGE 0 ${_length})
EndIf (Boost_FOUND)
