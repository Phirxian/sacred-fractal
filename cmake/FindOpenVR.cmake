# - Find OpenVR SDK
# Find the OpenVR SDK headers and libraries.
#
#  OPENVR_SDK_INCLUDE_DIRS - where to find openvr.h, etc.
#  OPENVR_SDK_LIBRARIES    - List of libraries when using OpenVR SDK.
#  OPENVR_SDK_FOUND        - True if OpenVR SDK found.


SET(OPENVR_SDK_INCLUDE_DIRS /usr/local/include)
SET(OPENVR_SDK_LIBRARIES /usr/local/lib64/libopenvr_api64.a)
SET(OPENVR_SDK_LIBRARY /usr/local/lib64/libopenvr_api64.a)

message(STATUS "OPENVR_SDK_INCLUDE_DIRS " ${OPENVR_SDK_INCLUDE_DIRS})
message(STATUS "OPENVR_SDK_ROOT_DIR     " ${OPENVR_SDK_ROOT_DIR})
message(STATUS "OPENVR_SDK_LIBRARIES    " ${OPENVR_SDK_LIBRARIES})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenVRSDK DEFAULT_MSG OPENVR_SDK_LIBRARIES OPENVR_SDK_INCLUDE_DIRS)

MARK_AS_ADVANCED(OPENVR_SDK_LIBRARIES OPENVR_SDK_INCLUDE_DIRS)
