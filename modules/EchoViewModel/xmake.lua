target("echo_view_model")
    set_kind("shared")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_deps("common_caf")

