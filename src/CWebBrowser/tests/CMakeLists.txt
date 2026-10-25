# src/CWebBrowser/tests/CMakeLists.txt

add_executable(test_webbrowser test_webbrowser.cpp)
target_link_libraries(test_webbrowser PRIVATE CWebBrowser)
add_test(NAME test_webbrowser COMMAND test_webbrowser)
