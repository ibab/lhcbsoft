################################################################################
# Package: VPDAQ
################################################################################
gaudi_subdir(VPDAQ v2r8)

gaudi_depends_on_subdirs(DAQ/DAQUtils
			 DAQ/DAQKernel	
                         Event/DAQEvent
                         Event/DigiEvent
                         GaudiAlg
                         Kernel/LHCbKernel)

find_package(AIDA)

gaudi_add_module(VPDAQ
                 src/*.cpp
                 INCLUDE_DIRS AIDA Event/DigiEvent
                 LINK_LIBRARIES DAQEventLib DAQKernelLib GaudiAlgLib LHCbKernel)

