target("WorkflowManager")
    set_kind("shared")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_deps("AcquisitionModule")
    add_deps("CommonCAF")
    add_packages("actor-framework", {components = {"caf_core", "caf_io"}})

    -- To indicate at runtime that its shared library dependencies shall be searched at the same directory (this is temporary)
    add_rpathdirs("$ORIGIN") 

-- Unit test target
target("WorkflowManagerTests")
    set_kind("binary")  
    add_files("tests/unit_tests/*.cpp")
    add_includedirs("include")
    add_deps("WorkflowManager") 
    add_deps("CommonCAF")
    add_packages("actor-framework", {components = {"caf_core", "caf_io", "caf_test"}})
    add_links("caf_test")
    add_tests("default", {runargs = {}}) -- Mark this target as a test
