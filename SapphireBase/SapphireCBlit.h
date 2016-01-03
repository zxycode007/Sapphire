#ifndef _SAPPHIRE_C_BLIT_
#define _SAPPHIRE_C_BLIT_

#include "SapphirePrerequisites.h"
#include "SapphireIImage.h"
#include "SapphireSoftDriverHelper.h"

namespace Sapphire
{
	// ¿éÒÆ¶¯Æ÷
	enum eBlitter
	{
		BLITTER_INVALID = 0,
		BLITTER_COLOR,
		BLITTER_COLOR_ALPHA,
		BLITTER_TEXTURE,
		BLITTER_TEXTURE_ALPHA_BLEND,
		BLITTER_TEXTURE_ALPHA_COLOR_BLEND
	};

	

	

	struct SBlitJob
	{
		AbsRectangle Dest;
		AbsRectangle Source;

		UINT32 argb;

		void * src;
		void * dst;

		SINT32 width;
		SINT32 height;

		UINT32 srcPitch;
		UINT32 dstPitch;

		UINT32 srcPixelMul;
		UINT32 dstPixelMul;

		bool stretch;
		float x_stretch;
		float y_stretch;

		SBlitJob() : stretch(false) {}
	};


	typedef void(*tExecuteBlit) (const SBlitJob * job);

	struct blitterTable
	{
		eBlitter operation;
		SINT32 destFormat;
		SINT32 sourceFormat;
		tExecuteBlit func;
	};


	#if defined(__BORLANDC__) || defined (__BCPLUSPLUS__)

	// 8-bit bools in borland builder

	//! conditional set based on mask and arithmetic shift
	FORCEINLINE UINT32 if_c_a_else_b ( const c8 condition, const UINT32 a, const UINT32 b )
	{
		return ( ( -condition >> 7 ) & ( a ^ b ) ) ^ b;
	}

	//! conditional set based on mask and arithmetic shift
	REALINLINE UINT32 if_c_a_else_0 ( const c8 condition, const UINT32 a )
	{
		return ( -condition >> 31 ) & a;
	}
#else

	//! conditional set based on mask and arithmetic shift
	FORCEINLINE UINT32 if_c_a_else_b(const SINT32 condition, const UINT32 a, const UINT32 b)
	{
		return ( ( -condition >> 31 ) & ( a ^ b ) ) ^ b;
	}

	//! conditional set based on mask and arithmetic shift
	FORCEINLINE UINT16 if_c_a_else_b(const SINT16 condition, const UINT16 a, const UINT16 b)
	{
		return ( ( -condition >> 15 ) & ( a ^ b ) ) ^ b;
	}

	//! conditional set based on mask and arithmetic shift
	FORCEINLINE UINT32 if_c_a_else_0(const SINT32 condition, const UINT32 a)
	{
		return ( -condition >> 31 ) & a;
	}
#endif

	/*!
	Scale Color by (1/value)
	value 0 - 256 ( alpha )
	*/
	inline UINT32 PixelLerp32(const UINT32 source, const UINT32 value)
	{
		UINT32 srcRB = source & 0x00FF00FF;
		UINT32 srcXG = (source & 0xFF00FF00) >> 8;

		srcRB *= value;
		srcXG *= value;

		srcRB >>= 8;
		//srcXG >>= 8;

		srcXG &= 0xFF00FF00;
		srcRB &= 0x00FF00FF;

		return srcRB | srcXG;
	}

	/*
	return alpha in [0;256] Granularity from 32-Bit ARGB
	add highbit alpha ( alpha > 127 ? + 1 )
	*/
	static inline UINT32 extractAlpha(const UINT32 c)
	{
		return (c >> 24) + (c >> 31);
	}

	static void executeBlit_TextureCopy_x_to_x(const SBlitJob * job)
	{
		const UINT32 w = job->width;
		const UINT32 h = job->height;
		if (job->stretch)
		{
			const UINT32 *src = static_cast<const UINT32*>(job->src);
			UINT32 *dst = static_cast<UINT32*>(job->dst);
			const float wscale = 1.f / job->x_stretch;
			const float hscale = 1.f / job->y_stretch;

			for (UINT32 dy = 0; dy < h; ++dy)
			{
				const UINT32 src_y = (UINT32)(dy*hscale);
				src = (UINT32*)((UINT8*)(job->src) + job->srcPitch*src_y);

				for (UINT32 dx = 0; dx < w; ++dx)
				{
					const UINT32 src_x = (UINT32)(dx*wscale);
					dst[dx] = src[src_x];
				}
				dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
			}
		}
		else
		{
			const UINT32 widthPitch = job->width * job->dstPixelMul;
			const void *src = (void*)job->src;
			void *dst = (void*)job->dst;

			for (UINT32 dy = 0; dy != h; ++dy)
			{
				memcpy(dst, src, widthPitch);

				src = (void*)((UINT8*)(src)+job->srcPitch);
				dst = (void*)((UINT8*)(dst)+job->dstPitch);
			}
		}
	}

	static void executeBlit_TextureCopy_32_to_16(const SBlitJob * job)
	{
		const UINT32 w = job->width;
		const UINT32 h = job->height;
		const UINT32 *src = static_cast<const UINT32*>(job->src);
		UINT16 *dst = static_cast<UINT16*>(job->dst);

		if (job->stretch)
		{
			const float wscale = 1.f / job->x_stretch;
			const float hscale = 1.f / job->y_stretch;

			for (UINT32 dy = 0; dy < h; ++dy)
			{
				const UINT32 src_y = (UINT32)(dy*hscale);
				src = (UINT32*)((UINT8*)(job->src) + job->srcPitch*src_y);

				for (UINT32 dx = 0; dx < w; ++dx)
				{
					const UINT32 src_x = (UINT32)(dx*wscale);
					//16 bit Blitter depends on pre-multiplied color
					const UINT32 s = PixelLerp32(src[src_x] | 0xFF000000, extractAlpha(src[src_x]));
					dst[dx] = A8R8G8B8toA1R5G5B5(s);
				}
				dst = (UINT16*)((UINT8*)(dst)+job->dstPitch);
			}
		}
		else
		{
			for (UINT32 dy = 0; dy != h; ++dy)
			{
				for (UINT32 dx = 0; dx != w; ++dx)
				{
					//16 bit Blitter depends on pre-multiplied color
					const UINT32 s = PixelLerp32(src[dx] | 0xFF000000, extractAlpha(src[dx]));
					dst[dx] = A8R8G8B8toA1R5G5B5(s);
				}

				src = (UINT32*)((UINT8*)(src)+job->srcPitch);
				dst = (UINT16*)((UINT8*)(dst)+job->dstPitch);
			}
		}
	}

	static void executeBlit_TextureCopy_24_to_16(const SBlitJob * job)
	{
		const UINT32 w = job->width;
		const UINT32 h = job->height;
		const UINT8 *src = static_cast<const UINT8*>(job->src);
		UINT16 *dst = static_cast<UINT16*>(job->dst);

		if (job->stretch)
		{
			const float wscale = 3.f / job->x_stretch;
			const float hscale = 1.f / job->y_stretch;

			for (UINT32 dy = 0; dy < h; ++dy)
			{
				const UINT32 src_y = (UINT32)(dy*hscale);
				src = (UINT8*)(job->src) + job->srcPitch*src_y;

				for (UINT32 dx = 0; dx < w; ++dx)
				{
					const UINT8* src_x = src + (UINT32)(dx*wscale);
					dst[dx] = RGBA16(src_x[0], src_x[1], src_x[2]);
				}
				dst = (UINT16*)((UINT8*)(dst)+job->dstPitch);
			}
		}
		else
		{
			for (UINT32 dy = 0; dy != h; ++dy)
			{
				const UINT8* s = src;
				for (UINT32 dx = 0; dx != w; ++dx)
				{
					dst[dx] = RGBA16(s[0], s[1], s[2]);
					s += 3;
				}

				src = src + job->srcPitch;
				dst = (UINT16*)((UINT8*)(dst)+job->dstPitch);
			}
		}
	}

	static void executeBlit_TextureCopy_16_to_32(const SBlitJob * job)
	{
		const UINT32 w = job->width;
		const UINT32 h = job->height;
		const UINT16 *src = static_cast<const UINT16*>(job->src);
		UINT32 *dst = static_cast<UINT32*>(job->dst);

		if (job->stretch)
		{
			const float wscale = 1.f / job->x_stretch;
			const float hscale = 1.f / job->y_stretch;

			for (UINT32 dy = 0; dy < h; ++dy)
			{
				const UINT32 src_y = (UINT32)(dy*hscale);
				src = (UINT16*)((UINT8*)(job->src) + job->srcPitch*src_y);

				for (UINT32 dx = 0; dx < w; ++dx)
				{
					const UINT32 src_x = (UINT32)(dx*wscale);
					dst[dx] = A1R5G5B5toA8R8G8B8(src[src_x]);
				}
				dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
			}
		}
		else
		{
			for (UINT32 dy = 0; dy != h; ++dy)
			{
				for (UINT32 dx = 0; dx != w; ++dx)
				{
					dst[dx] = A1R5G5B5toA8R8G8B8(src[dx]);
				}

				src = (UINT16*)((UINT8*)(src)+job->srcPitch);
				dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
			}
		}
	}

	static void executeBlit_TextureCopy_24_to_32(const SBlitJob * job)
	{
		const UINT32 w = job->width;
		const UINT32 h = job->height;
		const UINT8 *src = static_cast<const UINT8*>(job->src);
		UINT32 *dst = static_cast<UINT32*>(job->dst);

		if (job->stretch)
		{
			const float wscale = 3.f / job->x_stretch;
			const float hscale = 1.f / job->y_stretch;

			for (UINT32 dy = 0; dy < h; ++dy)
			{
				const UINT32 src_y = (UINT32)(dy*hscale);
				src = (const UINT8*)job->src + (job->srcPitch*src_y);

				for (UINT32 dx = 0; dx < w; ++dx)
				{
					const UINT8* s = src + (UINT32)(dx*wscale);
					dst[dx] = 0xFF000000 | s[0] << 16 | s[1] << 8 | s[2];
				}
				dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
			}
		}
		else
		{
			for (SINT32 dy = 0; dy != job->height; ++dy)
			{
				const UINT8* s = src;

				for (SINT32 dx = 0; dx != job->width; ++dx)
				{
					dst[dx] = 0xFF000000 | s[0] << 16 | s[1] << 8 | s[2];
					s += 3;
				}

				src = src + job->srcPitch;
				dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
			}
		}
	}


	static void executeBlit_TextureCopy_16_to_24(const SBlitJob * job)
	{
		const UINT32 w = job->width;
		const UINT32 h = job->height;
		const UINT16 *src = static_cast<const UINT16*>(job->src);
		UINT8 *dst = static_cast<UINT8*>(job->dst);

		if (job->stretch)
		{
			const float wscale = 1.f / job->x_stretch;
			const float hscale = 1.f / job->y_stretch;

			for (UINT32 dy = 0; dy < h; ++dy)
			{
				const UINT32 src_y = (UINT32)(dy*hscale);
				src = (UINT16*)((UINT8*)(job->src) + job->srcPitch*src_y);

				for (UINT32 dx = 0; dx < w; ++dx)
				{
					const UINT32 src_x = (UINT32)(dx*wscale);
					UINT32 color = A1R5G5B5toA8R8G8B8(src[src_x]);
					UINT8 * writeTo = &dst[dx * 3];
					*writeTo++ = (color >> 16) & 0xFF;
					*writeTo++ = (color >> 8) & 0xFF;
					*writeTo++ = color & 0xFF;
				}
				dst += job->dstPitch;
			}
		}
		else
		{
			for (UINT32 dy = 0; dy != h; ++dy)
			{
				for (UINT32 dx = 0; dx != w; ++dx)
				{
					UINT32 color = A1R5G5B5toA8R8G8B8(src[dx]);
					UINT8 * writeTo = &dst[dx * 3];
					*writeTo++ = (color >> 16) & 0xFF;
					*writeTo++ = (color >> 8) & 0xFF;
					*writeTo++ = color & 0xFF;
				}

				src = (UINT16*)((UINT8*)(src)+job->srcPitch);
				dst += job->dstPitch;
			}
		}
	}

	static void executeBlit_TextureCopy_32_to_24(const SBlitJob * job)
	{
		const UINT32 w = job->width;
		const UINT32 h = job->height;
		const UINT32 *src = static_cast<const UINT32*>(job->src);
		UINT8 *dst = static_cast<UINT8*>(job->dst);

		if (job->stretch)
		{
			const float wscale = 1.f / job->x_stretch;
			const float hscale = 1.f / job->y_stretch;

			for (UINT32 dy = 0; dy < h; ++dy)
			{
				const UINT32 src_y = (UINT32)(dy*hscale);
				src = (UINT32*)((UINT8*)(job->src) + job->srcPitch*src_y);

				for (UINT32 dx = 0; dx < w; ++dx)
				{
					const UINT32 src_x = src[(UINT32)(dx*wscale)];
					UINT8 * writeTo = &dst[dx * 3];
					*writeTo++ = (src_x >> 16) & 0xFF;
					*writeTo++ = (src_x >> 8) & 0xFF;
					*writeTo++ = src_x & 0xFF;
				}
				dst += job->dstPitch;
			}
		}
		else
		{
			for (UINT32 dy = 0; dy != h; ++dy)
			{
				for (UINT32 dx = 0; dx != w; ++dx)
				{
					UINT8 * writeTo = &dst[dx * 3];
					*writeTo++ = (src[dx] >> 16) & 0xFF;
					*writeTo++ = (src[dx] >> 8) & 0xFF;
					*writeTo++ = src[dx] & 0xFF;
				}

				src = (UINT32*)((UINT8*)(src)+job->srcPitch);
				dst += job->dstPitch;
			}
		}
	}

	static void executeBlit_TextureBlend_16_to_16(const SBlitJob * job)
	{
		const UINT32 w = job->width;
		const UINT32 h = job->height;
		const UINT32 rdx = w >> 1;

		const UINT32 *src = (UINT32*)job->src;
		UINT32 *dst = (UINT32*)job->dst;

		if (job->stretch)
		{
			const float wscale = 1.f / job->x_stretch;
			const float hscale = 1.f / job->y_stretch;
			const UINT32 off = if_c_a_else_b(w & 1, (UINT32)((w - 1)*wscale), 0);
			for (UINT32 dy = 0; dy < h; ++dy)
			{
				const UINT32 src_y = (UINT32)(dy*hscale);
				src = (UINT32*)((UINT8*)(job->src) + job->srcPitch*src_y);

				for (UINT32 dx = 0; dx < rdx; ++dx)
				{
					const UINT32 src_x = (UINT32)(dx*wscale);
					dst[dx] = PixelBlend16_simd(dst[dx], src[src_x]);
				}
				if (off)
				{
					((UINT16*)dst)[off] = PixelBlend16(((UINT16*)dst)[off], ((UINT16*)src)[off]);
				}

				dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
			}
		}
		else
		{
			const UINT32 off = if_c_a_else_b(w & 1, w - 1, 0);
			for (UINT32 dy = 0; dy != h; ++dy)
			{
				for (UINT32 dx = 0; dx != rdx; ++dx)
				{
					dst[dx] = PixelBlend16_simd(dst[dx], src[dx]);
				}

				if (off)
				{
					((UINT16*)dst)[off] = PixelBlend16(((UINT16*)dst)[off], ((UINT16*)src)[off]);
				}

				src = (UINT32*)((UINT8*)(src)+job->srcPitch);
				dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
			}
		}
	}

	static void executeBlit_TextureBlend_32_to_32(const SBlitJob * job)
	{
		const UINT32 w = job->width;
		const UINT32 h = job->height;
		const UINT32 *src = (UINT32*)job->src;
		UINT32 *dst = (UINT32*)job->dst;

		if (job->stretch)
		{
			const float wscale = 1.f / job->x_stretch;
			const float hscale = 1.f / job->y_stretch;
			for (UINT32 dy = 0; dy < h; ++dy)
			{
				const UINT32 src_y = (UINT32)(dy*hscale);
				src = (UINT32*)((UINT8*)(job->src) + job->srcPitch*src_y);

				for (UINT32 dx = 0; dx < w; ++dx)
				{
					const UINT32 src_x = (UINT32)(dx*wscale);
					dst[dx] = PixelBlend32(dst[dx], src[src_x]);
				}

				dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
			}
		}
		else
		{
			for (UINT32 dy = 0; dy != h; ++dy)
			{
				for (UINT32 dx = 0; dx != w; ++dx)
				{
					dst[dx] = PixelBlend32(dst[dx], src[dx]);
				}
				src = (UINT32*)((UINT8*)(src)+job->srcPitch);
				dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
			}
		}
	}

	static void executeBlit_TextureBlendColor_16_to_16(const SBlitJob * job)
	{
		UINT16 *src = (UINT16*)job->src;
		UINT16 *dst = (UINT16*)job->dst;

		UINT16 blend = A8R8G8B8toA1R5G5B5(job->argb);
		for (SINT32 dy = 0; dy != job->height; ++dy)
		{
			for (SINT32 dx = 0; dx != job->width; ++dx)
			{
				if (0 == (src[dx] & 0x8000))
					continue;

				dst[dx] = PixelMul16_2(src[dx], blend);
			}
			src = (UINT16*)((UINT8*)(src)+job->srcPitch);
			dst = (UINT16*)((UINT8*)(dst)+job->dstPitch);
		}
	}

	/*!
	*/
	static void executeBlit_TextureBlendColor_32_to_32(const SBlitJob * job)
	{
		UINT32 *src = (UINT32*)job->src;
		UINT32 *dst = (UINT32*)job->dst;

		for (SINT32 dy = 0; dy != job->height; ++dy)
		{
			for (SINT32 dx = 0; dx != job->width; ++dx)
			{
				dst[dx] = PixelBlend32(dst[dx], PixelMul32_2(src[dx], job->argb));
			}
			src = (UINT32*)((UINT8*)(src)+job->srcPitch);
			dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
		}
	}

	static void executeBlit_Color_16_to_16(const SBlitJob * job)
	{
		const UINT16 c = A8R8G8B8toA1R5G5B5(job->argb);
		UINT16 *dst = (UINT16*)job->dst;

		for (SINT32 dy = 0; dy != job->height; ++dy)
		{
			memset16(dst, c, job->srcPitch);
			dst = (UINT16*)((UINT8*)(dst)+job->dstPitch);
		}
	}

	/*!
	*/
	static void executeBlit_Color_32_to_32(const SBlitJob * job)
	{
		UINT32 *dst = (UINT32*)job->dst;

		for (SINT32 dy = 0; dy != job->height; ++dy)
		{
			memset32(dst, job->argb, job->srcPitch);
			dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
		}
	}

	/*!
	*/
	static void executeBlit_ColorAlpha_16_to_16(const SBlitJob * job)
	{
		UINT16 *dst = (UINT16*)job->dst;

		const UINT16 alpha = extractAlpha(job->argb) >> 3;
		if (0 == alpha)
			return;
		const UINT32 src = A8R8G8B8toA1R5G5B5(job->argb);

		for (SINT32 dy = 0; dy != job->height; ++dy)
		{
			for (SINT32 dx = 0; dx != job->width; ++dx)
			{
				dst[dx] = 0x8000 | PixelBlend16(dst[dx], src, alpha);
			}
			dst = (UINT16*)((UINT8*)(dst)+job->dstPitch);
		}
	}

	/*!
	*/
	static void executeBlit_ColorAlpha_32_to_32(const SBlitJob * job)
	{
		UINT32 *dst = (UINT32*)job->dst;

		const UINT32 alpha = extractAlpha(job->argb);
		const UINT32 src = job->argb;

		for (SINT32 dy = 0; dy != job->height; ++dy)
		{
			for (SINT32 dx = 0; dx != job->width; ++dx)
			{
				dst[dx] = (job->argb & 0xFF000000) | PixelBlend32(dst[dx], src, alpha);
			}
			dst = (UINT32*)((UINT8*)(dst)+job->dstPitch);
		}
	}

	static const blitterTable blitTable[] =
	{
		{ BLITTER_TEXTURE, -2, -2, executeBlit_TextureCopy_x_to_x },
		{ BLITTER_TEXTURE, ECF_A1R5G5B5, ECF_A8R8G8B8, executeBlit_TextureCopy_32_to_16 },
		{ BLITTER_TEXTURE, ECF_A1R5G5B5, ECF_R8G8B8, executeBlit_TextureCopy_24_to_16 },
		{ BLITTER_TEXTURE, ECF_A8R8G8B8, ECF_A1R5G5B5, executeBlit_TextureCopy_16_to_32 },
		{ BLITTER_TEXTURE, ECF_A8R8G8B8, ECF_R8G8B8, executeBlit_TextureCopy_24_to_32 },
		{ BLITTER_TEXTURE, ECF_R8G8B8, ECF_A1R5G5B5, executeBlit_TextureCopy_16_to_24 },
		{ BLITTER_TEXTURE, ECF_R8G8B8, ECF_A8R8G8B8, executeBlit_TextureCopy_32_to_24 },
		{ BLITTER_TEXTURE_ALPHA_BLEND, ECF_A1R5G5B5, ECF_A1R5G5B5, executeBlit_TextureBlend_16_to_16 },
		{ BLITTER_TEXTURE_ALPHA_BLEND, ECF_A8R8G8B8, ECF_A8R8G8B8, executeBlit_TextureBlend_32_to_32 },
		{ BLITTER_TEXTURE_ALPHA_COLOR_BLEND, ECF_A1R5G5B5, ECF_A1R5G5B5, executeBlit_TextureBlendColor_16_to_16 },
		{ BLITTER_TEXTURE_ALPHA_COLOR_BLEND, ECF_A8R8G8B8, ECF_A8R8G8B8, executeBlit_TextureBlendColor_32_to_32 },
		{ BLITTER_COLOR, ECF_A1R5G5B5, -1, executeBlit_Color_16_to_16 },
		{ BLITTER_COLOR, ECF_A8R8G8B8, -1, executeBlit_Color_32_to_32 },
		{ BLITTER_COLOR_ALPHA, ECF_A1R5G5B5, -1, executeBlit_ColorAlpha_16_to_16 },
		{ BLITTER_COLOR_ALPHA, ECF_A8R8G8B8, -1, executeBlit_ColorAlpha_32_to_32 },
		{ BLITTER_INVALID, -1, -1, 0 }
	};


	// bounce clipping to texture
	inline void setClip(AbsRectangle &out, const rect<SINT32> *clip,
		const IImage * tex, SINT32 passnative)
	{
		if (clip && 0 == tex && passnative)
		{
			out.x0 = clip->UpperLeftCorner.x;
			out.x1 = clip->LowerRightCorner.x;
			out.y0 = clip->UpperLeftCorner.y;
			out.y1 = clip->LowerRightCorner.y;
			return;
		}

		const SINT32 w = tex ? tex->getDimension().Width : 0;
		const SINT32 h = tex ? tex->getDimension().Height : 0;
		if (clip)
		{
			out.x0 = SINT32_clamp(clip->UpperLeftCorner.x, 0, w);
			out.x1 = SINT32_clamp(clip->LowerRightCorner.x, out.x0, w);
			out.y0 = SINT32_clamp(clip->UpperLeftCorner.y, 0, h);
			out.y1 = SINT32_clamp(clip->LowerRightCorner.y, out.y0, h);
		}
		else
		{
			out.x0 = 0;
			out.y0 = 0;
			out.x1 = w;
			out.y1 = h;
		}

	}


	static inline tExecuteBlit getBlitter2(eBlitter operation, const IImage * dest, const IImage * source)
	{
		ECOLOR_FORMAT sourceFormat = (ECOLOR_FORMAT) (source ? source->getColorFormat() : -1);
		ECOLOR_FORMAT destFormat = (ECOLOR_FORMAT) (dest ? dest->getColorFormat() : -1);

		const blitterTable * b = blitTable;

		while (b->operation != BLITTER_INVALID)
		{
			if (b->operation == operation)
			{
				if ((b->destFormat == -1 || b->destFormat == destFormat) &&
					(b->sourceFormat == -1 || b->sourceFormat == sourceFormat))
					return b->func;
				else
					if (b->destFormat == -2 && (sourceFormat == destFormat))
						return b->func;
			}
			b += 1;
		}
		return 0;
	}

	

	/*!
	a generic 2D Blitter
	*/
	static SINT32 Blit(eBlitter operation,
		IImage * dest,
		const rect<SINT32> *destClipping,
		const Vector2 *destPos,
		IImage * const source,
		const rect<SINT32> *sourceClipping,
		UINT32 argb)
	{
		tExecuteBlit blitter = getBlitter2(operation, dest, source);
		if (0 == blitter)
		{
			return 0;
		}

		// Clipping
		AbsRectangle sourceClip;
		AbsRectangle destClip;
		AbsRectangle v;

		SBlitJob job;

		setClip(sourceClip, sourceClipping, source, 1);
		setClip(destClip, destClipping, dest, 0);

		v.x0 = destPos ? destPos->x : 0;
		v.y0 = destPos ? destPos->y : 0;
		v.x1 = v.x0 + (sourceClip.x1 - sourceClip.x0);
		v.y1 = v.y0 + (sourceClip.y1 - sourceClip.y0);

		if (!intersect(job.Dest, destClip, v))
			return 0;

		job.width = job.Dest.x1 - job.Dest.x0;
		job.height = job.Dest.y1 - job.Dest.y0;

		job.Source.x0 = sourceClip.x0 + (job.Dest.x0 - v.x0);
		job.Source.x1 = job.Source.x0 + job.width;
		job.Source.y0 = sourceClip.y0 + (job.Dest.y0 - v.y0);
		job.Source.y1 = job.Source.y0 + job.height;

		job.argb = argb;

		if (source)
		{
			job.srcPitch = source->getPitch();
			job.srcPixelMul = source->getBytesPerPixel();
			job.src = (void*)((UINT8*)source->lock() + (job.Source.y0 * job.srcPitch) + (job.Source.x0 * job.srcPixelMul));
		}
		else
		{
			// use srcPitch for color operation on dest
			job.srcPitch = job.width * dest->getBytesPerPixel();
		}

		job.dstPitch = dest->getPitch();
		job.dstPixelMul = dest->getBytesPerPixel();
		job.dst = (void*)((UINT8*)dest->lock() + (job.Dest.y0 * job.dstPitch) + (job.Dest.x0 * job.dstPixelMul));

		blitter(&job);

		if (source)
			source->unlock();

		if (dest)
			dest->unlock();

		return 1;
	}
}
#endif