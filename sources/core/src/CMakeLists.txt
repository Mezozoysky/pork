set(HEADER_FILES
    pork/core/StrUtils.hpp
    pork/core/Exception.hpp
    pork/core/Errors.hpp
    # pork/core/TypeInfo.hpp
    pork/core/Logging.hpp
    )

set(SOURCE_FILES
    pork/core/StrUtils.cpp
    pork/core/Exception.cpp
    pork/core/Errors.cpp
    # pork/core/TypeInfo.cpp
    pork/core/Logging.cpp
    )

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}"
    PREFIX ""
    FILES ${HEADER_FILES} ${SOURCE_FILES}
    )

target_sources(${PROJECT_NAME}
    PUBLIC ${HEADER_FILES}
    PRIVATE ${SOURCE_FILES}
    )
