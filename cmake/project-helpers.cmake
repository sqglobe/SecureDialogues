include(CheckIPOSupported)

macro(enable_interprocedural_optimization target)
    check_ipo_supported(RESULT result)
    if(result AND ENABLE_INTERPROCEDURAL_OPTIMIZATION)
        set_target_properties(${target} PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    endif()
endmacro()
