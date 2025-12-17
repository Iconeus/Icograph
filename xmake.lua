add_defines("ROOT")

add_rules("mode.debug", "mode.release")

set_languages("cxx23")

includes("SessionManager")
includes("modules/WorkflowManager")
includes("modules/AcquisitionModule")
includes("modules/EchoViewModel")
includes("modules/DomainModel")
includes("modules/Common/CAF")

