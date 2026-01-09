target("common_logger")
    set_kind("shared")
    add_includedirs("include", {public = true})
    add_files("src/*.cpp")
    add_packages("spdlog", {public = true})


-- Unit test target
target("common_logger_tests")
    set_kind("binary")  
    add_files("tests/unit_tests/*.cpp")
    add_deps("common_logger") 
    add_packages("catch2")
    add_tests("default")