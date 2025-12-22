target("CommonCAF")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_links("caf_core", "caf_io")

