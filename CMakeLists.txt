cmake_minimum_required(VERSION 3.6)

PROJECT(gacpp)

INCLUDE_DIRECTORIES(
"lib3rd/catch.git/include/"
"./"
)

AUX_SOURCE_DIRECTORY(test TEST_ALL_SRCS)


SET(TEST_ALL
	${TEST_ALL_SRCS}
)

#SET(LIBRARIES
#	libm.so
#)


ADD_EXECUTABLE(test_all ${TEST_ALL})
#TARGET_LINK_LIBRARIES(../bin/bin ${LIBRARIES})

