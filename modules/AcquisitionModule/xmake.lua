target("acquisition_module")
    set_kind("shared")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_deps("common_caf")
    add_deps("common_logger")

    -- Unit test target
target("acquisition_module_tests")
    set_kind("binary")  
    add_files("tests/unit_tests/*.cpp")
    add_includedirs("include")
    add_deps("acquisition_module")
    add_packages("actor-framework", {components = {"caf_test"}})
    add_links("caf_test")
    add_tests("default", {runargs = {}}) -- Mark this target as a test
