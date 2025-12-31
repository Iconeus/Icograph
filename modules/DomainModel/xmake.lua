target("domain_model")
    set_kind("shared")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_deps("common_caf")
    add_deps("common_logger")

