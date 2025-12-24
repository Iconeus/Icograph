target("session_manager")
    set_kind("binary")
    add_includedirs("include")
    add_files("src/*.cpp")
    add_deps("workflow_manager")
    add_deps("echo_view_model")
    add_deps("domain_model")
    add_deps("common_caf")
    add_deps("common_logger")

    -- Set the CAF option --config-file to pass a configuration file to the target.
    --  
    on_run(function (target)
        local configfile = get_config("caf-config-file")
        os.execv(target:targetfile(), { "--config-file", configfile })
    end)