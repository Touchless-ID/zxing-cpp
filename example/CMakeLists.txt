#show that it is possible to use the library (in its simples form) with a c++-11 compiler
set (CMAKE_CXX_STANDARD 11)

zxing_add_package_stb()

if (BUILD_READERS)
    add_executable (ZXingReader ZXingReader.cpp)

    target_link_libraries (ZXingReader ZXing::ZXing stb::stb)

    add_test(NAME ZXingReaderTest COMMAND ZXingReader -fast -format qrcode "${CMAKE_SOURCE_DIR}/test/samples/qrcode-1/1.png")
endif()

if (BUILD_WRITERS)
    add_executable (ZXingWriter ZXingWriter.cpp)

    target_link_libraries (ZXingWriter ZXing::ZXing stb::stb)

    add_test(NAME ZXingWriterTest COMMAND ZXingWriter qrcode "I have the best words." test.png)
endif()


if (BUILD_READERS)
    find_package(Qt5 COMPONENTS Gui Multimedia Quick)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    if (TARGET Qt5::Gui)
        add_executable (ZXingQtReader ZXingQtReader.cpp ZXingQtReader.h)
        target_link_libraries(ZXingQtReader ZXing::ZXing Qt5::Gui)
    endif()

    if (TARGET Qt5::Multimedia)
        add_executable(ZXingQtCamReader ZXingQtCamReader.cpp ZXingQtCamReader.qrc ZXingQtReader.h)
        target_link_libraries(ZXingQtCamReader ZXing::ZXing Qt5::Gui Qt5::Multimedia Qt5::Quick)
    endif()

    find_package(OpenCV)
    if (OpenCV_FOUND)
        add_executable (ZXingOpenCV ZXingOpenCV.cpp)
        target_include_directories (ZXingOpenCV PRIVATE ${OpenCV_INCLUDE_DIRS})
        target_link_libraries (ZXingOpenCV ZXing::ZXing ${OpenCV_LIBS})
    endif()
endif()
