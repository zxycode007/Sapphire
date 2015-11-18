#ifndef _SAPPHIRE_MEOMORY_ALLOCATOR_CONFIG_
#define _SAPPHIRE_MEOMORY_ALLOCATOR_CONFIG_

#include "SapphireMemoryAllocatedObject.h"


/*

����ļ�����Sapphire���е��ڴ�������������ΪSapphire�������޸ķ�����Ϊ

���Զ����ڴ������������Ҫ�ṩһ���������Զ����������࣬��Щ����Ҫ����ʵ��

@code
// �����ֽ� - file/line/func ��Ϣ��ѡ
static inline void* allocateBytes(size_t count, const char* file = 0, int line = 0, const char* func = 0);
// �ͷ��ֽ�
static inline void deallocateBytes(void* ptr);
// ���ص����������п��Է��������ֽ���
static inline size_t getMaxAllocationSize();
@endcode

���Ե�ʵ�����ڵ��ð�װ��ͺꡣ AllocatedObj��һ���า����new��delete��������֮��ֱ���ɲ������������䷽��
STLAllocator��һ��Ϊ�˹ҽ�������������ķ��������������,���ṩSTL��������

SAPPHIRE_NEW
���ڷ���һ�������Զ����new��delete�����Ķ����������Զ���ķ��䣬ͨ������ζ��������Sapphire����AllocatedObject����������
�ͷ��ڴ���SAPPHIRE_DELETE, ʵ������Ҳ�����ñ�׼��new��delete�����������ܻ�ȡ�кŵ�debug��Ϣ��
�ڴ�Ŀ¼������Щ�������Զ��������������ķ��������㶼����ָ����

SAPPHIRE_NEW_T
���ڷ���һ�����������ṹ�壬���ǲ����Զ����new��delete��������һ������(Vector3,Quaternion)���������ⲿ�Ŀ⣨��STL��
�����ϣ������һ�����������������SAPPHIRE_DELETE_T�����ͷš� ����㲻��Ҫ�������������������SAPPHIRE_FREE���ͷ��ڴ棬
��Щ��ȷ���˹��������������ܹ�����ȷ�ĵ��á� ����ע�⣺��ͬʱ����ʱ������Ҫ֪�����ͺ��ڴ�Ŀ¼��ʹ����ȷ�ķ�������ȡ�á�

SAPPHIRE_NEW_ARRAY_T
�� SAPPHIRE_NEW_T�����һ������Ĳ�����������ʵ�����������ڴ档�ͷ���SAPPHIRE_DELETE_ARRAY_T

SAPPHIRE_ALLOC_T
���ڰ�ȫ����һ���������͵ļ��ϣ���ʱ����Ҫ��Ч�ķ���һ�鲻�õ���ÿ��Ԫ�ع���/���������ڴ档
�ͷ���SAPPHIRE_FREE

SAPPHIRE_MALLOC
������ԭʼ�ķ���ķ�������һ���ֽڵļ��ϣ���SAPPHIRE_FREE���ͷ�

����
@code
/// AllocatedObject ֮��, ���Զ����new��delete
AllocatedClass* obj = SAPPHIRE_NEW AllocatedClass();
SAPPHIRE_DELETE obj;
AllocatedClass* array = SAPPHIRE_NEW AllocatedClass[10];
SAPPHIRE_DELETE [] obj;
/// ��������ⲿ�� ��Ҫ���ù���/��������
ExternalClass* obj = SAPPHIRE_NEW_T(ExternalClass, MEMCATEGORY_GENERAL)(constructorArgs);
SAPPHIRE_DELETE_T(obj, ExternalClass, MEMCATEGORY_GENERAL);
ExternalClass* obj = SAPPHIRE_NEW_ARRAY_T(ExternalClass, 10, MEMCATEGORY_GENERAL);
SAPPHIRE_DELETE_ARRAY_T(obj, NonVirtualClass, 10, MEMCATEGORY_GENERAL);
/// ��������
long* pLong = SAPPHIRE_ALLOC_T(long, 10, MEMCATEGORY_GENERAL);
SAPPHIRE_FREE(pLong, MEMCATEGORY_GENERAL);
/// ʹ�ù������Ļ�������Primitive type with constructor (����û����������������Բ�ƥ��SAPPHIRE_NEW_T �� SAPPHIRE_FREE)
long* pLong = SAPPHIRE_NEW_T(long, MEMCATEGORY_GENERAL)(0);
SAPPHIRE_FREE(pLong, MEMCATEGORY_GENERAL);
/// ԭʼ�ڴ�
void* pVoid = SAPPHIRE_MALLOC(1024, MEMCATEGORY_GENERAL);
SAPPHIRE_FREE(pVoid, MEMCATEGORY_GENERAL);
@endcode
SAPPHIRE_ALLOC_T �����ڻ���������int��float�ķ��䷽���������ʹ��SAPPHIRE_FREE�������ͷţ�SIMD�������Զ������ı���


*/





namespace Sapphire
{

	/**
	ָ��һ��Ҫ���з����ڴ��Ŀ�ĵļ�������
	���Ŀ¼���ڷ���ʱ�ṩ������ԵĲ�ͬ��Ϊ������������һ�������ԣ�����Ȼ�����в�ͬ����Ϊ��
	���˲�ͬ�Ĳ��Ե���ͬ�����⣬���Ƽ�������Ϊ������ߵļ���
	*/
	enum MemoryCategory
	{
		/// һ��Ӧ��
		MEMCATEGORY_GENERAL = 0,
		/// ���������ڴ��еļ�����
		MEMCATEGORY_GEOMETRY = 1,
		/// ���ڶ������ݣ�����٣���������
		MEMCATEGORY_ANIMATION = 2,
		/// �ڵ㣬��������
		MEMCATEGORY_SCENE_CONTROL = 3,
		/// ��������ʵ��
		MEMCATEGORY_SCENE_OBJECTS = 4,
		/// ������Դ
		MEMCATEGORY_RESOURCE = 5,
		/// �ű�
		MEMCATEGORY_SCRIPTING = 6,
		/// ��Ⱦϵͳ�ṹ
		MEMCATEGORY_RENDERSYS = 7,


		//���ã�����ֵ
		MEMCATEGORY_COUNT = 8
	};

}

#include "SapphireMemorySTLAllocator.h"

#if SAPPHIRE_MEMORY_ALLOCATOR == SAPPHIRE_MEMORY_ALLOCATOR_NEDPOOLING       //�ж��ڴ���������õ�����NEDPOOLING

//�����ned �ڴ�أ���ô������ص�ͷ�ļ�
#include "SapphireMemoryNedPooling.h"
namespace Sapphire
{
	// ���û������ѡ��Ĭ�ϵķ�����
	// ע������������ʹ������ڴ����࣬������Ȼ�������뵽��������У������ѡ��Ϊ��ͬ�ڴ���;ѡ��ͬ�ķ������

	// �������࣬����ͨ�õķ����ڴ�
	// ע��������������Ч��
	template <MemoryCategory Cat> class CategorisedAllocPolicy : public NedPoolingPolicy{};   //�����ڴ����������� ģ���� CategorisedAllocPolicy �̳���Ned�ز���NedPoolingPolicy
	template <MemoryCategory Cat, size_t align = 0> class CategorisedAlignAllocPolicy : public NedPoolingAlignedPolicy<align>{};  //�������ֽڶ����
}

#elif SAPPHIRE_MEMORY_ALLOCATOR == SAPPHIRE_MEMORY_ALLOCATOR_NED

//#  include "SapphireMemoryNedAlloc.h"
//namespace Sapphire
//{
//	// ���û������ѡ��Ĭ�ϵķ�����
//	// ע������������ʹ������ڴ����࣬������Ȼ�������뵽��������У������ѡ��Ϊ��ͬ�ڴ���;ѡ��ͬ�ķ������

	// �������࣬����ͨ�õķ����ڴ�
	// ע��������������Ч��
//	template <MemoryCategory Cat> class CategorisedAllocPolicy : public NedAllocPolicy{};  //�����ڴ����������� ģ���� CategorisedAllocPolicy �̳���Ned����NedAllocPolicy
//	template <MemoryCategory Cat, size_t align = 0> class CategorisedAlignAllocPolicy : public NedAlignedAllocPolicy<align>{};
//}

#  include "SapphireMemoryStdAlloc.h"
namespace Sapphire
{
	// ���û������ѡ��Ĭ�ϵķ�����
	// ע������������ʹ������ڴ����࣬������Ȼ�������뵽��������У������ѡ��Ϊ��ͬ�ڴ���;ѡ��ͬ�ķ������

	// �������࣬����ͨ�õķ����ڴ�
	// ע��������������Ч��
	template <MemoryCategory Cat> class CategorisedAllocPolicy : public StdAllocPolicy{};
	template <MemoryCategory Cat, size_t align = 0> class CategorisedAlignAllocPolicy : public StdAlignedAllocPolicy < align > {};

	// ���ҪΪÿ���ڴ�����ƶ����䣬�������
	// template <> class CategorisedAllocPolicy<MEMCATEGORY_SCENE_OBJECTS> : public YourSceneObjectAllocPolicy{};
	// template <size_t align> class CategorisedAlignAllocPolicy<MEMCATEGORY_SCENE_OBJECTS, align> : public YourSceneObjectAllocPolicy<align>{};


}

#elif SAPPHIRE_MEMORY_ALLOCATOR == SAPPHIRE_MEMORY_ALLOCATOR_STD

#  include "SapphireMemoryStdAlloc.h"
namespace Sapphire
{
	// ���û������ѡ��Ĭ�ϵķ�����
	// ע������������ʹ������ڴ����࣬������Ȼ�������뵽��������У������ѡ��Ϊ��ͬ�ڴ���;ѡ��ͬ�ķ������

	// �������࣬����ͨ�õķ����ڴ�
	// ע��������������Ч��
	template <MemoryCategory Cat> class CategorisedAllocPolicy : public StdAllocPolicy{};  //��׼�������,��Ҫ�Բ�ͬ���Ͷ��󣬽��в�ͬ�����ڴ������Զ�������
	//template <> class CategorisedAllocPolicy<MEMCATEGORY_TYPE> : public YouCustomAllocPolicy{};
	template <MemoryCategory Cat, size_t align = 0> class CategorisedAlignAllocPolicy : public StdAlignedAllocPolicy < align > {};

	// ���ҪΪÿ���ڴ�����ƶ����䣬�������
	// template <> class CategorisedAllocPolicy<MEMCATEGORY_SCENE_OBJECTS> : public YourSceneObjectAllocPolicy{};
	// template <size_t align> class CategorisedAlignAllocPolicy<MEMCATEGORY_SCENE_OBJECTS, align> : public YourSceneObjectAllocPolicy<align>{};


}

#else

// �������Խ��ķ�����

#endif

namespace Sapphire
{
	// ��Ӧ�ķ������Ͳ��Կ�ݷ�ʽ
	typedef CategorisedAllocPolicy<MEMCATEGORY_GENERAL> GeneralAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_GEOMETRY> GeometryAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_ANIMATION> AnimationAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_SCENE_CONTROL> SceneCtlAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_SCENE_OBJECTS> SceneObjAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_RESOURCE> ResourceAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_SCRIPTING> ScriptingAllocPolicy;
	typedef CategorisedAllocPolicy<MEMCATEGORY_RENDERSYS> RenderSysAllocPolicy;

	// ����Ϊÿ�����乤���������еĻ���
	typedef AllocatedObj<GeneralAllocPolicy> GeneralAllocatedObject;
	typedef AllocatedObj<GeometryAllocPolicy> GeometryAllocatedObject;
	typedef AllocatedObj<AnimationAllocPolicy> AnimationAllocatedObject;
	typedef AllocatedObj<SceneCtlAllocPolicy> SceneCtlAllocatedObject;
	typedef AllocatedObj<SceneObjAllocPolicy> SceneObjAllocatedObject;
	typedef AllocatedObj<ResourceAllocPolicy> ResourceAllocatedObject;
	typedef AllocatedObj<ScriptingAllocPolicy> ScriptingAllocatedObject;
	typedef AllocatedObj<RenderSysAllocPolicy> RenderSysAllocatedObject;


	// ����ÿ����������
	// NOTE: С�������� ������Ϊ������������
	// ʹ��ԭ����STL������/��������
	typedef ScriptingAllocatedObject	AbstractNodeAlloc;
	typedef AnimationAllocatedObject	AnimableAlloc;
	typedef AnimationAllocatedObject	AnimationAlloc;
	typedef GeneralAllocatedObject		ArchiveAlloc;
	typedef GeometryAllocatedObject		BatchedGeometryAlloc;
	typedef RenderSysAllocatedObject	BufferAlloc;
	typedef GeneralAllocatedObject		CodecAlloc;
	typedef ResourceAllocatedObject		CompositorInstAlloc;
	typedef GeneralAllocatedObject		ConfigAlloc;
	typedef GeneralAllocatedObject		ControllerAlloc;
	typedef GeometryAllocatedObject		DebugGeomAlloc;
	typedef GeneralAllocatedObject		DynLibAlloc;
	typedef GeometryAllocatedObject		EdgeDataAlloc;
	typedef GeneralAllocatedObject		FactoryAlloc;
	typedef SceneObjAllocatedObject		FXAlloc;
	typedef GeneralAllocatedObject		ImageAlloc;
	typedef GeometryAllocatedObject		IndexDataAlloc;
	typedef GeneralAllocatedObject		LogAlloc;
	typedef SceneObjAllocatedObject		MovableAlloc;
	typedef SceneCtlAllocatedObject		NodeAlloc;
	typedef SceneObjAllocatedObject		OverlayAlloc;
	typedef RenderSysAllocatedObject	GpuParamsAlloc;
	typedef ResourceAllocatedObject		PassAlloc;
	typedef GeometryAllocatedObject		PatchAlloc;
	typedef GeneralAllocatedObject		PluginAlloc;
	typedef GeneralAllocatedObject		ProfilerAlloc;
	typedef GeometryAllocatedObject		ProgMeshAlloc;
	typedef SceneCtlAllocatedObject		RenderQueueAlloc;
	typedef RenderSysAllocatedObject	RenderSysAlloc;
	typedef GeneralAllocatedObject		RootAlloc;
	typedef ResourceAllocatedObject		ResourceAlloc;
	typedef GeneralAllocatedObject		SerializerAlloc;
	typedef SceneCtlAllocatedObject		SceneMgtAlloc;
	typedef ScriptingAllocatedObject    ScriptCompilerAlloc;
	typedef ScriptingAllocatedObject    ScriptTranslatorAlloc;
	typedef SceneCtlAllocatedObject		ShadowDataAlloc;
	typedef GeneralAllocatedObject		StreamAlloc;
	typedef SceneObjAllocatedObject		SubEntityAlloc;
	typedef ResourceAllocatedObject		SubMeshAlloc;
	typedef ResourceAllocatedObject		TechniqueAlloc;
	typedef GeneralAllocatedObject		TimerAlloc;
	typedef ResourceAllocatedObject		TextureUnitStateAlloc;
	typedef GeneralAllocatedObject		UtilityAlloc;
	typedef GeometryAllocatedObject		VertexDataAlloc;
	typedef RenderSysAllocatedObject	ViewportAlloc;
	typedef SceneCtlAllocatedObject		LodAlloc;
	typedef GeneralAllocatedObject		SceneAlloc;
	typedef GeneralAllocatedObject		VertexAlloc;

	// ����
	// ��typedef STLAllocator<T, DefaultAllocPolicy, Category> TAlloc;����
	// �� vector<T, TAlloc>::type ��ʹ��



}

//���ߺ���
namespace Sapphire
{


	/** ����һ��placement new �Ķ�������ĺ���,ʵ���ǵ������������ʹ��new[]������ʵ���飬�����չ����һ���Ƿ��Ķ��������ڴ棬���������������Զ���ķ�����
	*/
	template<typename T>
	T* constructN(T* basePtr, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			new ((void*)(basePtr + i)) T();
		}
		return basePtr;
	}


}


#if SAPPHIRE_DEBUG_MODE      //DEBUGģʽ����

/// ֱ�ӷ���һ��raw�ڴ�飬����ָʾ��;����
#	define SAPPHIRE_MALLOC(bytes, category) ::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__)
/// ֱ�ӷ�����������͵��ڴ�飬��ָʾ����;
#	define SAPPHIRE_ALLOC_T(T, count, category) static_cast<T*>(::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))
/// �ͷ���SAPPHIRE_MALLOC �� SAPPHIRE_ALLOC_T ������ڴ棬��Ҫȷ������;��Ӧ�Ĳ���
#	define SAPPHIRE_FREE(ptr, category) ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr)

/// ��placement new����һ���������ͣ���չ���ͻ��߹����������÷��������
#	define SAPPHIRE_NEW_T(T, category) new (::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
/// ����һ��count��С�Ļ������͵��ڴ�飬 Sapphire::constructN�������Լ̳���AllocatedObj����
#	define SAPPHIRE_NEW_ARRAY_T(T, count, category) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count) 
/// �ͷ���SAPPHIRE_NEW_T������ڴ�
#	define SAPPHIRE_DELETE_T(ptr, T, category) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}
/// �ͷ�SAPPHIRE_NEW_ARRAY_T������ڴ�
#	define SAPPHIRE_DELETE_ARRAY_T(ptr, T, count, category) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}

//�������
/// ����һ�����뵽SIMD�߽��RAW�ڴ��,����ָ������ʹ������
#	define SAPPHIRE_MALLOC_SIMD(bytes, category) ::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__)  //align Ĭ��=0
/// ����һ��raw�ڴ������û�����ı߽磬����ָʾ����ʹ�õ�����
#	define SAPPHIRE_MALLOC_ALIGN(bytes, category, align) ::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__)
/// ����һ�����뵽SIMD�߽�Ļ������͵��ڴ�飬����ָʾ����ʹ������
#	define SAPPHIRE_ALLOC_T_SIMD(T, count, category) static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)) //align Ĭ��=0
///����һ�����뵽�û�����߽�Ļ������͵��ڴ�飬����ָʾ����ʹ������
#	define SAPPHIRE_ALLOC_T_ALIGN(T, count, category, align) static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))
/// �ͷ�SAPPHIRE_MALLOC_SIMD SAPPHIRE_ALLOC_T_SIMD������ڴ棬ʹ��������Ҫ˵�����Ա����
#	define SAPPHIRE_FREE_SIMD(ptr, category) ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes(ptr)
/// �ͷ�SAPPHIRE_MALLOC_ALIGN SAPPHIRE_ALLOC_T_ALIGN  ������ڴ棬ʹ��������Ҫ˵�����Ա����
#	define SAPPHIRE_FREE_ALIGN(ptr, category, align) ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes(ptr)

/// ��placement new ����һ���������ͣ� ��������ͻ��߷��������Ͷ��뵽SIMD�߽�
#	define SAPPHIRE_NEW_T_SIMD(T, category) new (::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
/// ����ָ�������Ķ��뵽SIMD�߽�������͵�һ���ڴ�飬 Sapphire::constructN����ʹ���ڼ̳�AllocatedObj����
#	define SAPPHIRE_NEW_ARRAY_T_SIMD(T, count, category) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count) 
/// �ͷ���SAPPHIRE_NEW_T_SIMD������ڴ�
#	define SAPPHIRE_DELETE_T_SIMD(ptr, T, category) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes(ptr);}
/// �ͷ���SAPPHIRE_NEW_ARRAY_T_SIMD �����һϵ���ڴ棬 ���ε�����������
#	define SAPPHIRE_DELETE_ARRAY_T_SIMD(ptr, T, count, category) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes(ptr);}
/// ��placement new����һ���������ͣ��������ͻ�������Ͳ������û��Ķ���߽�
#	define SAPPHIRE_NEW_T_ALIGN(T, category, align) new (::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
/// ����һ���������뵽�û�����߽��һ���ڴ�飬 Sapphire::constructN�������ڻ�����AllocatedObj����
#	define SAPPHIRE_NEW_ARRAY_T_ALIGN(T, count, category, align) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count) 
/// �ͷ���SAPPHIRE_NEW_T_ALIGN ������ڴ�
#	define SAPPHIRE_DELETE_T_ALIGN(ptr, T, category, align) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes(ptr);}
///�ͷ���SAPPHIRE_NEW_ARRAY_T_ALIGN������ڴ棬�����ε�����������
#	define SAPPHIRE_DELETE_ARRAY_T_ALIGN(ptr, T, count, category, align) if(ptr){for (size_t _b = 0; _b < count; ++_b) { (ptr)[_b].~T();} ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes(ptr);}

// new / delete ����������AllocatedObj���� ��������ÿ������Ծ���������ȡ���ļ�/�к�/�������Ĳ�����������AllocatedObj����ֻ��ʹ���Զ����new/delete
//����AllocatedObj������new �� delete
#	define SAPPHIRE_NEW new (__FILE__, __LINE__, __FUNCTION__)
#	define SAPPHIRE_DELETE delete


#else // Releaseģʽ


#	define SAPPHIRE_MALLOC(bytes, category) ::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(bytes)

#	define SAPPHIRE_ALLOC_T(T, count, category) static_cast<T*>(::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count)))

#	define SAPPHIRE_FREE(ptr, category) ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr)


#	define SAPPHIRE_NEW_T(T, category) new (::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T))) T

#	define SAPPHIRE_NEW_ARRAY_T(T, count, category) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count))), count) 

#	define SAPPHIRE_DELETE_T(ptr, T, category) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}

#	define SAPPHIRE_DELETE_ARRAY_T(ptr, T, count, category) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Sapphire::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}


#	define SAPPHIRE_MALLOC_SIMD(bytes, category) ::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(bytes)

#	define SAPPHIRE_MALLOC_ALIGN(bytes, category, align) ::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(bytes)

#	define SAPPHIRE_ALLOC_T_SIMD(T, count, category) static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count)))

#	define SAPPHIRE_ALLOC_T_ALIGN(T, count, category, align) static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count)))

#	define SAPPHIRE_FREE_SIMD(ptr, category) ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes((void*)ptr)

#	define SAPPHIRE_FREE_ALIGN(ptr, category, align) ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes((void*)ptr)


#	define SAPPHIRE_NEW_T_SIMD(T, category) new (::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T))) T

#	define SAPPHIRE_NEW_ARRAY_T_SIMD(T, count, category) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category>::allocateBytes(sizeof(T)*(count))), count) 

#	define SAPPHIRE_DELETE_T_SIMD(ptr, T, category) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes((void*)ptr);}

#	define SAPPHIRE_DELETE_ARRAY_T_SIMD(ptr, T, count, category) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Sapphire::CategorisedAlignAllocPolicy<category>::deallocateBytes((void*)ptr);}

#	define SAPPHIRE_NEW_T_ALIGN(T, category, align) new (::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T))) T

#	define SAPPHIRE_NEW_ARRAY_T_ALIGN(T, count, category, align) ::Sapphire::constructN(static_cast<T*>(::Sapphire::CategorisedAlignAllocPolicy<category, align>::allocateBytes(sizeof(T)*(count))), count) 

#	define SAPPHIRE_DELETE_T_ALIGN(ptr, T, category, align) if(ptr){(ptr)->~T(); ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes((void*)ptr);}

#	define SAPPHIRE_DELETE_ARRAY_T_ALIGN(ptr, T, count, category, align) if(ptr){for (size_t _b = 0; _b < count; ++_b) { (ptr)[_b].~T();} ::Sapphire::CategorisedAlignAllocPolicy<category, align>::deallocateBytes((void*)ptr);}


#	define SAPPHIRE_NEW new 
#	define SAPPHIRE_DELETE delete

#endif  


namespace Sapphire
{
	/**
	���������һ��������ָ������SAPPHIRE_DELETE
	*/
	template<typename T>
	void deletePtr(T* ptr)
	{
		SAPPHIRE_DELETE ptr;
	}
}






#endif