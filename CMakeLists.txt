PROJECT(mwc)

ADD_EXECUTABLE(test_mwc main.cpp mwc.cpp test_product.cpp test_mock.cpp)
TARGET_LINK_LIBRARIES(test_mwc cppunit gmock gtest pthread)
