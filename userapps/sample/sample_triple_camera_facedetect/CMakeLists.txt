cmake_minimum_required(VERSION 3.2)

set(mpp_sdk_root $ENV{MPP_SRC_DIR})
set(nncase_sdk_root $ENV{MPP_SRC_DIR}/../libs/nncase)
set(install_dir $ENV{MPP_USERAPP_SMAPLE_ELF_DIR})

message(STATUS "nncase_sdk_root ${nncase_sdk_root}")

include(${nncase_sdk_root}/examples/cmake/Riscv64.cmake)

project(sample_triple_camera_facedetect C CXX)

set(CMAKE_EXE_LINKER_FLAGS "-T ${nncase_sdk_root}/examples/cmake/link.lds --static")

set(src main.cc model.cc mobile_retinaface.cc anchors_320.cc util.cc)
set(bin sample_triple_camera_facedetect.elf)
set(script ./script/init.sh)
set(data ${mpp_sdk_root}/userapps/sample/kmodel/mobile_retinaface/test.kmodel)

set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/../" CACHE STRING "The path to use for make install" FORCE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -march=rv64imafdcv -mabi=lp64d -mcmodel=medany" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${nncase_sdk_root}/riscv64)
include_directories(${nncase_sdk_root}/riscv64/nncase/include)
include_directories(${nncase_sdk_root}/riscv64/nncase/include/nncase/runtime)
include_directories(${nncase_sdk_root}/riscv64/rvvlib/include)
include_directories(${mpp_sdk_root}/userapps/api/)
include_directories(${mpp_sdk_root}/include)
include_directories(${mpp_sdk_root}/include/comm)
include_directories(${mpp_sdk_root}/userapps/sample/sample_vo)

link_directories(${mpp_sdk_root}/userapps/lib)
link_directories(${nncase_sdk_root}/riscv64/rvvlib/)
link_directories(${nncase_sdk_root}/riscv64/nncase/lib/)

add_executable(${bin} ${src})
target_link_libraries(${bin} -Wl,--start-group rvv Nncase.Runtime.Native nncase.rt_modules.k230 functional_k230 sys vicap vb cam_device cam_engine
 hal oslayer ebase fpga isp_drv binder auto_ctrol common cam_caldb isi 3a buffer_management cameric_drv video_in virtual_hal start_engine cmd_buffer
 switch cameric_reg_drv t_database_c t_mxml_c t_json_c t_common_c vo connector sensor atomic dma -Wl,--end-group)

install(TARGETS ${bin} DESTINATION ${install_dir}/triple_camera_facedetect)
install(FILES ${data} DESTINATION ${install_dir}/triple_camera_facedetect)
