check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources    
        #test/vlc-examples-test-suite.cc
        )
endif()    

build_lib(
    LIBNAME vlcnew
    SOURCE_FILES      
        model/vlcnew.cc   
        helper/vlcnew-helper.cc
        helper/vlcChannelHelper.cc
        helper/vlcDeviceHelper.cc
        model/vlcNetDevice.cc
        model/vlcchannel.cc
        model/vlcMobilityModel.cc
        model/VlcPropagationLoss.cc
        model/vlcNetDeviceRX.cc
        model/vlcNetDeviceTX.cc
        model/VLC_SNR.cc
        model/VLC-error-model.cc
    HEADER_FILES   
        model/vlcnew.h
        helper/vlcnew-helper.h
        helper/vlcChannelHelper.h
        helper/vlcDeviceHelper.h
        model/vlcNetDevice.h
        model/vlcchannel.h
        model/vlcMobilityModel.h
        model/VlcPropagationLoss.h
        model/vlcNetDeviceRX.h
        model/vlcNetDeviceTX.h
        model/VLC_SNR.h
        model/VLC-error-model.h
    LIBRARIES_TO_LINK 
        ${libnetwork}
        ${libmpi}
        ${libmobility}
        ${libpropagation}
        ${libpoint-to-point}
    TEST_SOURCES test/vlcnew-test-suite.cc
                 ${examples_as_tests_sources}
)
    
