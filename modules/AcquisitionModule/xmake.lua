add_defines("ACQUISITION_MODULE")

target("acquisition_module")
    set_kind("shared")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_deps("common_caf")
    add_links("caf_core", "caf_io")