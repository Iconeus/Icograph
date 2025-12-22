target("DomainModel")
    set_kind("shared")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    add_packages("actor-framework", {components = {"caf_core", "caf_io"}})

