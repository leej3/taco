# Add an interface library for miscellaneous headers
add_library(libheaders INTERFACE)
# Add version header:
configure_file(
  "${PROJECT_SOURCE_DIR}/include/taco/version.h.in"
  "${PROJECT_BINARY_DIR}/include/taco/version.h"
  @ONLY
)
target_sources(libheaders PUBLIC ${PROJECT_BINARY_DIR}/include/taco/version.h)

target_include_directories(
  libheaders
  INTERFACE ${PROJECT_BINARY_DIR}/include
)

add_library(taco)
target_sources(
  taco
  PRIVATE
    codegen/codegen.cpp codegen/codegen_c.cpp codegen/codegen_cuda.cpp
    codegen/module.cpp cuda.cpp error.cpp error/error_checks.cpp
    error/error_messages.cpp format.cpp index_notation/index_notation.cpp
    index_notation/index_notation_nodes.cpp
    index_notation/index_notation_nodes_abstract.cpp
    index_notation/index_notation_printer.cpp
    index_notation/index_notation_rewriter.cpp
    index_notation/index_notation_visitor.cpp index_notation/intrinsic.cpp
    index_notation/kernel.cpp index_notation/provenance_graph.cpp
    index_notation/schedule.cpp index_notation/transformations.cpp ir/ir.cpp
    ir/ir_generators.cpp ir/ir_printer.cpp ir/ir_rewriter.cpp
    ir/ir_verifier.cpp ir/ir_visitor.cpp ir/simplify.cpp
    ir/workspace_rewriter.cpp ir_tags.cpp lower/expr_tools.cpp
    lower/iteration_forest.cpp lower/iteration_graph.cpp lower/iterator.cpp
    lower/lower.cpp lower/lowerer_impl.cpp lower/lowerer_impl_imperative.cpp
    lower/merge_lattice.cpp lower/mode.cpp lower/mode_access.cpp
    lower/mode_format_compressed.cpp lower/mode_format_dense.cpp
    lower/mode_format_impl.cpp lower/mode_format_singleton.cpp
    lower/tensor_path.cpp parser/einsum_parser.cpp parser/lexer.cpp
    parser/parser.cpp parser/schedule_parser.cpp storage/array.cpp
    storage/file_io_mtx.cpp storage/file_io_rb.cpp storage/file_io_tns.cpp
    storage/index.cpp storage/pack.cpp storage/storage.cpp
    storage/typed_index.cpp storage/typed_value.cpp taco_tensor_t.cpp
    target.cpp tensor.cpp type.cpp util/env.cpp util/files.cpp
    util/name_generator.cpp util/strings.cpp
)

if (CUDA)
  include_directories(${CUDA_INCLUDE_DIRS})
  target_link_libraries(taco PUBLIC ${CUDA_LIBRARIES})
endif (CUDA)
target_include_directories(
  taco
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/taco>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}/taco>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(taco
  PUBLIC
    libheaders
  PRIVATE
    $<TARGET_NAME_IF_EXISTS:OpenMP::OpenMP_CXX>
)
target_compile_definitions(taco
  PRIVATE
    "$<IF:$<TARGET_EXISTS:OpenMP::OpenMP_CXX>,USE_OPENMP,>"
    $<$<PLATFORM_ID:Linux>:dl>
)
set_target_properties(
  taco
  PROPERTIES PUBLIC_HEADER
    $<TARGET_PROPERTY:libheaders,INTERFACE_SOURCES>
)

# add alias for namespaced target to allow downstreams to use targets provided
# from a pre-existing installation (system installation or superbuild project
# structure) or by the current cmake system at configure time (non-superbuild
# project structure).
add_library(taco::taco ALIAS taco)

# Install taco library along with the public headers that are located outside
# of the include directory in the project source.
install(
  TARGETS taco
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/taco
)
# Install headers for taco
install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h"
)
