add_requires("actor-framework 1.1.0")
add_requires("spdlog v1.16.0")
add_requires("catch2 v3.11.0")

add_rules("mode.debug", "mode.release")

set_languages("cxx23")
set_warnings("everything", "error") -- Treat all compilation warnings as errors

includes("SessionManager")
includes("modules/WorkflowManager")
includes("modules/AcquisitionModule")
includes("modules/EchoViewModel")
includes("modules/DomainModel")
includes("modules/Common/CAF")
includes("modules/Common/Logger")

-- Option to add the caf configuration file with "xmake run".
-- Override default value with the command "xmake config --caf-config-file=path/to/file"
option("caf-config-file")
    set_default("configuration/caf-application.conf")
    set_showmenu(true)
    set_description("Set the configuration file for the application")
