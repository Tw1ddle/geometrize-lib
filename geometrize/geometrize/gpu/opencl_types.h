#pragma once

#if defined(_WIN32)
#define GEOMETRIZE_CL_CALL __stdcall
#define GEOMETRIZE_CL_CALLBACK __stdcall
#else
#define GEOMETRIZE_CL_CALL
#define GEOMETRIZE_CL_CALLBACK
#endif

#include <cstddef>
#include <cstdint>

namespace geometrize
{
namespace gpu
{
namespace cl
{

using char_t = std::int8_t;
using uchar_t = std::uint8_t;
using short_t = std::int16_t;
using ushort_t = std::uint16_t;
using int_t = std::int32_t;
using uint_t = std::uint32_t;
using long_t = std::int64_t;
using ulong_t = std::uint64_t;
using bool_t = uint_t;
using bitfield = ulong_t;
using device_type = bitfield;
using mem_flags = bitfield;
using command_queue_properties = bitfield;
using platform_info = uint_t;
using device_info = uint_t;
using program_build_info = uint_t;
using context_properties = std::intptr_t;

struct platform_id_impl;
struct device_id_impl;
struct context_impl;
struct command_queue_impl;
struct mem_impl;
struct program_impl;
struct kernel_impl;
struct event_impl;

using platform_id = platform_id_impl*;
using device_id = device_id_impl*;
using context = context_impl*;
using command_queue = command_queue_impl*;
using mem = mem_impl*;
using program = program_impl*;
using kernel = kernel_impl*;
using event = event_impl*;

constexpr int_t success = 0;
constexpr int_t device_not_found = -1;
constexpr bool_t false_value = 0;
constexpr bool_t true_value = 1;

constexpr device_type device_type_gpu = (1ULL << 2U);

constexpr platform_info platform_name = 0x0902;
constexpr platform_info platform_vendor = 0x0903;

constexpr device_info device_max_compute_units = 0x1002;
constexpr device_info device_max_work_group_size = 0x1004;
constexpr device_info device_available = 0x1027;
constexpr device_info device_compiler_available = 0x1028;
constexpr device_info device_name = 0x102B;
constexpr device_info device_vendor = 0x102C;
constexpr device_info device_host_unified_memory = 0x1035;

constexpr context_properties context_platform = 0x1084;

constexpr mem_flags mem_read_write = (1ULL << 0U);
constexpr mem_flags mem_write_only = (1ULL << 1U);
constexpr mem_flags mem_read_only = (1ULL << 2U);

constexpr program_build_info program_build_log = 0x1183;

using context_notify = void (GEOMETRIZE_CL_CALLBACK *)(const char*, const void*, std::size_t, void*);
using program_notify = void (GEOMETRIZE_CL_CALLBACK *)(program, void*);

using get_platform_ids_fn = int_t (GEOMETRIZE_CL_CALL *)(uint_t, platform_id*, uint_t*);
using get_platform_info_fn = int_t (GEOMETRIZE_CL_CALL *)(platform_id, platform_info, std::size_t, void*, std::size_t*);
using get_device_ids_fn = int_t (GEOMETRIZE_CL_CALL *)(platform_id, device_type, uint_t, device_id*, uint_t*);
using get_device_info_fn = int_t (GEOMETRIZE_CL_CALL *)(device_id, device_info, std::size_t, void*, std::size_t*);
using create_context_fn = context (GEOMETRIZE_CL_CALL *)(const context_properties*, uint_t, const device_id*, context_notify, void*, int_t*);
using release_context_fn = int_t (GEOMETRIZE_CL_CALL *)(context);
using create_command_queue_fn = command_queue (GEOMETRIZE_CL_CALL *)(context, device_id, command_queue_properties, int_t*);
using release_command_queue_fn = int_t (GEOMETRIZE_CL_CALL *)(command_queue);
using create_program_with_source_fn = program (GEOMETRIZE_CL_CALL *)(context, uint_t, const char**, const std::size_t*, int_t*);
using build_program_fn = int_t (GEOMETRIZE_CL_CALL *)(program, uint_t, const device_id*, const char*, program_notify, void*);
using get_program_build_info_fn = int_t (GEOMETRIZE_CL_CALL *)(program, device_id, program_build_info, std::size_t, void*, std::size_t*);
using release_program_fn = int_t (GEOMETRIZE_CL_CALL *)(program);
using create_kernel_fn = kernel (GEOMETRIZE_CL_CALL *)(program, const char*, int_t*);
using release_kernel_fn = int_t (GEOMETRIZE_CL_CALL *)(kernel);
using create_buffer_fn = mem (GEOMETRIZE_CL_CALL *)(context, mem_flags, std::size_t, void*, int_t*);
using release_mem_object_fn = int_t (GEOMETRIZE_CL_CALL *)(mem);
using set_kernel_arg_fn = int_t (GEOMETRIZE_CL_CALL *)(kernel, uint_t, std::size_t, const void*);
using enqueue_write_buffer_fn = int_t (GEOMETRIZE_CL_CALL *)(command_queue, mem, bool_t, std::size_t, std::size_t, const void*, uint_t, const event*, event*);
using enqueue_read_buffer_fn = int_t (GEOMETRIZE_CL_CALL *)(command_queue, mem, bool_t, std::size_t, std::size_t, void*, uint_t, const event*, event*);
using enqueue_nd_range_kernel_fn = int_t (GEOMETRIZE_CL_CALL *)(command_queue, kernel, uint_t, const std::size_t*, const std::size_t*, const std::size_t*, uint_t, const event*, event*);
using finish_fn = int_t (GEOMETRIZE_CL_CALL *)(command_queue);

}
}
}

