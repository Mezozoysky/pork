set(HEADER_FILES
    StrUtils.hpp
    Exception.hpp
    # TypeInfo.hpp
    # Service.hpp
    # Context.hpp
    Logging.hpp
    BaseApplication.hpp
    )

set(SOURCE_FILES
    StrUtils.cpp
    Exception.cpp
    # TypeInfo.cpp
    # Service.cpp
    # Context.cpp
    Logging.cpp
    BaseApplication.cpp
    )

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}"
    PREFIX ""
    FILES ${HEADER_FILES} ${SOURCE_FILES}
    )

target_sources(${PROJECT_NAME}
    PUBLIC ${HEADER_FILES}
    PRIVATE ${SOURCE_FILES}
    )
