build_lib_example(
  NAME vlcnew-example
  SOURCE_FILES vlcnew-example.cc
  LIBRARIES_TO_LINK
    ${libcore}
    ${libvlcnew}
)

