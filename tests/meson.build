

# add gtest depecency to the test executable
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')


dirs = include_directories('../src', '../include','../tests')

ctest = executable('MCP_test', ['test_main.cpp'] + src_code, dependencies: gtest_dep , include_directories: dirs)

test('MCP_test', ctest)


