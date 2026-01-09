target("common_caf")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_packages("actor-framework", {components = {"caf_core", "caf_io"}}, {public = true})

