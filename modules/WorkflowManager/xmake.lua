target("workflow_manager")
    set_kind("shared")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_deps("acquisition_module")
    add_deps("common_caf")
    add_deps("common_logger")

    -- To indicate at runtime that its shared library dependencies shall be searched at the same directory (this is temporary)
    add_rpathdirs("$ORIGIN") 

-- Unit test target
target("workflow_manager_tests")
    set_kind("binary")  
    add_files("tests/unit_tests/*.cpp")
    add_includedirs("include")
    add_deps("workflow_manager") 
    add_packages("actor-framework", {components = {"caf_test"}})
    add_links("caf_test")
    add_tests("default", {runargs = {}}) -- Mark this target as a test
