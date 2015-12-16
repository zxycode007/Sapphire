#ifndef _SAPPHIRE_ALIGNED_ALLOCATOR_
#define _SAPPHIRE_ALIGNED_ALLOCATOR_

namespace Sapphire {

	/** 
	
	�ṩ�����������ڴ����
	���е�SIMD������Ҫ�����ڴ棬 ��ЩSIMD�����Ϊֻ�ܶ����ڴ���ܹ����� ������ݴ���ʹ��SIMD������
	����Ҫ������ܻ�ø��ߵ����ܡ�
	���⣬��ʹ��ʹ��SIMD��������������߽��������Ҳ��������ܡ�����������ṩ��һ�Է�������ڴ�ĺ���
	
	������Σ�ͨ������£��㲻��Ҫֱ��ʹ������࣬Sapphire�ڲ���������ʹ��SIMD��cache�Ż���
	��Щ����һ����ɣ������Ǳ�ڵĹ���Ҫ��������㲻֪����Ҫ����ȥ���У���ô�������������
	
	*/
	class _SapphireExport AlignedMemory
	{
	public:
		/** �ø�������ֵ�������ڴ�
		@param
		size ��Ҫ������ڴ��С
		@param
		alignment ����ֵ�Ľ��ָ�룬����Ϊ2���ݣ���Χ������1~128
		@return
		The �����ڴ���ָ��
		@par
		On ���ʧ���׳��쳣
		*/
		static void* allocate(size_t size, size_t alignment);

		/** �����ڴ���ƽ̨Ĭ�ϵĶ���ֵ
		@remarks
		���Ĭ�ϵĶ���ֵ������SapphireĬ�ϵ��趨ֵ���������SIMD�ͻ������߽�ʹ�ö�����ڴ�
		@param
		size �����ڴ�Ĵ�С
		@return
		The �����ڴ���ָ��
		@par
		���ʧ���׳��쳣
		*/
		static void* allocate(size_t size);

		/** �ͷŵ�����������ڴ�
		@param
		p ָ�����������ڴ��ָ�����NULLָ��
		@par
		����ǿ�ָ�룬�򲻻���в���
		*/
		static void deallocate(void* p);
	};

	/** @} */
	/** @} */

}


#endif