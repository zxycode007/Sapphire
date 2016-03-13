#ifndef _SAPPHIRE_C_OPENGL_EXTENSION_HANDLER_
#define _SAPPHIRE_C_OPENGL_EXTENSION_HANDLER_

#include "SapphireEDriverFeatures.h"
#include "SapphirePrerequisites.h"

//#include "irrTypes.h"
//#include "os.h"

#if SAPPHIRE_3D_API == SAPPHIRE_WITH_OPENGL


#if SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_WIN32
//包含WINDOWS头文件
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
#define GL_GLEXT_LEGACY 1
#endif
#include <GL/gl.h>
#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
#include "glext.h"
#endif
#include "wglext.h"

#if (SAPPHIRE_COMPILER == SAPPHIRE_COMPILER_MSVC)
#pragma comment(lib, "OpenGL32.lib")
#endif

#elif (SAPPHIRE_PLATFORM == SAPPHIRE_PLATFORM_APPLE)
#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
#define GL_GLEXT_LEGACY 1
#endif
#include <OpenGL/gl.h>
#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
#include "glext.h"
#endif
#elif defined(SAPPHIRE_PLATFORM_SDL) && !defined(SAPPHIRE_PLATFORM_X11)
#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
#define GL_GLEXT_LEGACY 1
#define GLX_GLXEXT_LEGACY 1
#else
#define GL_GLEXT_PROTOTYPES 1
#define GLX_GLXEXT_PROTOTYPES 1
#endif
#define NO_SDL_GLEXT
#include <SDL/SDL_video.h>
#include <SDL/SDL_opengl.h>
#include "glext.h"
#else
#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
#define GL_GLEXT_LEGACY 1
#define GLX_GLXEXT_LEGACY 1
#else
#define GL_GLEXT_PROTOTYPES 1
#define GLX_GLXEXT_PROTOTYPES 1
#endif
#include <GL/gl.h>
#include <GL/glx.h>
#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
#include "glext.h"
#undef GLX_ARB_get_proc_address // avoid problems with local glxext.h
#include "glxext.h"
#endif
#endif

#ifndef GL_ARB_shader_objects
/* 
对可编程shader文本和shader对象句柄的GL类型

*/
typedef char GLcharARB;
typedef unsigned int GLhandleARB;
#endif

#ifndef GL_VERSION_2_0
/* 
对可编程shader文本的GL类型
GL type for program/shader text
*/
typedef char GLchar;
#endif


namespace Sapphire
{


		static const char* const OpenGLFeatureStrings[] = {
			"GL_3DFX_multisample",
			"GL_3DFX_tbuffer",
			"GL_3DFX_texture_compression_FXT1",
			"GL_AMD_blend_minmax_factor",
			"GL_AMD_conservative_depth",
			"GL_AMD_debug_output",
			"GL_AMD_depth_clamp_separate",
			"GL_AMD_draw_buffers_blend",
			"GL_AMD_multi_draw_indirect",
			"GL_AMD_name_gen_delete",
			"GL_AMD_performance_monitor",
			"GL_AMD_sample_positions",
			"GL_AMD_seamless_cubemap_per_texture",
			"GL_AMD_shader_stencil_export",
			"GL_AMD_texture_texture4",
			"GL_AMD_transform_feedback3_lines_triangles",
			"GL_AMD_vertex_shader_tesselator",
			"GL_APPLE_aux_depth_stencil",
			"GL_APPLE_client_storage",
			"GL_APPLE_element_array",
			"GL_APPLE_fence",
			"GL_APPLE_float_pixels",
			"GL_APPLE_flush_buffer_range",
			"GL_APPLE_object_purgeable",
			"GL_APPLE_rgb_422",
			"GL_APPLE_row_bytes",
			"GL_APPLE_specular_vector",
			"GL_APPLE_texture_range",
			"GL_APPLE_transform_hint",
			"GL_APPLE_vertex_array_object",
			"GL_APPLE_vertex_array_range",
			"GL_APPLE_vertex_program_evaluators",
			"GL_APPLE_ycbcr_422",
			"GL_ARB_base_instance",
			"GL_ARB_blend_func_extended",
			"GL_ARB_cl_event",
			"GL_ARB_color_buffer_float",
			"GL_ARB_compatibility",
			"GL_ARB_compressed_texture_pixel_storage",
			"GL_ARB_conservative_depth",
			"GL_ARB_copy_buffer",
			"GL_ARB_debug_output",
			"GL_ARB_depth_buffer_float",
			"GL_ARB_depth_clamp",
			"GL_ARB_depth_texture",
			"GL_ARB_draw_buffers",
			"GL_ARB_draw_buffers_blend",
			"GL_ARB_draw_elements_base_vertex",
			"GL_ARB_draw_indirect",
			"GL_ARB_draw_instanced",
			"GL_ARB_ES2_compatibility",
			"GL_ARB_explicit_attrib_location",
			"GL_ARB_fragment_coord_conventions",
			"GL_ARB_fragment_program",
			"GL_ARB_fragment_program_shadow",
			"GL_ARB_fragment_shader",
			"GL_ARB_framebuffer_object",
			"GL_ARB_framebuffer_sRGB",
			"GL_ARB_get_program_binary",
			"GL_ARB_geometry_shader4",
			"GL_ARB_gpu_shader5",
			"GL_ARB_gpu_shader_fp64",
			"GL_ARB_half_float_pixel",
			"GL_ARB_half_float_vertex",
			"GL_ARB_imaging",
			"GL_ARB_instanced_arrays",
			"GL_ARB_internalformat_query",
			"GL_ARB_map_buffer_alignment",
			"GL_ARB_map_buffer_range",
			"GL_ARB_matrix_palette",
			"GL_ARB_multisample",
			"GL_ARB_multitexture",
			"GL_ARB_occlusion_query",
			"GL_ARB_occlusion_query2",
			"GL_ARB_pixel_buffer_object",
			"GL_ARB_point_parameters",
			"GL_ARB_point_sprite",
			"GL_ARB_provoking_vertex",
			"GL_ARB_robustness",
			"GL_ARB_sample_shading",
			"GL_ARB_sampler_objects",
			"GL_ARB_seamless_cube_map",
			"GL_ARB_separate_shader_objects",
			"GL_ARB_shader_atomic_counters",
			"GL_ARB_shader_bit_encoding",
			"GL_ARB_shader_image_load_store",
			"GL_ARB_shader_objects",
			"GL_ARB_shader_precision",
			"GL_ARB_shader_stencil_export",
			"GL_ARB_shader_subroutine",
			"GL_ARB_shader_texture_lod",
			"GL_ARB_shading_language_100",
			"GL_ARB_shading_language_420pack",
			"GL_ARB_shading_language_include",
			"GL_ARB_shading_language_packing",
			"GL_ARB_shadow",
			"GL_ARB_shadow_ambient",
			"GL_ARB_sync",
			"GL_ARB_tessellation_shader",
			"GL_ARB_texture_border_clamp",
			"GL_ARB_texture_buffer_object",
			"GL_ARB_texture_buffer_object_rgb32",
			"GL_ARB_texture_compression",
			"GL_ARB_texture_compression_bptc",
			"GL_ARB_texture_compression_rgtc",
			"GL_ARB_texture_cube_map",
			"GL_ARB_texture_cube_map_array",
			"GL_ARB_texture_env_add",
			"GL_ARB_texture_env_combine",
			"GL_ARB_texture_env_crossbar",
			"GL_ARB_texture_env_dot3",
			"GL_ARB_texture_float",
			"GL_ARB_texture_gather",
			"GL_ARB_texture_mirrored_repeat",
			"GL_ARB_texture_multisample",
			"GL_ARB_texture_non_power_of_two",
			"GL_ARB_texture_query_lod",
			"GL_ARB_texture_rectangle",
			"GL_ARB_texture_rg",
			"GL_ARB_texture_rgb10_a2ui",
			"GL_ARB_texture_storage",
			"GL_ARB_texture_swizzle",
			"GL_ARB_timer_query",
			"GL_ARB_transform_feedback2",
			"GL_ARB_transform_feedback3",
			"GL_ARB_transform_feedback_instanced",
			"GL_ARB_transpose_matrix",
			"GL_ARB_uniform_buffer_object",
			"GL_ARB_vertex_array_bgra",
			"GL_ARB_vertex_array_object",
			"GL_ARB_vertex_attrib_64bit",
			"GL_ARB_vertex_blend",
			"GL_ARB_vertex_buffer_object",
			"GL_ARB_vertex_program",
			"GL_ARB_vertex_shader",
			"GL_ARB_vertex_type_2_10_10_10_rev",
			"GL_ARB_viewport_array",
			"GL_ARB_window_pos",
			"GL_ATI_draw_buffers",
			"GL_ATI_element_array",
			"GL_ATI_envmap_bumpmap",
			"GL_ATI_fragment_shader",
			"GL_ATI_map_object_buffer",
			"GL_ATI_meminfo",
			"GL_ATI_pixel_format_float",
			"GL_ATI_pn_triangles",
			"GL_ATI_separate_stencil",
			"GL_ATI_text_fragment_shader",
			"GL_ATI_texture_env_combine3",
			"GL_ATI_texture_float",
			"GL_ATI_texture_mirror_once",
			"GL_ATI_vertex_array_object",
			"GL_ATI_vertex_attrib_array_object",
			"GL_ATI_vertex_streams",
			"GL_EXT_422_pixels",
			"GL_EXT_abgr",
			"GL_EXT_bgra",
			"GL_EXT_bindable_uniform",
			"GL_EXT_blend_color",
			"GL_EXT_blend_equation_separate",
			"GL_EXT_blend_func_separate",
			"GL_EXT_blend_logic_op",
			"GL_EXT_blend_minmax",
			"GL_EXT_blend_subtract",
			"GL_EXT_clip_volume_hint",
			"GL_EXT_cmyka",
			"GL_EXT_color_subtable",
			"GL_EXT_compiled_vertex_array",
			"GL_EXT_convolution",
			"GL_EXT_coordinate_frame",
			"GL_EXT_copy_texture",
			"GL_EXT_cull_vertex",
			"GL_EXT_depth_bounds_test",
			"GL_EXT_direct_state_access",
			"GL_EXT_draw_buffers2",
			"GL_EXT_draw_instanced",
			"GL_EXT_draw_range_elements",
			"GL_EXT_fog_coord",
			"GL_EXT_framebuffer_blit",
			"GL_EXT_framebuffer_multisample",
			"GL_EXT_framebuffer_multisample_blit_scaled",
			"GL_EXT_framebuffer_object",
			"GL_EXT_framebuffer_sRGB",
			"GL_EXT_geometry_shader4",
			"GL_EXT_gpu_program_parameters",
			"GL_EXT_gpu_shader4",
			"GL_EXT_histogram",
			"GL_EXT_index_array_formats",
			"GL_EXT_index_func",
			"GL_EXT_index_material",
			"GL_EXT_index_texture",
			"GL_EXT_light_texture",
			"GL_EXT_misc_attribute",
			"GL_EXT_multi_draw_arrays",
			"GL_EXT_multisample",
			"GL_EXT_packed_depth_stencil",
			"GL_EXT_packed_float",
			"GL_EXT_packed_pixels",
			"GL_EXT_paletted_texture",
			"GL_EXT_pixel_buffer_object",
			"GL_EXT_pixel_transform",
			"GL_EXT_pixel_transform_color_table",
			"GL_EXT_point_parameters",
			"GL_EXT_polygon_offset",
			"GL_EXT_provoking_vertex",
			"GL_EXT_rescale_normal",
			"GL_EXT_secondary_color",
			"GL_EXT_separate_shader_objects",
			"GL_EXT_separate_specular_color",
			"GL_EXT_shader_image_load_store",
			"GL_EXT_shadow_funcs",
			"GL_EXT_shared_texture_palette",
			"GL_EXT_stencil_clear_tag",
			"GL_EXT_stencil_two_side",
			"GL_EXT_stencil_wrap",
			"GL_EXT_subtexture",
			"GL_EXT_texture",
			"GL_EXT_texture3D",
			"GL_EXT_texture_array",
			"GL_EXT_texture_buffer_object",
			"GL_EXT_texture_compression_latc",
			"GL_EXT_texture_compression_rgtc",
			"GL_EXT_texture_compression_s3tc",
			"GL_EXT_texture_cube_map",
			"GL_EXT_texture_env_add",
			"GL_EXT_texture_env_combine",
			"GL_EXT_texture_env_dot3",
			"GL_EXT_texture_filter_anisotropic",
			"GL_EXT_texture_integer",
			"GL_EXT_texture_lod_bias",
			"GL_EXT_texture_mirror_clamp",
			"GL_EXT_texture_object",
			"GL_EXT_texture_perturb_normal",
			"GL_EXT_texture_shared_exponent",
			"GL_EXT_texture_snorm",
			"GL_EXT_texture_sRGB",
			"GL_EXT_texture_sRGB_decode",
			"GL_EXT_texture_swizzle",
			"GL_EXT_timer_query",
			"GL_EXT_transform_feedback",
			"GL_EXT_vertex_array",
			"GL_EXT_vertex_array_bgra",
			"GL_EXT_vertex_attrib_64bit",
			"GL_EXT_vertex_shader",
			"GL_EXT_vertex_weighting",
			"GL_EXT_x11_sync_object",
			"GL_FfdMaskSGIX",
			"GL_GREMEDY_frame_terminator",
			"GL_GREMEDY_string_marker",
			"GL_HP_convolution_border_modes",
			"GL_HP_image_transform",
			"GL_HP_occlusion_test",
			"GL_HP_texture_lighting",
			"GL_IBM_cull_vertex",
			"GL_IBM_multimode_draw_arrays",
			"GL_IBM_rasterpos_clip",
			"GL_IBM_texture_mirrored_repeat",
			"GL_IBM_vertex_array_lists",
			"GL_INGR_blend_func_separate",
			"GL_INGR_color_clamp",
			"GL_INGR_interlace_read",
			"GL_INGR_palette_buffer",
			"GL_INTEL_parallel_arrays",
			"GL_INTEL_texture_scissor",
			"GL_MESA_pack_invert",
			"GL_MESA_resize_buffers",
			"GL_MESA_window_pos",
			"GL_MESAX_texture_stack",
			"GL_MESA_ycbcr_texture",
			"GL_NV_blend_square",
			"GL_NV_conditional_render",
			"GL_NV_copy_depth_to_color",
			"GL_NV_copy_image",
			"GL_NV_depth_buffer_float",
			"GL_NV_depth_clamp",
			"GL_NV_evaluators",
			"GL_NV_explicit_multisample",
			"GL_NV_fence",
			"GL_NV_float_buffer",
			"GL_NV_fog_distance",
			"GL_NV_fragment_program",
			"GL_NV_fragment_program2",
			"GL_NV_fragment_program4",
			"GL_NV_fragment_program_option",
			"GL_NV_framebuffer_multisample_coverage",
			"GL_NV_geometry_program4",
			"GL_NV_geometry_shader4",
			"GL_NV_gpu_program4",
			"GL_NV_gpu_program5",
			"GL_NV_gpu_shader5",
			"GL_NV_half_float",
			"GL_NV_light_max_exponent",
			"GL_NV_multisample_coverage",
			"GL_NV_multisample_filter_hint",
			"GL_NV_occlusion_query",
			"GL_NV_packed_depth_stencil",
			"GL_NV_parameter_buffer_object",
			"GL_NV_parameter_buffer_object2",
			"GL_NV_pixel_data_range",
			"GL_NV_point_sprite",
			"GL_NV_present_video",
			"GL_NV_primitive_restart",
			"GL_NV_register_combiners",
			"GL_NV_register_combiners2",
			"GL_NV_shader_buffer_load",
			"GL_NV_shader_buffer_store",
			"GL_NV_tessellation_program5",
			"GL_NV_texgen_emboss",
			"GL_NV_texgen_reflection",
			"GL_NV_texture_barrier",
			"GL_NV_texture_compression_vtc",
			"GL_NV_texture_env_combine4",
			"GL_NV_texture_expand_normal",
			"GL_NV_texture_multisample",
			"GL_NV_texture_rectangle",
			"GL_NV_texture_shader",
			"GL_NV_texture_shader2",
			"GL_NV_texture_shader3",
			"GL_NV_transform_feedback",
			"GL_NV_transform_feedback2",
			"GL_NV_vdpau_interop",
			"GL_NV_vertex_array_range",
			"GL_NV_vertex_array_range2",
			"GL_NV_vertex_attrib_integer_64bit",
			"GL_NV_vertex_buffer_unified_memory",
			"GL_NV_vertex_program",
			"GL_NV_vertex_program1_1",
			"GL_NV_vertex_program2",
			"GL_NV_vertex_program2_option",
			"GL_NV_vertex_program3",
			"GL_NV_vertex_program4",
			"GL_NV_video_capture",
			"GL_OES_read_format",
			"GL_OML_interlace",
			"GL_OML_resample",
			"GL_OML_subsample",
			"GL_PGI_misc_hints",
			"GL_PGI_vertex_hints",
			"GL_REND_screen_coordinates",
			"GL_S3_s3tc",
			"GL_SGI_color_matrix",
			"GL_SGI_color_table",
			"GL_SGI_depth_pass_instrument",
			"GL_SGIS_detail_texture",
			"GL_SGIS_fog_function",
			"GL_SGIS_generate_mipmap",
			"GL_SGIS_multisample",
			"GL_SGIS_pixel_texture",
			"GL_SGIS_point_line_texgen",
			"GL_SGIS_point_parameters",
			"GL_SGIS_sharpen_texture",
			"GL_SGIS_texture4D",
			"GL_SGIS_texture_border_clamp",
			"GL_SGIS_texture_color_mask",
			"GL_SGIS_texture_edge_clamp",
			"GL_SGIS_texture_filter4",
			"GL_SGIS_texture_lod",
			"GL_SGIS_texture_select",
			"GL_SGI_texture_color_table",
			"GL_SGIX_async",
			"GL_SGIX_async_histogram",
			"GL_SGIX_async_pixel",
			"GL_SGIX_blend_alpha_minmax",
			"GL_SGIX_calligraphic_fragment",
			"GL_SGIX_clipmap",
			"GL_SGIX_convolution_accuracy",
			"GL_SGIX_depth_pass_instrument",
			"GL_SGIX_depth_texture",
			"GL_SGIX_flush_raster",
			"GL_SGIX_fog_offset",
			"GL_SGIX_fog_scale",
			"GL_SGIX_fragment_lighting",
			"GL_SGIX_framezoom",
			"GL_SGIX_igloo_interface",
			"GL_SGIX_impact_pixel_texture",
			"GL_SGIX_instruments",
			"GL_SGIX_interlace",
			"GL_SGIX_ir_instrument1",
			"GL_SGIX_list_priority",
			"GL_SGIX_pixel_texture",
			"GL_SGIX_pixel_tiles",
			"GL_SGIX_polynomial_ffd",
			"GL_SGIX_reference_plane",
			"GL_SGIX_resample",
			"GL_SGIX_scalebias_hint",
			"GL_SGIX_shadow",
			"GL_SGIX_shadow_ambient",
			"GL_SGIX_sprite",
			"GL_SGIX_subsample",
			"GL_SGIX_tag_sample_buffer",
			"GL_SGIX_texture_add_env",
			"GL_SGIX_texture_coordinate_clamp",
			"GL_SGIX_texture_lod_bias",
			"GL_SGIX_texture_multi_buffer",
			"GL_SGIX_texture_scale_bias",
			"GL_SGIX_texture_select",
			"GL_SGIX_vertex_preclip",
			"GL_SGIX_ycrcb",
			"GL_SGIX_ycrcba",
			"GL_SGIX_ycrcb_subsample",
			"GL_SUN_convolution_border_modes",
			"GL_SUN_global_alpha",
			"GL_SUN_mesh_array",
			"GL_SUN_slice_accum",
			"GL_SUN_triangle_list",
			"GL_SUN_vertex",
			"GL_SUNX_constant_data",
			"GL_WIN_phong_shading",
			"GL_WIN_specular_fog",
			// unofficial stuff
			"GL_NVX_gpu_memory_info"
		};


		class COpenGLExtensionHandler
		{
		public:
			enum EOpenGLFeatures {
				SAPPHIRE_3DFX_multisample = 0,
				SAPPHIRE_3DFX_tbuffer,
				SAPPHIRE_3DFX_texture_compression_FXT1,
				SAPPHIRE_AMD_blend_minmax_factor,
				SAPPHIRE_AMD_conservative_depth,
				SAPPHIRE_AMD_debug_output,
				SAPPHIRE_AMD_depth_clamp_separate,
				SAPPHIRE_AMD_draw_buffers_blend,
				SAPPHIRE_AMD_multi_draw_indirect,
				SAPPHIRE_AMD_name_gen_delete,
				SAPPHIRE_AMD_performance_monitor,
				SAPPHIRE_AMD_sample_positions,
				SAPPHIRE_AMD_seamless_cubemap_per_texture,
				SAPPHIRE_AMD_shader_stencil_export,
				SAPPHIRE_AMD_texture_texture4,
				SAPPHIRE_AMD_transform_feedback3_lines_triangles,
				SAPPHIRE_AMD_vertex_shader_tesselator,
				SAPPHIRE_APPLE_aux_depth_stencil,
				SAPPHIRE_APPLE_client_storage,
				SAPPHIRE_APPLE_element_array,
				SAPPHIRE_APPLE_fence,
				SAPPHIRE_APPLE_float_pixels,
				SAPPHIRE_APPLE_flush_buffer_range,
				SAPPHIRE_APPLE_object_purgeable,
				SAPPHIRE_APPLE_rgb_422,
				SAPPHIRE_APPLE_row_bytes,
				SAPPHIRE_APPLE_specular_vector,
				SAPPHIRE_APPLE_texture_range,
				SAPPHIRE_APPLE_transform_hint,
				SAPPHIRE_APPLE_vertex_array_object,
				SAPPHIRE_APPLE_vertex_array_range,
				SAPPHIRE_APPLE_vertex_program_evaluators,
				SAPPHIRE_APPLE_ycbcr_422,
				SAPPHIRE_ARB_base_instance,
				SAPPHIRE_ARB_blend_func_extended,
				SAPPHIRE_ARB_cl_event,
				SAPPHIRE_ARB_color_buffer_float,
				SAPPHIRE_ARB_compatibility,
				SAPPHIRE_ARB_compressed_texture_pixel_storage,
				SAPPHIRE_ARB_conservative_depth,
				SAPPHIRE_ARB_copy_buffer,
				SAPPHIRE_ARB_debug_output,
				SAPPHIRE_ARB_depth_buffer_float,
				SAPPHIRE_ARB_depth_clamp,
				SAPPHIRE_ARB_depth_texture,
				SAPPHIRE_ARB_draw_buffers,
				SAPPHIRE_ARB_draw_buffers_blend,
				SAPPHIRE_ARB_draw_elements_base_vertex,
				SAPPHIRE_ARB_draw_indirect,
				SAPPHIRE_ARB_draw_instanced,
				SAPPHIRE_ARB_ES2_compatibility,
				SAPPHIRE_ARB_explicit_attrib_location,
				SAPPHIRE_ARB_fragment_coord_conventions,
				SAPPHIRE_ARB_fragment_program,
				SAPPHIRE_ARB_fragment_program_shadow,
				SAPPHIRE_ARB_fragment_shader,
				SAPPHIRE_ARB_framebuffer_object,
				SAPPHIRE_ARB_framebuffer_sRGB,
				SAPPHIRE_ARB_geometry_shader4,
				SAPPHIRE_ARB_get_program_binary,
				SAPPHIRE_ARB_gpu_shader5,
				SAPPHIRE_ARB_gpu_shader_fp64,
				SAPPHIRE_ARB_half_float_pixel,
				SAPPHIRE_ARB_half_float_vertex,
				SAPPHIRE_ARB_imaging,
				SAPPHIRE_ARB_instanced_arrays,
				SAPPHIRE_ARB_internalformat_query,
				SAPPHIRE_ARB_map_buffer_alignment,
				SAPPHIRE_ARB_map_buffer_range,
				SAPPHIRE_ARB_matrix_palette,
				SAPPHIRE_ARB_multisample,
				SAPPHIRE_ARB_multitexture,
				SAPPHIRE_ARB_occlusion_query,
				SAPPHIRE_ARB_occlusion_query2,
				SAPPHIRE_ARB_pixel_buffer_object,
				SAPPHIRE_ARB_point_parameters,
				SAPPHIRE_ARB_point_sprite,
				SAPPHIRE_ARB_provoking_vertex,
				SAPPHIRE_ARB_robustness,
				SAPPHIRE_ARB_sample_shading,
				SAPPHIRE_ARB_sampler_objects,
				SAPPHIRE_ARB_seamless_cube_map,
				SAPPHIRE_ARB_separate_shader_objects,
				SAPPHIRE_ARB_shader_atomic_counters,
				SAPPHIRE_ARB_shader_bit_encoding,
				SAPPHIRE_ARB_shader_image_load_store,
				SAPPHIRE_ARB_shader_objects,
				SAPPHIRE_ARB_shader_precision,
				SAPPHIRE_ARB_shader_stencil_export,
				SAPPHIRE_ARB_shader_subroutine,
				SAPPHIRE_ARB_shader_texture_lod,
				SAPPHIRE_ARB_shading_language_100,
				SAPPHIRE_ARB_shading_language_420pack,
				SAPPHIRE_ARB_shading_language_include,
				SAPPHIRE_ARB_shading_language_packing,
				SAPPHIRE_ARB_shadow,
				SAPPHIRE_ARB_shadow_ambient,
				SAPPHIRE_ARB_sync,
				SAPPHIRE_ARB_tessellation_shader,
				SAPPHIRE_ARB_texture_border_clamp,
				SAPPHIRE_ARB_texture_buffer_object,
				SAPPHIRE_ARB_texture_buffer_object_rgb32,
				SAPPHIRE_ARB_texture_compression,
				SAPPHIRE_ARB_texture_compression_bptc,
				SAPPHIRE_ARB_texture_compression_rgtc,
				SAPPHIRE_ARB_texture_cube_map,
				SAPPHIRE_ARB_texture_cube_map_array,
				SAPPHIRE_ARB_texture_env_add,
				SAPPHIRE_ARB_texture_env_combine,
				SAPPHIRE_ARB_texture_env_crossbar,
				SAPPHIRE_ARB_texture_env_dot3,
				SAPPHIRE_ARB_texture_float,
				SAPPHIRE_ARB_texture_gather,
				SAPPHIRE_ARB_texture_mirrored_repeat,
				SAPPHIRE_ARB_texture_multisample,
				SAPPHIRE_ARB_texture_non_power_of_two,
				SAPPHIRE_ARB_texture_query_lod,
				SAPPHIRE_ARB_texture_rectangle,
				SAPPHIRE_ARB_texture_rg,
				SAPPHIRE_ARB_texture_rgb10_a2ui,
				SAPPHIRE_ARB_texture_storage,
				SAPPHIRE_ARB_texture_swizzle,
				SAPPHIRE_ARB_timer_query,
				SAPPHIRE_ARB_transform_feedback2,
				SAPPHIRE_ARB_transform_feedback3,
				SAPPHIRE_ARB_transform_feedback_instanced,
				SAPPHIRE_ARB_transpose_matrix,
				SAPPHIRE_ARB_uniform_buffer_object,
				SAPPHIRE_ARB_vertex_array_bgra,
				SAPPHIRE_ARB_vertex_array_object,
				SAPPHIRE_ARB_vertex_attrib_64bit,
				SAPPHIRE_ARB_vertex_blend,
				SAPPHIRE_ARB_vertex_buffer_object,
				SAPPHIRE_ARB_vertex_program,
				SAPPHIRE_ARB_vertex_shader,
				SAPPHIRE_ARB_vertex_type_2_10_10_10_rev,
				SAPPHIRE_ARB_viewport_array,
				SAPPHIRE_ARB_window_pos,
				SAPPHIRE_ATI_draw_buffers,
				SAPPHIRE_ATI_element_array,
				SAPPHIRE_ATI_envmap_bumpmap,
				SAPPHIRE_ATI_fragment_shader,
				SAPPHIRE_ATI_map_object_buffer,
				SAPPHIRE_ATI_meminfo,
				SAPPHIRE_ATI_pixel_format_float,
				SAPPHIRE_ATI_pn_triangles,
				SAPPHIRE_ATI_separate_stencil,
				SAPPHIRE_ATI_text_fragment_shader,
				SAPPHIRE_ATI_texture_env_combine3,
				SAPPHIRE_ATI_texture_float,
				SAPPHIRE_ATI_texture_mirror_once,
				SAPPHIRE_ATI_vertex_array_object,
				SAPPHIRE_ATI_vertex_attrib_array_object,
				SAPPHIRE_ATI_vertex_streams,
				SAPPHIRE_EXT_422_pixels,
				SAPPHIRE_EXT_abgr,
				SAPPHIRE_EXT_bgra,
				SAPPHIRE_EXT_bindable_uniform,
				SAPPHIRE_EXT_blend_color,
				SAPPHIRE_EXT_blend_equation_separate,
				SAPPHIRE_EXT_blend_func_separate,
				SAPPHIRE_EXT_blend_logic_op,
				SAPPHIRE_EXT_blend_minmax,
				SAPPHIRE_EXT_blend_subtract,
				SAPPHIRE_EXT_clip_volume_hint,
				SAPPHIRE_EXT_cmyka,
				SAPPHIRE_EXT_color_subtable,
				SAPPHIRE_EXT_compiled_vertex_array,
				SAPPHIRE_EXT_convolution,
				SAPPHIRE_EXT_coordinate_frame,
				SAPPHIRE_EXT_copy_texture,
				SAPPHIRE_EXT_cull_vertex,
				SAPPHIRE_EXT_depth_bounds_test,
				SAPPHIRE_EXT_direct_state_access,
				SAPPHIRE_EXT_draw_buffers2,
				SAPPHIRE_EXT_draw_instanced,
				SAPPHIRE_EXT_draw_range_elements,
				SAPPHIRE_EXT_fog_coord,
				SAPPHIRE_EXT_framebuffer_blit,
				SAPPHIRE_EXT_framebuffer_multisample,
				SAPPHIRE_EXT_framebuffer_multisample_blit_scaled,
				SAPPHIRE_EXT_framebuffer_object,
				SAPPHIRE_EXT_framebuffer_sRGB,
				SAPPHIRE_EXT_geometry_shader4,
				SAPPHIRE_EXT_gpu_program_parameters,
				SAPPHIRE_EXT_gpu_shader4,
				SAPPHIRE_EXT_histogram,
				SAPPHIRE_EXT_index_array_formats,
				SAPPHIRE_EXT_index_func,
				SAPPHIRE_EXT_index_material,
				SAPPHIRE_EXT_index_texture,
				SAPPHIRE_EXT_light_texture,
				SAPPHIRE_EXT_misc_attribute,
				SAPPHIRE_EXT_multi_draw_arrays,
				SAPPHIRE_EXT_multisample,
				SAPPHIRE_EXT_packed_depth_stencil,
				SAPPHIRE_EXT_packed_float,
				SAPPHIRE_EXT_packed_pixels,
				SAPPHIRE_EXT_paletted_texture,
				SAPPHIRE_EXT_pixel_buffer_object,
				SAPPHIRE_EXT_pixel_transform,
				SAPPHIRE_EXT_pixel_transform_color_table,
				SAPPHIRE_EXT_point_parameters,
				SAPPHIRE_EXT_polygon_offset,
				SAPPHIRE_EXT_provoking_vertex,
				SAPPHIRE_EXT_rescale_normal,
				SAPPHIRE_EXT_secondary_color,
				SAPPHIRE_EXT_separate_shader_objects,
				SAPPHIRE_EXT_separate_specular_color,
				SAPPHIRE_EXT_shader_image_load_store,
				SAPPHIRE_EXT_shadow_funcs,
				SAPPHIRE_EXT_shared_texture_palette,
				SAPPHIRE_EXT_stencil_clear_tag,
				SAPPHIRE_EXT_stencil_two_side,
				SAPPHIRE_EXT_stencil_wrap,
				SAPPHIRE_EXT_subtexture,
				SAPPHIRE_EXT_texture,
				SAPPHIRE_EXT_texture3D,
				SAPPHIRE_EXT_texture_array,
				SAPPHIRE_EXT_texture_buffer_object,
				SAPPHIRE_EXT_texture_compression_latc,
				SAPPHIRE_EXT_texture_compression_rgtc,
				SAPPHIRE_EXT_texture_compression_s3tc,
				SAPPHIRE_EXT_texture_cube_map,
				SAPPHIRE_EXT_texture_env_add,
				SAPPHIRE_EXT_texture_env_combine,
				SAPPHIRE_EXT_texture_env_dot3,
				SAPPHIRE_EXT_texture_filter_anisotropic,
				SAPPHIRE_EXT_texture_integer,
				SAPPHIRE_EXT_texture_lod_bias,
				SAPPHIRE_EXT_texture_mirror_clamp,
				SAPPHIRE_EXT_texture_object,
				SAPPHIRE_EXT_texture_perturb_normal,
				SAPPHIRE_EXT_texture_shared_exponent,
				SAPPHIRE_EXT_texture_snorm,
				SAPPHIRE_EXT_texture_sRGB,
				SAPPHIRE_EXT_texture_sRGB_decode,
				SAPPHIRE_EXT_texture_swizzle,
				SAPPHIRE_EXT_timer_query,
				SAPPHIRE_EXT_transform_feedback,
				SAPPHIRE_EXT_vertex_array,
				SAPPHIRE_EXT_vertex_array_bgra,
				SAPPHIRE_EXT_vertex_attrib_64bit,
				SAPPHIRE_EXT_vertex_shader,
				SAPPHIRE_EXT_vertex_weighting,
				SAPPHIRE_EXT_x11_sync_object,
				SAPPHIRE_FfdMaskSGIX,
				SAPPHIRE_GREMEDY_frame_terminator,
				SAPPHIRE_GREMEDY_string_marker,
				SAPPHIRE_HP_convolution_border_modes,
				SAPPHIRE_HP_image_transform,
				SAPPHIRE_HP_occlusion_test,
				SAPPHIRE_HP_texture_lighting,
				SAPPHIRE_IBM_cull_vertex,
				SAPPHIRE_IBM_multimode_draw_arrays,
				SAPPHIRE_IBM_rasterpos_clip,
				SAPPHIRE_IBM_texture_mirrored_repeat,
				SAPPHIRE_IBM_vertex_array_lists,
				SAPPHIRE_INGR_blend_func_separate,
				SAPPHIRE_INGR_color_clamp,
				SAPPHIRE_INGR_interlace_read,
				SAPPHIRE_INGR_palette_buffer,
				SAPPHIRE_INTEL_parallel_arrays,
				SAPPHIRE_INTEL_texture_scissor,
				SAPPHIRE_MESA_pack_invert,
				SAPPHIRE_MESA_resize_buffers,
				SAPPHIRE_MESA_window_pos,
				SAPPHIRE_MESAX_texture_stack,
				SAPPHIRE_MESA_ycbcr_texture,
				SAPPHIRE_NV_blend_square,
				SAPPHIRE_NV_conditional_render,
				SAPPHIRE_NV_copy_depth_to_color,
				SAPPHIRE_NV_copy_image,
				SAPPHIRE_NV_depth_buffer_float,
				SAPPHIRE_NV_depth_clamp,
				SAPPHIRE_NV_evaluators,
				SAPPHIRE_NV_explicit_multisample,
				SAPPHIRE_NV_fence,
				SAPPHIRE_NV_float_buffer,
				SAPPHIRE_NV_fog_distance,
				SAPPHIRE_NV_fragment_program,
				SAPPHIRE_NV_fragment_program2,
				SAPPHIRE_NV_fragment_program4,
				SAPPHIRE_NV_fragment_program_option,
				SAPPHIRE_NV_framebuffer_multisample_coverage,
				SAPPHIRE_NV_geometry_program4,
				SAPPHIRE_NV_geometry_shader4,
				SAPPHIRE_NV_gpu_program4,
				SAPPHIRE_NV_gpu_program5,
				SAPPHIRE_NV_gpu_shader5,
				SAPPHIRE_NV_half_float,
				SAPPHIRE_NV_light_max_exponent,
				SAPPHIRE_NV_multisample_coverage,
				SAPPHIRE_NV_multisample_filter_hint,
				SAPPHIRE_NV_occlusion_query,
				SAPPHIRE_NV_packed_depth_stencil,
				SAPPHIRE_NV_parameter_buffer_object,
				SAPPHIRE_NV_parameter_buffer_object2,
				SAPPHIRE_NV_pixel_data_range,
				SAPPHIRE_NV_point_sprite,
				SAPPHIRE_NV_present_video,
				SAPPHIRE_NV_primitive_restart,
				SAPPHIRE_NV_register_combiners,
				SAPPHIRE_NV_register_combiners2,
				SAPPHIRE_NV_shader_buffer_load,
				SAPPHIRE_NV_shader_buffer_store,
				SAPPHIRE_NV_tessellation_program5,
				SAPPHIRE_NV_texgen_emboss,
				SAPPHIRE_NV_texgen_reflection,
				SAPPHIRE_NV_texture_barrier,
				SAPPHIRE_NV_texture_compression_vtc,
				SAPPHIRE_NV_texture_env_combine4,
				SAPPHIRE_NV_texture_expand_normal,
				SAPPHIRE_NV_texture_multisample,
				SAPPHIRE_NV_texture_rectangle,
				SAPPHIRE_NV_texture_shader,
				SAPPHIRE_NV_texture_shader2,
				SAPPHIRE_NV_texture_shader3,
				SAPPHIRE_NV_transform_feedback,
				SAPPHIRE_NV_transform_feedback2,
				SAPPHIRE_NV_vdpau_interop,
				SAPPHIRE_NV_vertex_array_range,
				SAPPHIRE_NV_vertex_array_range2,
				SAPPHIRE_NV_vertex_attrib_integer_64bit,
				SAPPHIRE_NV_vertex_buffer_unified_memory,
				SAPPHIRE_NV_vertex_program,
				SAPPHIRE_NV_vertex_program1_1,
				SAPPHIRE_NV_vertex_program2,
				SAPPHIRE_NV_vertex_program2_option,
				SAPPHIRE_NV_vertex_program3,
				SAPPHIRE_NV_vertex_program4,
				SAPPHIRE_NV_video_capture,
				SAPPHIRE_OES_read_format,
				SAPPHIRE_OML_interlace,
				SAPPHIRE_OML_resample,
				SAPPHIRE_OML_subsample,
				SAPPHIRE_PGI_misc_hints,
				SAPPHIRE_PGI_vertex_hints,
				SAPPHIRE_REND_screen_coordinates,
				SAPPHIRE_S3_s3tc,
				SAPPHIRE_SGI_color_matrix,
				SAPPHIRE_SGI_color_table,
				SAPPHIRE_SGI_depth_pass_instrument,
				SAPPHIRE_SGIS_detail_texture,
				SAPPHIRE_SGIS_fog_function,
				SAPPHIRE_SGIS_generate_mipmap,
				SAPPHIRE_SGIS_multisample,
				SAPPHIRE_SGIS_pixel_texture,
				SAPPHIRE_SGIS_point_line_texgen,
				SAPPHIRE_SGIS_point_parameters,
				SAPPHIRE_SGIS_sharpen_texture,
				SAPPHIRE_SGIS_texture4D,
				SAPPHIRE_SGIS_texture_border_clamp,
				SAPPHIRE_SGIS_texture_color_mask,
				SAPPHIRE_SGIS_texture_edge_clamp,
				SAPPHIRE_SGIS_texture_filter4,
				SAPPHIRE_SGIS_texture_lod,
				SAPPHIRE_SGIS_texture_select,
				SAPPHIRE_SGI_texture_color_table,
				SAPPHIRE_SGIX_async,
				SAPPHIRE_SGIX_async_histogram,
				SAPPHIRE_SGIX_async_pixel,
				SAPPHIRE_SGIX_blend_alpha_minmax,
				SAPPHIRE_SGIX_calligraphic_fragment,
				SAPPHIRE_SGIX_clipmap,
				SAPPHIRE_SGIX_convolution_accuracy,
				SAPPHIRE_SGIX_depth_pass_instrument,
				SAPPHIRE_SGIX_depth_texture,
				SAPPHIRE_SGIX_flush_raster,
				SAPPHIRE_SGIX_fog_offset,
				SAPPHIRE_SGIX_fog_scale,
				SAPPHIRE_SGIX_fragment_lighting,
				SAPPHIRE_SGIX_framezoom,
				SAPPHIRE_SGIX_igloo_interface,
				SAPPHIRE_SGIX_impact_pixel_texture,
				SAPPHIRE_SGIX_instruments,
				SAPPHIRE_SGIX_interlace,
				SAPPHIRE_SGIX_ir_instrument1,
				SAPPHIRE_SGIX_list_priority,
				SAPPHIRE_SGIX_pixel_texture,
				SAPPHIRE_SGIX_pixel_tiles,
				SAPPHIRE_SGIX_polynomial_ffd,
				SAPPHIRE_SGIX_reference_plane,
				SAPPHIRE_SGIX_resample,
				SAPPHIRE_SGIX_scalebias_hint,
				SAPPHIRE_SGIX_shadow,
				SAPPHIRE_SGIX_shadow_ambient,
				SAPPHIRE_SGIX_sprite,
				SAPPHIRE_SGIX_subsample,
				SAPPHIRE_SGIX_tag_sample_buffer,
				SAPPHIRE_SGIX_texture_add_env,
				SAPPHIRE_SGIX_texture_coordinate_clamp,
				SAPPHIRE_SGIX_texture_lod_bias,
				SAPPHIRE_SGIX_texture_multi_buffer,
				SAPPHIRE_SGIX_texture_scale_bias,
				SAPPHIRE_SGIX_texture_select,
				SAPPHIRE_SGIX_vertex_preclip,
				SAPPHIRE_SGIX_ycrcb,
				SAPPHIRE_SGIX_ycrcba,
				SAPPHIRE_SGIX_ycrcb_subsample,
				SAPPHIRE_SUN_convolution_border_modes,
				SAPPHIRE_SUN_global_alpha,
				SAPPHIRE_SUN_mesh_array,
				SAPPHIRE_SUN_slice_accum,
				SAPPHIRE_SUN_triangle_list,
				SAPPHIRE_SUN_vertex,
				SAPPHIRE_SUNX_constant_data,
				SAPPHIRE_WIN_phong_shading,
				SAPPHIRE_WIN_specular_fog,
				SAPPHIRE_NVX_gpu_memory_info,
				SAPPHIRE_OpenGL_Feature_Count
			};


			// constructor
			COpenGLExtensionHandler();

			// deferred initialization
			void initExtensions(bool stencilBuffer);

			//! queries the features of the driver, returns true if feature is available
			bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const;

			//! queries the features of the driver, returns true if feature is available
			bool queryOpenGLFeature(EOpenGLFeatures feature) const
			{
				return FeatureAvailable[feature];
			}

			//! show all features with availablity
			void dump() const;

			void dumpFramebufferFormats() const;

			// Some variables for properties
			bool StencilBuffer;
			bool MultiTextureExtension;
			bool TextureCompressionExtension;

			// Some non-boolean properties
			//! Maxmimum texture layers supported by the fixed pipeline
			UINT8 MaxSupportedTextures;
			//! Maxmimum texture layers supported by the engine
			UINT8 MaxTextureUnits;
			//! Maximum hardware lights supported
			UINT8 MaxLights;
			//! Maximal Anisotropy
			UINT8 MaxAnisotropy;
			//! Number of user clipplanes
			UINT8 MaxUserClipPlanes;
			//! Number of auxiliary buffers
			UINT8 MaxAuxBuffers;
			//! Number of rendertargets available as MRTs
			UINT8 MaxMultipleRenderTargets;
			//! Optimal number of indices per meshbuffer
			UINT32 MaxIndices;
			//! Maximal texture dimension
			UINT32 MaxTextureSize;
			//! Maximal vertices handled by geometry shaders
			UINT32 MaxGeometryVerticesOut;
			//! Maximal LOD Bias
			float32 MaxTextureLODBias;
			//! Minimal and maximal supported thickness for lines without smoothing
			GLfloat DimAliasedLine[2];
			//! Minimal and maximal supported thickness for points without smoothing
			GLfloat DimAliasedPoint[2];
			//! Minimal and maximal supported thickness for lines with smoothing
			GLfloat DimSmoothedLine[2];
			//! Minimal and maximal supported thickness for points with smoothing
			GLfloat DimSmoothedPoint[2];

			//! OpenGL version as Integer: 100*Major+Minor, i.e. 2.1 becomes 201
			UINT16 Version;
			//! GLSL version as Integer: 100*Major+Minor
			UINT16 ShaderLanguageVersion;

			bool OcclusionQuerySupport;

			// public access to the (loaded) extensions.
			// general functions
			void extGlActiveTexture(GLenum texture);
			void extGlClientActiveTexture(GLenum texture);
			void extGlPointParameterf(GLint loc, GLfloat f);
			void extGlPointParameterfv(GLint loc, const GLfloat *v);
			void extGlStencilFuncSeparate(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
			void extGlStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
			void extGlCompressedTexImage2D(GLenum target, GLint level,
				GLenum internalformat, GLsizei width, GLsizei height,
				GLint border, GLsizei imageSize, const void* data);

			// shader programming
			void extGlGenPrograms(GLsizei n, GLuint *programs);
			void extGlBindProgram(GLenum target, GLuint program);
			void extGlProgramString(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
			void extGlLoadProgram(GLenum target, GLuint id, GLsizei len, const GLubyte *string);
			void extGlDeletePrograms(GLsizei n, const GLuint *programs);
			void extGlProgramLocalParameter4fv(GLenum, GLuint, const GLfloat *);
			GLhandleARB extGlCreateShaderObject(GLenum shaderType);
			GLuint extGlCreateShader(GLenum shaderType);
			// note: Due to the type confusion between shader_objects and OpenGL 2.0
			// we have to add the ARB extension for proper method definitions in case
			// that handleARB and uint are the same type
			void extGlShaderSourceARB(GLhandleARB shader, GLsizei numOfStrings, const char **strings, const GLint *lenOfStrings);
			void extGlShaderSource(GLuint shader, GLsizei numOfStrings, const char **strings, const GLint *lenOfStrings);
			// note: Due to the type confusion between shader_objects and OpenGL 2.0
			// we have to add the ARB extension for proper method definitions in case
			// that handleARB and uint are the same type
			void extGlCompileShaderARB(GLhandleARB shader);
			void extGlCompileShader(GLuint shader);
			GLhandleARB extGlCreateProgramObject(void);
			GLuint extGlCreateProgram(void);
			void extGlAttachObject(GLhandleARB program, GLhandleARB shader);
			void extGlAttachShader(GLuint program, GLuint shader);
			void extGlLinkProgramARB(GLhandleARB program);
			// note: Due to the type confusion between shader_objects and OpenGL 2.0
			// we have to add the ARB extension for proper method definitions in case
			// that handleARB and uint are the same type
			void extGlLinkProgram(GLuint program);
			void extGlUseProgramObject(GLhandleARB prog);
			void extGlUseProgram(GLuint prog);
			void extGlDeleteObject(GLhandleARB object);
			void extGlDeleteProgram(GLuint object);
			void extGlDeleteShader(GLuint shader);
			void extGlGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
			void extGlGetAttachedObjects(GLhandleARB program, GLsizei maxcount, GLsizei* count, GLhandleARB* shaders);
			void extGlGetInfoLog(GLhandleARB object, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
			void extGlGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
			void extGlGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
			void extGlGetObjectParameteriv(GLhandleARB object, GLenum type, GLint *param);
			void extGlGetShaderiv(GLuint shader, GLenum type, GLint *param);
			void extGlGetProgramiv(GLuint program, GLenum type, GLint *param);
			GLint extGlGetUniformLocationARB(GLhandleARB program, const char *name);
			GLint extGlGetUniformLocation(GLuint program, const char *name);
			void extGlUniform1fv(GLint loc, GLsizei count, const GLfloat *v);
			void extGlUniform2fv(GLint loc, GLsizei count, const GLfloat *v);
			void extGlUniform3fv(GLint loc, GLsizei count, const GLfloat *v);
			void extGlUniform4fv(GLint loc, GLsizei count, const GLfloat *v);
			void extGlUniform1bv(GLint loc, GLsizei count, const bool *v);
			void extGlUniform2bv(GLint loc, GLsizei count, const bool *v);
			void extGlUniform3bv(GLint loc, GLsizei count, const bool *v);
			void extGlUniform4bv(GLint loc, GLsizei count, const bool *v);
			void extGlUniform1iv(GLint loc, GLsizei count, const GLint *v);
			void extGlUniform2iv(GLint loc, GLsizei count, const GLint *v);
			void extGlUniform3iv(GLint loc, GLsizei count, const GLint *v);
			void extGlUniform4iv(GLint loc, GLsizei count, const GLint *v);
			void extGlUniformMatrix2fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v);
			void extGlUniformMatrix3fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v);
			void extGlUniformMatrix4fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v);
			void extGlGetActiveUniformARB(GLhandleARB program, GLuint index, GLsizei maxlength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
			void extGlGetActiveUniform(GLuint program, GLuint index, GLsizei maxlength, GLsizei *length, GLint *size, GLenum *type, GLchar *name);

			// framebuffer objects
			void extGlBindFramebuffer(GLenum target, GLuint framebuffer);
			void extGlDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
			void extGlGenFramebuffers(GLsizei n, GLuint *framebuffers);
			GLenum extGlCheckFramebufferStatus(GLenum target);
			void extGlFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
			void extGlBindRenderbuffer(GLenum target, GLuint renderbuffer);
			void extGlDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
			void extGlGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
			void extGlRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
			void extGlFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
			void extGlGenerateMipmap(GLenum target);
			void extGlActiveStencilFace(GLenum face);
			void extGlDrawBuffers(GLsizei n, const GLenum *bufs);

			// vertex buffer object
			void extGlGenBuffers(GLsizei n, GLuint *buffers);
			void extGlBindBuffer(GLenum target, GLuint buffer);
			void extGlBufferData(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
			void extGlDeleteBuffers(GLsizei n, const GLuint *buffers);
			void extGlBufferSubData(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
			void extGlGetBufferSubData(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
			void *extGlMapBuffer(GLenum target, GLenum access);
			GLboolean extGlUnmapBuffer(GLenum target);
			GLboolean extGlIsBuffer(GLuint buffer);
			void extGlGetBufferParameteriv(GLenum target, GLenum pname, GLint *params);
			void extGlGetBufferPointerv(GLenum target, GLenum pname, GLvoid **params);
			void extGlProvokingVertex(GLenum mode);
			void extGlColorMaskIndexed(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
			void extGlEnableIndexed(GLenum target, GLuint index);
			void extGlDisableIndexed(GLenum target, GLuint index);
			void extGlBlendFuncIndexed(GLuint buf, GLenum src, GLenum dst);
			void extGlBlendEquationIndexed(GLuint buf, GLenum mode);
			void extGlProgramParameteri(GLuint program, GLenum pname, GLint value);

			// occlusion query
			void extGlGenQueries(GLsizei n, GLuint *ids);
			void extGlDeleteQueries(GLsizei n, const GLuint *ids);
			GLboolean extGlIsQuery(GLuint id);
			void extGlBeginQuery(GLenum target, GLuint id);
			void extGlEndQuery(GLenum target);
			void extGlGetQueryiv(GLenum target, GLenum pname, GLint *params);
			void extGlGetQueryObjectiv(GLuint id, GLenum pname, GLint *params);
			void extGlGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params);

			// generic vsync setting method for several extensions
			void extGlSwapInterval(int interval);

			// blend operations
			void extGlBlendEquation(GLenum mode);

			// the global feature array
			bool FeatureAvailable[SAPPHIRE_OpenGL_Feature_Count];

		protected:
#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
			PFNGLACTIVETEXTUREARBPROC pGlActiveTextureARB;
			PFNGLCLIENTACTIVETEXTUREARBPROC	pGlClientActiveTextureARB;
			PFNGLGENPROGRAMSARBPROC pGlGenProgramsARB;
			PFNGLGENPROGRAMSNVPROC pGlGenProgramsNV;
			PFNGLBINDPROGRAMARBPROC pGlBindProgramARB;
			PFNGLBINDPROGRAMNVPROC pGlBindProgramNV;
			PFNGLDELETEPROGRAMSARBPROC pGlDeleteProgramsARB;
			PFNGLDELETEPROGRAMSNVPROC pGlDeleteProgramsNV;
			PFNGLPROGRAMSTRINGARBPROC pGlProgramStringARB;
			PFNGLLOADPROGRAMNVPROC pGlLoadProgramNV;
			PFNGLPROGRAMLOCALPARAMETER4FVARBPROC pGlProgramLocalParameter4fvARB;
			PFNGLCREATESHADEROBJECTARBPROC pGlCreateShaderObjectARB;
			PFNGLSHADERSOURCEARBPROC pGlShaderSourceARB;
			PFNGLCOMPILESHADERARBPROC pGlCompileShaderARB;
			PFNGLCREATEPROGRAMOBJECTARBPROC pGlCreateProgramObjectARB;
			PFNGLATTACHOBJECTARBPROC pGlAttachObjectARB;
			PFNGLLINKPROGRAMARBPROC pGlLinkProgramARB;
			PFNGLUSEPROGRAMOBJECTARBPROC pGlUseProgramObjectARB;
			PFNGLDELETEOBJECTARBPROC pGlDeleteObjectARB;
			PFNGLCREATEPROGRAMPROC pGlCreateProgram;
			PFNGLUSEPROGRAMPROC pGlUseProgram;
			PFNGLDELETEPROGRAMPROC pGlDeleteProgram;
			PFNGLDELETESHADERPROC pGlDeleteShader;
			PFNGLGETATTACHEDOBJECTSARBPROC pGlGetAttachedObjectsARB;
			PFNGLGETATTACHEDSHADERSPROC pGlGetAttachedShaders;
			PFNGLCREATESHADERPROC pGlCreateShader;
			PFNGLSHADERSOURCEPROC pGlShaderSource;
			PFNGLCOMPILESHADERPROC pGlCompileShader;
			PFNGLATTACHSHADERPROC pGlAttachShader;
			PFNGLLINKPROGRAMPROC pGlLinkProgram;
			PFNGLGETINFOLOGARBPROC pGlGetInfoLogARB;
			PFNGLGETSHADERINFOLOGPROC pGlGetShaderInfoLog;
			PFNGLGETPROGRAMINFOLOGPROC pGlGetProgramInfoLog;
			PFNGLGETOBJECTPARAMETERIVARBPROC pGlGetObjectParameterivARB;
			PFNGLGETSHADERIVPROC pGlGetShaderiv;
			PFNGLGETSHADERIVPROC pGlGetProgramiv;
			PFNGLGETUNIFORMLOCATIONARBPROC pGlGetUniformLocationARB;
			PFNGLGETUNIFORMLOCATIONPROC pGlGetUniformLocation;
			PFNGLUNIFORM1FVARBPROC pGlUniform1fvARB;
			PFNGLUNIFORM2FVARBPROC pGlUniform2fvARB;
			PFNGLUNIFORM3FVARBPROC pGlUniform3fvARB;
			PFNGLUNIFORM4FVARBPROC pGlUniform4fvARB;
			PFNGLUNIFORM1IVARBPROC pGlUniform1ivARB;
			PFNGLUNIFORM2IVARBPROC pGlUniform2ivARB;
			PFNGLUNIFORM3IVARBPROC pGlUniform3ivARB;
			PFNGLUNIFORM4IVARBPROC pGlUniform4ivARB;
			PFNGLUNIFORMMATRIX2FVARBPROC pGlUniformMatrix2fvARB;
			PFNGLUNIFORMMATRIX3FVARBPROC pGlUniformMatrix3fvARB;
			PFNGLUNIFORMMATRIX4FVARBPROC pGlUniformMatrix4fvARB;
			PFNGLGETACTIVEUNIFORMARBPROC pGlGetActiveUniformARB;
			PFNGLGETACTIVEUNIFORMPROC pGlGetActiveUniform;
			PFNGLPOINTPARAMETERFARBPROC  pGlPointParameterfARB;
			PFNGLPOINTPARAMETERFVARBPROC pGlPointParameterfvARB;
			PFNGLSTENCILFUNCSEPARATEPROC pGlStencilFuncSeparate;
			PFNGLSTENCILOPSEPARATEPROC pGlStencilOpSeparate;
			PFNGLSTENCILFUNCSEPARATEATIPROC pGlStencilFuncSeparateATI;
			PFNGLSTENCILOPSEPARATEATIPROC pGlStencilOpSeparateATI;
			PFNGLCOMPRESSEDTEXIMAGE2DPROC pGlCompressedTexImage2D;
			// ARB framebuffer object
			PFNGLBINDFRAMEBUFFERPROC pGlBindFramebuffer;
			PFNGLDELETEFRAMEBUFFERSPROC pGlDeleteFramebuffers;
			PFNGLGENFRAMEBUFFERSPROC pGlGenFramebuffers;
			PFNGLCHECKFRAMEBUFFERSTATUSPROC pGlCheckFramebufferStatus;
			PFNGLFRAMEBUFFERTEXTURE2DPROC pGlFramebufferTexture2D;
			PFNGLBINDRENDERBUFFERPROC pGlBindRenderbuffer;
			PFNGLDELETERENDERBUFFERSPROC pGlDeleteRenderbuffers;
			PFNGLGENRENDERBUFFERSPROC pGlGenRenderbuffers;
			PFNGLRENDERBUFFERSTORAGEPROC pGlRenderbufferStorage;
			PFNGLFRAMEBUFFERRENDERBUFFERPROC pGlFramebufferRenderbuffer;
			PFNGLGENERATEMIPMAPPROC pGlGenerateMipmap;
			// EXT framebuffer object
			PFNGLBINDFRAMEBUFFEREXTPROC pGlBindFramebufferEXT;
			PFNGLDELETEFRAMEBUFFERSEXTPROC pGlDeleteFramebuffersEXT;
			PFNGLGENFRAMEBUFFERSEXTPROC pGlGenFramebuffersEXT;
			PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC pGlCheckFramebufferStatusEXT;
			PFNGLFRAMEBUFFERTEXTURE2DEXTPROC pGlFramebufferTexture2DEXT;
			PFNGLBINDRENDERBUFFEREXTPROC pGlBindRenderbufferEXT;
			PFNGLDELETERENDERBUFFERSEXTPROC pGlDeleteRenderbuffersEXT;
			PFNGLGENRENDERBUFFERSEXTPROC pGlGenRenderbuffersEXT;
			PFNGLRENDERBUFFERSTORAGEEXTPROC pGlRenderbufferStorageEXT;
			PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC pGlFramebufferRenderbufferEXT;
			PFNGLGENERATEMIPMAPEXTPROC pGlGenerateMipmapEXT;
			PFNGLACTIVESTENCILFACEEXTPROC pGlActiveStencilFaceEXT;
			PFNGLDRAWBUFFERSARBPROC pGlDrawBuffersARB;
			PFNGLDRAWBUFFERSATIPROC pGlDrawBuffersATI;
			PFNGLGENBUFFERSARBPROC pGlGenBuffersARB;
			PFNGLBINDBUFFERARBPROC pGlBindBufferARB;
			PFNGLBUFFERDATAARBPROC pGlBufferDataARB;
			PFNGLDELETEBUFFERSARBPROC pGlDeleteBuffersARB;
			PFNGLBUFFERSUBDATAARBPROC pGlBufferSubDataARB;
			PFNGLGETBUFFERSUBDATAARBPROC pGlGetBufferSubDataARB;
			PFNGLMAPBUFFERARBPROC pGlMapBufferARB;
			PFNGLUNMAPBUFFERARBPROC pGlUnmapBufferARB;
			PFNGLISBUFFERARBPROC pGlIsBufferARB;
			PFNGLGETBUFFERPARAMETERIVARBPROC pGlGetBufferParameterivARB;
			PFNGLGETBUFFERPOINTERVARBPROC pGlGetBufferPointervARB;
			PFNGLPROVOKINGVERTEXPROC pGlProvokingVertexARB;
			PFNGLPROVOKINGVERTEXEXTPROC pGlProvokingVertexEXT;
			PFNGLCOLORMASKINDEXEDEXTPROC pGlColorMaskIndexedEXT;
			PFNGLENABLEINDEXEDEXTPROC pGlEnableIndexedEXT;
			PFNGLDISABLEINDEXEDEXTPROC pGlDisableIndexedEXT;
			PFNGLBLENDFUNCINDEXEDAMDPROC pGlBlendFuncIndexedAMD;
			PFNGLBLENDFUNCIPROC pGlBlendFunciARB;
			PFNGLBLENDEQUATIONINDEXEDAMDPROC pGlBlendEquationIndexedAMD;
			PFNGLBLENDEQUATIONIPROC pGlBlendEquationiARB;
			PFNGLPROGRAMPARAMETERIARBPROC pGlProgramParameteriARB;
			PFNGLPROGRAMPARAMETERIEXTPROC pGlProgramParameteriEXT;
			PFNGLGENQUERIESARBPROC pGlGenQueriesARB;
			PFNGLDELETEQUERIESARBPROC pGlDeleteQueriesARB;
			PFNGLISQUERYARBPROC pGlIsQueryARB;
			PFNGLBEGINQUERYARBPROC pGlBeginQueryARB;
			PFNGLENDQUERYARBPROC pGlEndQueryARB;
			PFNGLGETQUERYIVARBPROC pGlGetQueryivARB;
			PFNGLGETQUERYOBJECTIVARBPROC pGlGetQueryObjectivARB;
			PFNGLGETQUERYOBJECTUIVARBPROC pGlGetQueryObjectuivARB;
			PFNGLGENOCCLUSIONQUERIESNVPROC pGlGenOcclusionQueriesNV;
			PFNGLDELETEOCCLUSIONQUERIESNVPROC pGlDeleteOcclusionQueriesNV;
			PFNGLISOCCLUSIONQUERYNVPROC pGlIsOcclusionQueryNV;
			PFNGLBEGINOCCLUSIONQUERYNVPROC pGlBeginOcclusionQueryNV;
			PFNGLENDOCCLUSIONQUERYNVPROC pGlEndOcclusionQueryNV;
			PFNGLGETOCCLUSIONQUERYIVNVPROC pGlGetOcclusionQueryivNV;
			PFNGLGETOCCLUSIONQUERYUIVNVPROC pGlGetOcclusionQueryuivNV;
			PFNGLBLENDEQUATIONEXTPROC pGlBlendEquationEXT;
			PFNGLBLENDEQUATIONPROC pGlBlendEquation;
#if defined(WGL_EXT_swap_control)
			PFNWGLSWAPINTERVALEXTPROC pWglSwapIntervalEXT;
#endif
#if defined(GLX_SGI_swap_control)
			PFNGLXSWAPINTERVALSGIPROC pGlxSwapIntervalSGI;
#endif
#if defined(GLX_EXT_swap_control)
			PFNGLXSWAPINTERVALEXTPROC pGlxSwapIntervalEXT;
#endif
#if defined(GLX_MESA_swap_control)
			PFNGLXSWAPINTERVALMESAPROC pGlxSwapIntervalMESA;
#endif
#endif
		};

		inline void COpenGLExtensionHandler::extGlActiveTexture(GLenum texture)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (MultiTextureExtension && pGlActiveTextureARB)
				pGlActiveTextureARB(texture); //选择可以由纹理函数进行修改的当前纹理单位，
#else
			if (MultiTextureExtension)
#ifdef GL_ARB_multitexture
				glActiveTextureARB(texture);
#else
				glActiveTexture(texture);
#endif
#endif
		}

		inline void COpenGLExtensionHandler::extGlClientActiveTexture(GLenum texture)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (MultiTextureExtension && pGlClientActiveTextureARB)
				pGlClientActiveTextureARB(texture);
#else
			if (MultiTextureExtension)
				glClientActiveTextureARB(texture);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGenPrograms(GLsizei n, GLuint *programs)
		{
			if (programs)
				memset(programs, 0, n*sizeof(GLuint));
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGenProgramsARB)
				pGlGenProgramsARB(n, programs);
			else if (pGlGenProgramsNV)
				pGlGenProgramsNV(n, programs);
#elif defined(GL_ARB_vertex_program) || defined(GL_ARB_fragment_program)
			glGenProgramsARB(n, programs);
#elif defined(GL_NV_vertex_program) || defined(GL_NV_fragment_program)
			glGenProgramsNV(n, programs);
#else
			os::Printer::log("glGenPrograms not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlBindProgram(GLenum target, GLuint program)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlBindProgramARB)
				pGlBindProgramARB(target, program);
			else if (pGlBindProgramNV)
				pGlBindProgramNV(target, program);
#elif defined(GL_ARB_vertex_program) || defined(GL_ARB_fragment_program)
			glBindProgramARB(target, program);
#elif defined(GL_NV_vertex_program) || defined(GL_NV_fragment_program)
			glBindProgramNV(target, program);
#else
			os::Printer::log("glBindProgram not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlProgramString(GLenum target, GLenum format, GLsizei len, const GLvoid *string)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlProgramStringARB)
				pGlProgramStringARB(target, format, len, string);
#elif defined(GL_ARB_vertex_program) || defined(GL_ARB_fragment_program)
			glProgramStringARB(target, format, len, string);
#else
			os::Printer::log("glProgramString not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlLoadProgram(GLenum target, GLuint id, GLsizei len, const GLubyte *string)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlLoadProgramNV)
				pGlLoadProgramNV(target, id, len, string);
#elif defined(GL_NV_vertex_program) || defined(GL_NV_fragment_program)
			glLoadProgramNV(target, id, len, string);
#else
			os::Printer::log("glLoadProgram not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlDeletePrograms(GLsizei n, const GLuint *programs)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlDeleteProgramsARB)
				pGlDeleteProgramsARB(n, programs);
			else if (pGlDeleteProgramsNV)
				pGlDeleteProgramsNV(n, programs);
#elif defined(GL_ARB_vertex_program) || defined(GL_ARB_fragment_program)
			glDeleteProgramsARB(n, programs);
#elif defined(GL_NV_vertex_program) || defined(GL_NV_fragment_program)
			glDeleteProgramsNV(n, programs);
#else
			os::Printer::log("glDeletePrograms not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlProgramLocalParameter4fv(GLenum n, GLuint i, const GLfloat *f)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlProgramLocalParameter4fvARB)
				pGlProgramLocalParameter4fvARB(n, i, f);
#elif defined(GL_ARB_vertex_program) || defined(GL_ARB_fragment_program)
			glProgramLocalParameter4fvARB(n, i, f);
#else
			os::Printer::log("glProgramLocalParameter4fv not supported", ELL_ERROR);
#endif
		}

		inline GLhandleARB COpenGLExtensionHandler::extGlCreateShaderObject(GLenum shaderType)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlCreateShaderObjectARB)
				return pGlCreateShaderObjectARB(shaderType);
#elif defined(GL_ARB_shader_objects)
			return glCreateShaderObjectARB(shaderType);
#else
			os::Printer::log("glCreateShaderObject not supported", ELL_ERROR);
#endif
			return 0;
		}

		inline GLuint COpenGLExtensionHandler::extGlCreateShader(GLenum shaderType)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlCreateShader)
				return pGlCreateShader(shaderType);
#elif defined(GL_VERSION_2_0)
			return glCreateShader(shaderType);
#else
			os::Printer::log("glCreateShader not supported", ELL_ERROR);
#endif
			return 0;
		}

		inline void COpenGLExtensionHandler::extGlShaderSourceARB(GLhandleARB shader, GLsizei numOfStrings, const char **strings, const GLint *lenOfStrings)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlShaderSourceARB)
				pGlShaderSourceARB(shader, numOfStrings, strings, lenOfStrings);
#elif defined(GL_ARB_shader_objects)
			glShaderSourceARB(shader, numOfStrings, strings, (GLint *)lenOfStrings);
#else
			os::Printer::log("glShaderSource not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlShaderSource(GLuint shader, GLsizei numOfStrings, const char **strings, const GLint *lenOfStrings)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlShaderSource)
				pGlShaderSource(shader, numOfStrings, strings, lenOfStrings);
#elif defined(GL_VERSION_2_0)
			glShaderSource(shader, numOfStrings, strings, (GLint *)lenOfStrings);
#else
			os::Printer::log("glShaderSource not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlCompileShaderARB(GLhandleARB shader)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlCompileShaderARB)
				pGlCompileShaderARB(shader);
#elif defined(GL_ARB_shader_objects)
			glCompileShaderARB(shader);
#else
			os::Printer::log("glCompileShader not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlCompileShader(GLuint shader)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlCompileShader)
				pGlCompileShader(shader);
#elif defined(GL_VERSION_2_0)
			glCompileShader(shader);
#else
			os::Printer::log("glCompileShader not supported", ELL_ERROR);
#endif
		}

		inline GLhandleARB COpenGLExtensionHandler::extGlCreateProgramObject(void)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlCreateProgramObjectARB)
				return pGlCreateProgramObjectARB();
#elif defined(GL_ARB_shader_objects)
			return glCreateProgramObjectARB();
#else
			os::Printer::log("glCreateProgramObject not supported", ELL_ERROR);
#endif
			return 0;
		}

		inline GLuint COpenGLExtensionHandler::extGlCreateProgram(void)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlCreateProgram)
				return pGlCreateProgram();
#elif defined(GL_VERSION_2_0)
			return glCreateProgram();
#else
			os::Printer::log("glCreateProgram not supported", ELL_ERROR);
#endif
			return 0;
		}

		inline void COpenGLExtensionHandler::extGlAttachObject(GLhandleARB program, GLhandleARB shader)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlAttachObjectARB)
				pGlAttachObjectARB(program, shader);
#elif defined(GL_ARB_shader_objects)
			glAttachObjectARB(program, shader);
#else
			os::Printer::log("glAttachObject not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlAttachShader(GLuint program, GLuint shader)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlAttachShader)
				pGlAttachShader(program, shader);
#elif defined(GL_VERSION_2_0)
			glAttachShader(program, shader);
#else
			os::Printer::log("glAttachShader not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlLinkProgramARB(GLhandleARB program)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlLinkProgramARB)
				pGlLinkProgramARB(program);
#elif defined(GL_ARB_shader_objects)
			glLinkProgramARB(program);
#else
			os::Printer::log("glLinkProgram not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlLinkProgram(GLuint program)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlLinkProgram)
				pGlLinkProgram(program);
#elif defined(GL_VERSION_2_0)
			glLinkProgram(program);
#else
			os::Printer::log("glLinkProgram not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUseProgramObject(GLhandleARB prog)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUseProgramObjectARB)
				pGlUseProgramObjectARB(prog);
#elif defined(GL_ARB_shader_objects)
			glUseProgramObjectARB(prog);
#else
			os::Printer::log("glUseProgramObject not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUseProgram(GLuint prog)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUseProgram)
				pGlUseProgram(prog);
#elif defined(GL_VERSION_2_0)
			glUseProgram(prog);
#else
			os::Printer::log("glUseProgram not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlDeleteObject(GLhandleARB object)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlDeleteObjectARB)
				pGlDeleteObjectARB(object);
#elif defined(GL_ARB_shader_objects)
			glDeleteObjectARB(object);
#else
			os::Printer::log("glDeleteObject not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlDeleteProgram(GLuint object)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlDeleteProgram)
				pGlDeleteProgram(object);
#elif defined(GL_VERSION_2_0)
			glDeleteProgram(object);
#else
			os::Printer::log("glDeleteProgram not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlDeleteShader(GLuint shader)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlDeleteShader)
				pGlDeleteShader(shader);
#elif defined(GL_VERSION_2_0)
			glDeleteShader(shader);
#else
			os::Printer::log("glDeleteShader not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetAttachedObjects(GLhandleARB program, GLsizei maxcount, GLsizei* count, GLhandleARB* shaders)
		{
			if (count)
				*count = 0;
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetAttachedObjectsARB)
				pGlGetAttachedObjectsARB(program, maxcount, count, shaders);
#elif defined(GL_ARB_shader_objects)
			glGetAttachedObjectsARB(program, maxcount, count, shaders);
#else
			os::Printer::log("glGetAttachedObjects not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
		{
			if (count)
				*count = 0;
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetAttachedShaders)
				pGlGetAttachedShaders(program, maxcount, count, shaders);
#elif defined(GL_VERSION_2_0)
			glGetAttachedShaders(program, maxcount, count, shaders);
#else
			os::Printer::log("glGetAttachedShaders not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetInfoLog(GLhandleARB object, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog)
		{
			if (length)
				*length = 0;
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetInfoLogARB)
				pGlGetInfoLogARB(object, maxLength, length, infoLog);
#elif defined(GL_ARB_shader_objects)
			glGetInfoLogARB(object, maxLength, length, infoLog);
#else
			os::Printer::log("glGetInfoLog not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
		{
			if (length)
				*length = 0;
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetShaderInfoLog)
				pGlGetShaderInfoLog(shader, maxLength, length, infoLog);
#elif defined(GL_VERSION_2_0)
			glGetShaderInfoLog(shader, maxLength, length, infoLog);
#else
			os::Printer::log("glGetShaderInfoLog not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
		{
			if (length)
				*length = 0;
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetProgramInfoLog)
				pGlGetProgramInfoLog(program, maxLength, length, infoLog);
#elif defined(GL_VERSION_2_0)
			glGetProgramInfoLog(program, maxLength, length, infoLog);
#else
			os::Printer::log("glGetProgramInfoLog not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetObjectParameteriv(GLhandleARB object, GLenum type, GLint *param)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetObjectParameterivARB)
				pGlGetObjectParameterivARB(object, type, param);
#elif defined(GL_ARB_shader_objects)
			glGetObjectParameterivARB(object, type, param);
#else
			os::Printer::log("glGetObjectParameteriv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetShaderiv(GLuint shader, GLenum type, GLint *param)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetShaderiv)
				pGlGetShaderiv(shader, type, param);
#elif defined(GL_VERSION_2_0)
			glGetShaderiv(shader, type, param);
#else
			os::Printer::log("glGetShaderiv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetProgramiv(GLuint program, GLenum type, GLint *param)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetProgramiv)
				pGlGetProgramiv(program, type, param);
#elif defined(GL_VERSION_2_0)
			glGetProgramiv(program, type, param);
#else
			os::Printer::log("glGetProgramiv not supported", ELL_ERROR);
#endif
		}

		inline GLint COpenGLExtensionHandler::extGlGetUniformLocationARB(GLhandleARB program, const char *name)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetUniformLocationARB)
				return pGlGetUniformLocationARB(program, name);
#elif defined(GL_ARB_shader_objects)
			return glGetUniformLocationARB(program, name);
#else
			os::Printer::log("glGetUniformLocation not supported", ELL_ERROR);
#endif
			return 0;
		}

		inline GLint COpenGLExtensionHandler::extGlGetUniformLocation(GLuint program, const char *name)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetUniformLocation)
				return pGlGetUniformLocation(program, name);
#elif defined(GL_VERSION_2_0)
			return glGetUniformLocation(program, name);
#else
			os::Printer::log("glGetUniformLocation not supported", ELL_ERROR);
#endif
			return 0;
		}

		inline void COpenGLExtensionHandler::extGlUniform1fv(GLint loc, GLsizei count, const GLfloat *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUniform1fvARB)
				pGlUniform1fvARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
			glUniform1fvARB(loc, count, v);
#else
			os::Printer::log("glUniform1fv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUniform2fv(GLint loc, GLsizei count, const GLfloat *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUniform2fvARB)
				pGlUniform2fvARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
			glUniform2fvARB(loc, count, v);
#else
			os::Printer::log("glUniform2fv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUniform3fv(GLint loc, GLsizei count, const GLfloat *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUniform3fvARB)
				pGlUniform3fvARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
			glUniform3fvARB(loc, count, v);
#else
			os::Printer::log("glUniform3fv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUniform4fv(GLint loc, GLsizei count, const GLfloat *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUniform4fvARB)
				pGlUniform4fvARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
			glUniform4fvARB(loc, count, v);
#else
			os::Printer::log("glUniform4fv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUniform1iv(GLint loc, GLsizei count, const GLint *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUniform1ivARB)
				pGlUniform1ivARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
			glUniform1ivARB(loc, count, v);
#else
			os::Printer::log("glUniform1iv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUniform2iv(GLint loc, GLsizei count, const GLint *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUniform2ivARB)
				pGlUniform2ivARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
			glUniform2ivARB(loc, count, v);
#else
			os::Printer::log("glUniform2iv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUniform3iv(GLint loc, GLsizei count, const GLint *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUniform3ivARB)
				pGlUniform3ivARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
			glUniform3ivARB(loc, count, v);
#else
			os::Printer::log("glUniform3iv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUniform4iv(GLint loc, GLsizei count, const GLint *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUniform4ivARB)
				pGlUniform4ivARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
			glUniform4ivARB(loc, count, v);
#else
			os::Printer::log("glUniform4iv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUniformMatrix2fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUniformMatrix2fvARB)
				pGlUniformMatrix2fvARB(loc, count, transpose, v);
#elif defined(GL_ARB_shader_objects)
			glUniformMatrix2fvARB(loc, count, transpose, v);
#else
			os::Printer::log("glUniformMatrix2fv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUniformMatrix3fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUniformMatrix3fvARB)
				pGlUniformMatrix3fvARB(loc, count, transpose, v);
#elif defined(GL_ARB_shader_objects)
			glUniformMatrix3fvARB(loc, count, transpose, v);
#else
			os::Printer::log("glUniformMatrix3fv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlUniformMatrix4fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUniformMatrix4fvARB)
				pGlUniformMatrix4fvARB(loc, count, transpose, v);
#elif defined(GL_ARB_shader_objects)
			glUniformMatrix4fvARB(loc, count, transpose, v);
#else
			os::Printer::log("glUniformMatrix4fv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetActiveUniformARB(GLhandleARB program,
			GLuint index, GLsizei maxlength, GLsizei *length,
			GLint *size, GLenum *type, GLcharARB *name)
		{
			if (length)
				*length = 0;
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetActiveUniformARB)
				pGlGetActiveUniformARB(program, index, maxlength, length, size, type, name);
#elif defined(GL_ARB_shader_objects)
			glGetActiveUniformARB(program, index, maxlength, length, size, type, name);
#else
			os::Printer::log("glGetActiveUniform not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetActiveUniform(GLuint program,
			GLuint index, GLsizei maxlength, GLsizei *length,
			GLint *size, GLenum *type, GLchar *name)
		{
			if (length)
				*length = 0;
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetActiveUniform)
				pGlGetActiveUniform(program, index, maxlength, length, size, type, name);
#elif defined(GL_VERSION_2_0)
			glGetActiveUniform(program, index, maxlength, length, size, type, name);
#else
			os::Printer::log("glGetActiveUniform not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlPointParameterf(GLint loc, GLfloat f)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlPointParameterfARB)
				pGlPointParameterfARB(loc, f);
#elif defined(GL_ARB_point_parameters)
			glPointParameterfARB(loc, f);
#else
			os::Printer::log("glPointParameterf not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlPointParameterfv(GLint loc, const GLfloat *v)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlPointParameterfvARB)
				pGlPointParameterfvARB(loc, v);
#elif defined(GL_ARB_point_parameters)
			glPointParameterfvARB(loc, v);
#else
			os::Printer::log("glPointParameterfv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlStencilFuncSeparate(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlStencilFuncSeparate)
				pGlStencilFuncSeparate(frontfunc, backfunc, ref, mask);
			else if (pGlStencilFuncSeparateATI)
				pGlStencilFuncSeparateATI(frontfunc, backfunc, ref, mask);
#elif defined(GL_VERSION_2_0)
			glStencilFuncSeparate(frontfunc, backfunc, ref, mask);
#elif defined(GL_ATI_separate_stencil)
			glStencilFuncSeparateATI(frontfunc, backfunc, ref, mask);
#else
			os::Printer::log("glStencilFuncSeparate not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlStencilOpSeparate)
				pGlStencilOpSeparate(face, fail, zfail, zpass);
			else if (pGlStencilOpSeparateATI)
				pGlStencilOpSeparateATI(face, fail, zfail, zpass);
#elif defined(GL_VERSION_2_0)
			glStencilOpSeparate(face, fail, zfail, zpass);
#elif defined(GL_ATI_separate_stencil)
			glStencilOpSeparateATI(face, fail, zfail, zpass);
#else
			os::Printer::log("glStencilOpSeparate not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width,
			GLsizei height, GLint border, GLsizei imageSize, const void* data)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlCompressedTexImage2D)
				pGlCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
#elif defined(GL_ARB_texture_compression)
			glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
#else
			os::Printer::log("glCompressedTexImage2D not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlBindFramebuffer(GLenum target, GLuint framebuffer)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlBindFramebuffer)
				pGlBindFramebuffer(target, framebuffer);
			else if (pGlBindFramebufferEXT)
				pGlBindFramebufferEXT(target, framebuffer);
#elif defined(GL_ARB_framebuffer_object)
			glBindFramebuffer(target, framebuffer);
#elif defined(GL_EXT_framebuffer_object)
			glBindFramebufferEXT(target, framebuffer);
#else
			os::Printer::log("glBindFramebuffer not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlDeleteFramebuffers)
				pGlDeleteFramebuffers(n, framebuffers);
			else if (pGlDeleteFramebuffersEXT)
				pGlDeleteFramebuffersEXT(n, framebuffers);
#elif defined(GL_ARB_framebuffer_object)
			glDeleteFramebuffers(n, framebuffers);
#elif defined(GL_EXT_framebuffer_object)
			glDeleteFramebuffersEXT(n, framebuffers);
#else
			os::Printer::log("glDeleteFramebuffers not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGenFramebuffers(GLsizei n, GLuint *framebuffers)
		{
			if (framebuffers)
				memset(framebuffers, 0, n*sizeof(GLuint));
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGenFramebuffers)
				pGlGenFramebuffers(n, framebuffers);
			else if (pGlGenFramebuffersEXT)
				pGlGenFramebuffersEXT(n, framebuffers);
#elif defined(GL_ARB_framebuffer_object)
			glGenFramebuffers(n, framebuffers);
#elif defined(GL_EXT_framebuffer_object)
			glGenFramebuffersEXT(n, framebuffers);
#else
			os::Printer::log("glGenFramebuffers not supported", ELL_ERROR);
#endif
		}

		inline GLenum COpenGLExtensionHandler::extGlCheckFramebufferStatus(GLenum target)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlCheckFramebufferStatus)
				return pGlCheckFramebufferStatus(target);
			else if (pGlCheckFramebufferStatusEXT)
				return pGlCheckFramebufferStatusEXT(target);
			else
				return 0;
#elif defined(GL_ARB_framebuffer_object)
			return glCheckFramebufferStatus(target);
#elif defined(GL_EXT_framebuffer_object)
			return glCheckFramebufferStatusEXT(target);
#else
			os::Printer::log("glCheckFramebufferStatus not supported", ELL_ERROR);
			return 0;
#endif
		}

		inline void COpenGLExtensionHandler::extGlFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlFramebufferTexture2D)
				pGlFramebufferTexture2D(target, attachment, textarget, texture, level);
			else if (pGlFramebufferTexture2DEXT)
				pGlFramebufferTexture2DEXT(target, attachment, textarget, texture, level);
#elif defined(GL_ARB_framebuffer_object)
			glFramebufferTexture2D(target, attachment, textarget, texture, level);
#elif defined(GL_EXT_framebuffer_object)
			glFramebufferTexture2DEXT(target, attachment, textarget, texture, level);
#else
			os::Printer::log("glFramebufferTexture2D not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlBindRenderbuffer(GLenum target, GLuint renderbuffer)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlBindRenderbuffer)
				pGlBindRenderbuffer(target, renderbuffer);
			else if (pGlBindRenderbufferEXT)
				pGlBindRenderbufferEXT(target, renderbuffer);
#elif defined(GL_ARB_framebuffer_object)
			glBindRenderbuffer(target, renderbuffer);
#elif defined(GL_EXT_framebuffer_object)
			glBindRenderbufferEXT(target, renderbuffer);
#else
			os::Printer::log("glBindRenderbuffer not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlDeleteRenderbuffers)
				pGlDeleteRenderbuffers(n, renderbuffers);
			else if (pGlDeleteRenderbuffersEXT)
				pGlDeleteRenderbuffersEXT(n, renderbuffers);
#elif defined(GL_ARB_framebuffer_object)
			glDeleteRenderbuffers(n, renderbuffers);
#elif defined(GL_EXT_framebuffer_object)
			glDeleteRenderbuffersEXT(n, renderbuffers);
#else
			os::Printer::log("glDeleteRenderbuffers not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
		{
			if (renderbuffers)
				memset(renderbuffers, 0, n*sizeof(GLuint));
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGenRenderbuffers)
				pGlGenRenderbuffers(n, renderbuffers);
			else if (pGlGenRenderbuffersEXT)
				pGlGenRenderbuffersEXT(n, renderbuffers);
#elif defined(GL_ARB_framebuffer_object)
			glGenRenderbuffers(n, renderbuffers);
#elif defined(GL_EXT_framebuffer_object)
			glGenRenderbuffersEXT(n, renderbuffers);
#else
			os::Printer::log("glGenRenderbuffers not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlRenderbufferStorage)
				pGlRenderbufferStorage(target, internalformat, width, height);
			else if (pGlRenderbufferStorageEXT)
				pGlRenderbufferStorageEXT(target, internalformat, width, height);
#elif defined(GL_ARB_framebuffer_object)
			glRenderbufferStorage(target, internalformat, width, height);
#elif defined(GL_EXT_framebuffer_object)
			glRenderbufferStorageEXT(target, internalformat, width, height);
#else
			os::Printer::log("glRenderbufferStorage not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlFramebufferRenderbuffer)
				pGlFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
			else if (pGlFramebufferRenderbufferEXT)
				pGlFramebufferRenderbufferEXT(target, attachment, renderbuffertarget, renderbuffer);
#elif defined(GL_ARB_framebuffer_object)
			glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
#elif defined(GL_EXT_framebuffer_object)
			glFramebufferRenderbufferEXT(target, attachment, renderbuffertarget, renderbuffer);
#else
			os::Printer::log("glFramebufferRenderbuffer not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGenerateMipmap(GLenum target)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGenerateMipmap)
				pGlGenerateMipmap(target);
			else if (pGlGenerateMipmapEXT)
				pGlGenerateMipmapEXT(target);
#elif defined(GL_ARB_framebuffer_object)
			glGenerateMipmap(target);
#elif defined(GL_EXT_framebuffer_object)
			glGenerateMipmapEXT(target);
#else
			os::Printer::log("glGenerateMipmap not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlActiveStencilFace(GLenum face)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlActiveStencilFaceEXT)
				pGlActiveStencilFaceEXT(face);
#elif defined(GL_EXT_stencil_two_side)
			glActiveStencilFaceEXT(face);
#else
			os::Printer::log("glActiveStencilFace not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlDrawBuffers(GLsizei n, const GLenum *bufs)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlDrawBuffersARB)
				pGlDrawBuffersARB(n, bufs);
			else if (pGlDrawBuffersATI)
				pGlDrawBuffersATI(n, bufs);
#elif defined(GL_ARB_draw_buffers)
			glDrawBuffersARB(n, bufs);
#elif defined(GL_ATI_draw_buffers)
			glDrawBuffersATI(n, bufs);
#else
			os::Printer::log("glDrawBuffers not supported", ELL_ERROR);
#endif
		}


		inline void COpenGLExtensionHandler::extGlGenBuffers(GLsizei n, GLuint *buffers)
		{
			if (buffers)
				memset(buffers, 0, n*sizeof(GLuint));
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGenBuffersARB)
				pGlGenBuffersARB(n, buffers);
#elif defined(GL_ARB_vertex_buffer_object)
			glGenBuffers(n, buffers);
#else
			os::Printer::log("glGenBuffers not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlBindBuffer(GLenum target, GLuint buffer)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlBindBufferARB)
				pGlBindBufferARB(target, buffer);
#elif defined(GL_ARB_vertex_buffer_object)
			glBindBuffer(target, buffer);
#else
			os::Printer::log("glBindBuffer not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlBufferData(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlBufferDataARB)
				pGlBufferDataARB(target, size, data, usage);
#elif defined(GL_ARB_vertex_buffer_object)
			glBufferData(target, size, data, usage);
#else
			os::Printer::log("glBufferData not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlDeleteBuffers(GLsizei n, const GLuint *buffers)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlDeleteBuffersARB)
				pGlDeleteBuffersARB(n, buffers);
#elif defined(GL_ARB_vertex_buffer_object)
			glDeleteBuffers(n, buffers);
#else
			os::Printer::log("glDeleteBuffers not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlBufferSubData(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlBufferSubDataARB)
				pGlBufferSubDataARB(target, offset, size, data);
#elif defined(GL_ARB_vertex_buffer_object)
			glBufferSubData(target, offset, size, data);
#else
			os::Printer::log("glBufferSubData not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetBufferSubData(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetBufferSubDataARB)
				pGlGetBufferSubDataARB(target, offset, size, data);
#elif defined(GL_ARB_vertex_buffer_object)
			glGetBufferSubData(target, offset, size, data);
#else
			os::Printer::log("glGetBufferSubData not supported", ELL_ERROR);
#endif
		}

		inline void *COpenGLExtensionHandler::extGlMapBuffer(GLenum target, GLenum access)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlMapBufferARB)
				return pGlMapBufferARB(target, access);
			return 0;
#elif defined(GL_ARB_vertex_buffer_object)
			return glMapBuffer(target, access);
#else
			os::Printer::log("glMapBuffer not supported", ELL_ERROR);
			return 0;
#endif
		}

		inline GLboolean COpenGLExtensionHandler::extGlUnmapBuffer(GLenum target)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlUnmapBufferARB)
				return pGlUnmapBufferARB(target);
			return false;
#elif defined(GL_ARB_vertex_buffer_object)
			return glUnmapBuffer(target);
#else
			os::Printer::log("glUnmapBuffer not supported", ELL_ERROR);
			return false;
#endif
		}

		inline GLboolean COpenGLExtensionHandler::extGlIsBuffer(GLuint buffer)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlIsBufferARB)
				return pGlIsBufferARB(buffer);
			return false;
#elif defined(GL_ARB_vertex_buffer_object)
			return glIsBuffer(buffer);
#else
			os::Printer::log("glDeleteBuffers not supported", ELL_ERROR);
			return false;
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetBufferParameterivARB)
				pGlGetBufferParameterivARB(target, pname, params);
#elif defined(GL_ARB_vertex_buffer_object)
			glGetBufferParameteriv(target, pname, params);
#else
			os::Printer::log("glGetBufferParameteriv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetBufferPointerv(GLenum target, GLenum pname, GLvoid **params)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetBufferPointervARB)
				pGlGetBufferPointervARB(target, pname, params);
#elif defined(GL_ARB_vertex_buffer_object)
			glGetBufferPointerv(target, pname, params);
#else
			os::Printer::log("glGetBufferPointerv not supported", ELL_ERROR);
#endif
		}


		inline void COpenGLExtensionHandler::extGlProvokingVertex(GLenum mode)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (FeatureAvailable[SAPPHIRE_ARB_provoking_vertex] && pGlProvokingVertexARB)
				pGlProvokingVertexARB(mode);
			else if (FeatureAvailable[SAPPHIRE_EXT_provoking_vertex] && pGlProvokingVertexEXT)
				pGlProvokingVertexEXT(mode);
#elif defined(GL_ARB_provoking_vertex)
			glProvokingVertex(mode);
#elif defined(GL_EXT_provoking_vertex)
			glProvokingVertexEXT(mode);
#else
			os::Printer::log("glProvokingVertex not supported", ELL_ERROR);
#endif
		}


		inline void COpenGLExtensionHandler::extGlColorMaskIndexed(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (FeatureAvailable[SAPPHIRE_EXT_draw_buffers2] && pGlColorMaskIndexedEXT)
				pGlColorMaskIndexedEXT(buf, r, g, b, a);
#elif defined(GL_EXT_draw_buffers2)
			glColorMaskIndexedEXT(buf, r, g, b, a);
#else
			os::Printer::log("glColorMaskIndexed not supported", ELL_ERROR);
#endif
		}


		inline void COpenGLExtensionHandler::extGlEnableIndexed(GLenum target, GLuint index)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (FeatureAvailable[SAPPHIRE_EXT_draw_buffers2] && pGlEnableIndexedEXT)
				pGlEnableIndexedEXT(target, index);
#elif defined(GL_EXT_draw_buffers2)
			glEnableIndexedEXT(target, index);
#else
			os::Printer::log("glEnableIndexed not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlDisableIndexed(GLenum target, GLuint index)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (FeatureAvailable[SAPPHIRE_EXT_draw_buffers2] && pGlDisableIndexedEXT)
				pGlDisableIndexedEXT(target, index);
#elif defined(GL_EXT_draw_buffers2)
			glDisableIndexedEXT(target, index);
#else
			os::Printer::log("glDisableIndexed not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlBlendFuncIndexed(GLuint buf, GLenum src, GLenum dst)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (FeatureAvailable[SAPPHIRE_ARB_draw_buffers_blend] && pGlBlendFunciARB)
				pGlBlendFunciARB(buf, src, dst);
			else if (FeatureAvailable[SAPPHIRE_AMD_draw_buffers_blend] && pGlBlendFuncIndexedAMD)
				pGlBlendFuncIndexedAMD(buf, src, dst);
#elif defined(GL_ARB_draw_buffers_blend)
			glBlendFunciARB(buf, src, dst);
#elif defined(GL_AMD_draw_buffers_blend)
			glBlendFuncIndexedAMD(buf, src, dst);
#else
			os::Printer::log("glBlendFuncIndexed not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlBlendEquationIndexed(GLuint buf, GLenum mode)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (FeatureAvailable[SAPPHIRE_ARB_draw_buffers_blend] && pGlBlendEquationiARB)
				pGlBlendEquationiARB(buf, mode);
			else if (FeatureAvailable[SAPPHIRE_AMD_draw_buffers_blend] && pGlBlendEquationIndexedAMD)
				pGlBlendEquationIndexedAMD(buf, mode);
#elif defined(GL_ARB_draw_buffers_blend)
			glBlendEquationiARB(buf, mode);
#elif defined(GL_AMD_draw_buffers_blend)
			glBlendEquationIndexedAMD(buf, mode);
#else
			os::Printer::log("glBlendEquationIndexed not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlProgramParameteri(GLuint program, GLenum pname, GLint value)
		{
#if defined(SAPPHIRE_OPENGL_USE_EXTPOINTER)
			if (queryFeature(EVDF_GEOMETRY_SHADER))
			{
				if (pGlProgramParameteriARB)
					pGlProgramParameteriARB(program, pname, value);
				else if (pGlProgramParameteriEXT)
					pGlProgramParameteriEXT(program, pname, value);
			}
#elif defined(GL_ARB_geometry_shader4)
			glProgramParameteriARB(program, pname, value);
#elif defined(GL_EXT_geometry_shader4)
			glProgramParameteriEXT(program, pname, value);
#elif defined(GL_NV_geometry_program4) || defined(GL_NV_geometry_shader4)
			glProgramParameteriNV(program, pname, value);
#else
			os::Printer::log("glProgramParameteri not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGenQueries(GLsizei n, GLuint *ids)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGenQueriesARB)
				pGlGenQueriesARB(n, ids);
			else if (pGlGenOcclusionQueriesNV)
				pGlGenOcclusionQueriesNV(n, ids);
#elif defined(GL_ARB_occlusion_query)
			glGenQueriesARB(n, ids);
#elif defined(GL_NV_occlusion_query)
			glGenOcclusionQueriesNV(n, ids);
#else
			os::Printer::log("glGenQueries not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlDeleteQueries(GLsizei n, const GLuint *ids)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlDeleteQueriesARB)
				pGlDeleteQueriesARB(n, ids);
			else if (pGlDeleteOcclusionQueriesNV)
				pGlDeleteOcclusionQueriesNV(n, ids);
#elif defined(GL_ARB_occlusion_query)
			glDeleteQueriesARB(n, ids);
#elif defined(GL_NV_occlusion_query)
			glDeleteOcclusionQueriesNV(n, ids);
#else
			os::Printer::log("glDeleteQueries not supported", ELL_ERROR);
#endif
		}

		inline GLboolean COpenGLExtensionHandler::extGlIsQuery(GLuint id)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlIsQueryARB)
				return pGlIsQueryARB(id);
			else if (pGlIsOcclusionQueryNV)
				return pGlIsOcclusionQueryNV(id);
			return false;
#elif defined(GL_ARB_occlusion_query)
			return glIsQueryARB(id);
#elif defined(GL_NV_occlusion_query)
			return glIsOcclusionQueryNV(id);
#else
			return false;
#endif
		}

		inline void COpenGLExtensionHandler::extGlBeginQuery(GLenum target, GLuint id)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlBeginQueryARB)
				pGlBeginQueryARB(target, id);
			else if (pGlBeginOcclusionQueryNV)
				pGlBeginOcclusionQueryNV(id);
#elif defined(GL_ARB_occlusion_query)
			glBeginQueryARB(target, id);
#elif defined(GL_NV_occlusion_query)
			glBeginOcclusionQueryNV(id);
#else
			os::Printer::log("glBeginQuery not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlEndQuery(GLenum target)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlEndQueryARB)
				pGlEndQueryARB(target);
			else if (pGlEndOcclusionQueryNV)
				pGlEndOcclusionQueryNV();
#elif defined(GL_ARB_occlusion_query)
			glEndQueryARB(target);
#elif defined(GL_NV_occlusion_query)
			glEndOcclusionQueryNV();
#else
			os::Printer::log("glEndQuery not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetQueryiv(GLenum target, GLenum pname, GLint *params)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetQueryivARB)
				pGlGetQueryivARB(target, pname, params);
#elif defined(GL_ARB_occlusion_query)
			glGetQueryivARB(target, pname, params);
#else
			os::Printer::log("glGetQueryivARB not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetQueryObjectivARB)
				pGlGetQueryObjectivARB(id, pname, params);
			else if (pGlGetOcclusionQueryivNV)
				pGlGetOcclusionQueryivNV(id, pname, params);
#elif defined(GL_ARB_occlusion_query)
			glGetQueryObjectivARB(id, pname, params);
#elif defined(GL_NV_occlusion_query)
			glGetOcclusionQueryivNV(id, pname, params);
#else
			os::Printer::log("glGetQueryObjectiv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlGetQueryObjectuivARB)
				pGlGetQueryObjectuivARB(id, pname, params);
			else if (pGlGetOcclusionQueryuivNV)
				pGlGetOcclusionQueryuivNV(id, pname, params);
#elif defined(GL_ARB_occlusion_query)
			glGetQueryObjectuivARB(id, pname, params);
#elif defined(GL_NV_occlusion_query)
			glGetOcclusionQueryuivNV(id, pname, params);
#else
			os::Printer::log("glGetQueryObjectuiv not supported", ELL_ERROR);
#endif
		}

		inline void COpenGLExtensionHandler::extGlSwapInterval(int interval)
		{
			// we have wglext, so try to use that
#if defined(_SAPPHIRE_WINDOWS_API_) && defined(_SAPPHIRE_COMPILE_WITH_WINDOWS_DEVICE_)
#ifdef WGL_EXT_swap_control
			if (pWglSwapIntervalEXT)
				pWglSwapIntervalEXT(interval);
#endif
#endif
#ifdef _SAPPHIRE_COMPILE_WITH_X11_DEVICE_
			//TODO: Check GLX_EXT_swap_control and GLX_MESA_swap_control
#ifdef GLX_SGI_swap_control
			// does not work with interval==0
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (interval && pGlxSwapIntervalSGI)
				pGlxSwapIntervalSGI(interval);
#else
			if (interval)
				glXSwapIntervalSGI(interval);
#endif
#elif defined(GLX_EXT_swap_control)
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			Display *dpy = glXGetCurrentDisplay();
			GLXDrawable drawable = glXGetCurrentDrawable();

			if (pGlxSwapIntervalEXT)
				pGlxSwapIntervalEXT(dpy, drawable, interval);
#else
			pGlXSwapIntervalEXT(dpy, drawable, interval);
#endif
#elif defined(GLX_MESA_swap_control)
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlxSwapIntervalMESA)
				pGlxSwapIntervalMESA(interval);
#else
			pGlXSwapIntervalMESA(interval);
#endif
#endif
#endif
		}

		inline void COpenGLExtensionHandler::extGlBlendEquation(GLenum mode)
		{
#ifdef SAPPHIRE_OPENGL_USE_EXTPOINTER
			if (pGlBlendEquation)
				pGlBlendEquation(mode);
			else if (pGlBlendEquationEXT)
				pGlBlendEquationEXT(mode);
#elif defined(GL_EXT_blend_minmax) || defined(GL_EXT_blend_subtract) || defined(GL_EXT_blend_logic_op)
			glBlendEquationEXT(mode);
#elif defined(GL_VERSION_1_2)
			glBlendEquation(mode);
#else
			os::Printer::log("glBlendEquation not supported", ELL_ERROR);
#endif
		}


	
}

#endif



#endif