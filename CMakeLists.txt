
cmake_minimum_required(VERSION 2.8.9)
project(directory_test)
 
#Bring the headers, such as Student.h into the project
include_directories(headerFiles/Parser samplePrograms)
include_directories(headerFiles)

include_directories(dotGraph/headerFiles)
include_directories(hashTable/headerFiles)
include_directories(Instruction/headerFiles)
include_directories(container/headerFiles)
include_directories(src/SSA/SSA_Instruction/headerFiles)
include_directories(src/SSA/SSA_Phi_Instr/headerFiles)
include_directories(src/SSA/SSA_ComSubExp/headerFiles)
include_directories(src/SSA/SSA_Arrays/headerFiles)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.c")
file(GLOB PARSER_SOURCES "src/parser/*.c*")

add_library(compilerDataStructures "container/src/container.c"
                                    "dotGraph/src/dotGraph.c"
                                    "hashTable/src/valueInt.c"
                                    "hashTable/src/keyString.c"
                                    "hashTable/src/hashTable.c"
                                    "Instruction/src/instruction.c"
                                    "src/SSA/SSA_Instruction/src/ssaInstr.c"
                                    "src/SSA/SSA_Phi_Instr/src/ssaPhi.c"
                                    "src/SSA/SSA_Instruction/src/ssaInstrRename.c"
                                    "src/SSA/SSA_ComSubExp/src/ssaComSubExp.c"
                                    "src/SSA/SSA_Arrays/src/arrays.c"
                                    "src/symTable.c"
                                    "src/SSA/SSA_Arrays/src/kills.c")
 
add_executable(smpl ${SOURCES} ${PARSER_SOURCES})
target_link_libraries(smpl compilerDataStructures)

add_executable(ssaPhiTest tests/testSSAPhi.c src/io.c)
target_link_libraries(ssaPhiTest compilerDataStructures)

add_executable(instrTest Instruction/src/instruction.c tests/instrTest.c)
add_executable(containerTest container/src/container.c 
                              Instruction/src/instruction.c 
                              tests/containerTest.c
                              hashTable/src/valueInt.c)
                              
add_executable(ssaInstrTest container/src/container.c 
                            Instruction/src/instruction.c
                            hashTable/src/hashTable.c
                            hashTable/src/keyString.c
                            hashTable/src/valueInt.c
                            src/SSA/SSA_Instruction/src/ssaInstr.c
                            src/SSA/SSA_ComSubExp/src/ssaComSubExp.c
                            src/io.c
                            tests/ssaInstr_Test.c src/symTable.c)

add_executable(dotGraphTest container/src/container.c
                            Instruction/src/instruction.c
                            hashTable/src/hashTable.c
                            hashTable/src/valueInt.c
                            dotGraph/src/dotGraph.c
                            tests/dotGraph_Test.c
                            src/io.c)
add_executable(ssaRenameTest src/io.c tests/ssaRenameTest.c src/SSA/SSA_Instruction/src/ssaInstrRename.c)
add_executable(ssaComSubTest src/io.c tests/ssaComSubTest.c)
add_executable(hashTableTest tests/hashTableTest.c)
add_executable(arraysTest tests/arraysTest.c src/io.c)
add_executable(symTableTest tests/symTableTest.c src/io.c )

target_link_libraries(symTableTest compilerDataStructures )
target_link_libraries(hashTableTest compilerDataStructures)
target_link_libraries(ssaRenameTest compilerDataStructures)
target_link_libraries(dotGraphTest compilerDataStructures)
target_link_libraries(ssaComSubTest compilerDataStructures)
target_link_libraries(arraysTest compilerDataStructures)
                
#include(FetchContent)
#FetchContent_Declare(
#  googletest
  # Specify the commit you depend on and update it regularly.
#  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
#)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
#add_executable(hashTest src/blocks.c src/hashTable.c tests/hashTest.c)
#target_link_libraries(hashTest gtest_main)
#add_test(NAME example_test COMMAND example)