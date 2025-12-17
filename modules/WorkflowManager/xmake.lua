add_defines("WORKFLOW_MANAGER")

target("workflow_manager")
    set_kind("shared")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_deps("acquisition_module")
    add_deps("common_caf")
    add_links("caf_core", "caf_io")

