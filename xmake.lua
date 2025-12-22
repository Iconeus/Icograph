add_requires("actor-framework 1.1.0")

add_rules("mode.debug", "mode.release")

set_languages("cxx23")
set_warnings("everything", "error") -- Treat all compilation warnings as errors

includes("SessionManager")
includes("modules/WorkflowManager")
includes("modules/AcquisitionModule")
includes("modules/EchoViewModel")
includes("modules/DomainModel")
includes("modules/Common/CAF")

