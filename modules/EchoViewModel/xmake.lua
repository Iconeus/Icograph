add_defines("ECHO_VIEW_MODEL")

target("echo_view_model")
    set_kind("shared")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_links("caf_core", "caf_io")
