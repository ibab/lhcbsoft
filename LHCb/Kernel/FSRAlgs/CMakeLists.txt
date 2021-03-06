################################################################################
# Package: FSRAlgs
################################################################################
gaudi_subdir(FSRAlgs v2r1)

gaudi_depends_on_subdirs(Det/DetDesc
                         Event/DAQEvent
                         Event/EventBase
                         Event/FSREvent
                         Event/LumiEvent
                         Event/RecEvent
                         Event/GenEvent
                         GaudiAlg
                         GaudiConf
                         Kernel/HltInterfaces
                         Kernel/LHCbAlgs
                         Kernel/LHCbKernel)

find_package(Boost COMPONENTS iostreams)
find_package(RELAX)

gaudi_add_module(FSRAlgs
                 src/*.cpp
                 INCLUDE_DIRS Boost RELAX Event/EventBase Event/LumiEvent
                 LINK_LIBRARIES Boost RELAX DetDescLib DAQEventLib RecEvent GaudiAlgLib HltInterfaces LHCbKernel)

gaudi_install_headers(FSRAlgs)
gaudi_install_python_modules()


gaudi_add_test(QMTest QMTEST)
