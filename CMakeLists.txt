project( textanalyzer ) 
cmake_minimum_required( VERSION 2.8.7 )
 
set(CMAKE_CXX_FLAGS "-fPIC -s -Wl,-z,relro,-z,now -fstack-protector -O2")
set(CMAKE_C_FLAGS "-fPIC -s -Wl,-z,relro,-z,now -fstack-protector -O2")
   
add_library(textanalyzer SHARED src/TextAnalyzer/WordCollection.c src/TextAnalyzer/WordCollection.h src/TextAnalyzer/HashFunctions.c src/TextAnalyzer/HashFunctions.h src/TextAnalyzer/TextAnalyzer.c src/TextAnalyzer/TextAnalyzer.h) 
#add_executable(textanalyzer WordCollection.cpp WordCollection.h HashFunctions.c HashFunctions.h TextAnalyzer.cpp TextAnalyzer.h) 
target_link_libraries(textanalyzer rt  )
set_target_properties(textanalyzer PROPERTIES DEBUG_POSTFIX "D")
       
set_target_properties(textanalyzer PROPERTIES 
                      DEBUG_POSTFIX "D"
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                     )


