set(HEADER_FILES
    pork/base/Service.hpp
    pork/base/Context.hpp
    pork/base/Application.hpp
    pork/base/ValueStore.hpp
    )

set(SOURCE_FILES
    pork/base/Service.cpp
    pork/base/Context.cpp
    pork/base/Application.cpp
    pork/base/ValueStore.cpp
    )

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}"
    PREFIX ""
    FILES ${HEADER_FILES} ${SOURCE_FILES}
    )

target_sources(${PROJECT_NAME}
    PUBLIC ${HEADER_FILES}
    PRIVATE ${SOURCE_FILES}
    )
