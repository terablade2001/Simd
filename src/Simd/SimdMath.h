/*
* Simd Library (http://simd.sourceforge.net).
*
* Copyright (c) 2011-2016 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef __SimdMath_h__
#define __SimdMath_h__

#include "Simd/SimdDefs.h"
#include "Simd/SimdConst.h"

namespace Simd
{
	template <class T> SIMD_INLINE void Swap(T & a, T & b)
	{
		T t = a;
		a = b;
		b = t;
	}

	template <class T> SIMD_INLINE T Min(T a, T b)
	{
		return a < b ? a : b;
	}

	template <class T> SIMD_INLINE T Max(T a, T b)
	{
		return a > b ? a : b;
	}

	template <class T> SIMD_INLINE T Abs(T a)
	{
		return a < 0 ? -a : a;
	}

	template <class T> SIMD_INLINE T RestrictRange(T value, T min, T max)
	{
		return Max(min, Min(max, value));
	}

	template <class T> SIMD_INLINE T Square(T a)
	{
		return a*a;
	}

#ifndef SIMD_ROUND
#define SIMD_ROUND
    SIMD_INLINE int Round(double value)
    {
#if defined(SIMD_SSE2_ENABLE) && ((defined(_MSC_VER) && defined(_M_X64)) || (defined(__GNUC__) && defined(__x86_64__)))
        __m128d t = _mm_set_sd(value);
        return _mm_cvtsd_si32(t);
#else
        return (int)(value + (value >= 0 ? 0.5 : -0.5));
#endif
    }
#endif

    namespace Base
    {
        SIMD_INLINE int Min(int a, int b)
        {
            return a < b ? a : b;
        }

        SIMD_INLINE int Max(int a, int b)
        {
            return a > b ? a : b;
        }

		SIMD_INLINE int RestrictRange(int value, int min = 0, int max = 255)
		{
			return Max(min, Min(max, value));
		}

        SIMD_INLINE int Square(int a)
        {
            return a*a;
        }

        SIMD_INLINE int SquaredDifference(int a, int b)
        {
            return Square(a - b);
        }

		SIMD_INLINE int AbsDifference(int a, int b)
		{
			return a > b ? a - b : b - a;
		}

		SIMD_INLINE int Average(int a, int b)
		{
			return (a + b + 1) >> 1;
		}

		SIMD_INLINE int Average(int a, int b, int c, int d)
		{
			return (a + b + c + d + 2) >> 2;
		}

		SIMD_INLINE void SortU8(int & a, int & b)
		{
			int d = a - b;
			int m = ~(d >> 8);
			b += d & m;
			a -= d & m;
		}

		SIMD_INLINE int AbsDifferenceU8(int a, int b)
		{
			int d = a - b;
			int m = d >> 8;
			return (d & ~m)|(-d & m);
		}

		SIMD_INLINE int MaxU8(int a, int b)
		{
			int d = a - b;
			int m = ~(d >> 8);
			return b + (d & m);
		}

		SIMD_INLINE int MinU8(int a, int b)
		{
			int d = a - b;
			int m = ~(d >> 8);
			return a - (d & m);
		}

        SIMD_INLINE int SaturatedSubtractionU8(int a, int b)
        {
            int d = a - b;
            int m = ~(d >> 8);
            return (d & m);
        }

        SIMD_INLINE int DivideBy255(int value)
        {
			return (value + 1 + (value >> 8)) >> 8;
        }

        template <bool compensation> SIMD_INLINE int DivideBy16(int value);

        template <> SIMD_INLINE int DivideBy16<true>(int value)
        {
            return (value + 8) >> 4;
        }

        template <> SIMD_INLINE int DivideBy16<false>(int value)
        {
            return value >> 4;
        }

        template <bool compensation> SIMD_INLINE int GaussianBlur3x3(const uint8_t *s0, const uint8_t *s1, const uint8_t *s2, size_t x0, size_t x1, size_t x2)
        {
            return DivideBy16<compensation>(s0[x0] + 2*s0[x1] + s0[x2] + (s1[x0] + 2*s1[x1] + s1[x2])*2 + s2[x0] + 2*s2[x1] + s2[x2]);
        }

        SIMD_INLINE void Reorder16bit(const uint8_t * src, uint8_t * dst)
        {
            uint16_t value = *(uint16_t*)src;
            *(uint16_t*)dst = value >> 8 | value << 8;
        }

        SIMD_INLINE void Reorder32bit(const uint8_t * src, uint8_t * dst)
        {
            uint32_t value = *(uint32_t*)src;
            *(uint32_t*)dst = 
                (value & 0x000000FF) << 24 | (value & 0x0000FF00) << 8 | 
                (value & 0x00FF0000) >> 8 | (value & 0xFF000000) >> 24;
        }

        SIMD_INLINE void Reorder64bit(const uint8_t * src, uint8_t * dst)
        {
            uint64_t value = *(uint64_t*)src;
            *(uint64_t*)dst = 
                (value & 0x00000000000000FF) << 56 | (value & 0x000000000000FF00) << 40 | 
                (value & 0x0000000000FF0000) << 24 | (value & 0x00000000FF000000) << 8 | 
                (value & 0x000000FF00000000) >> 8  | (value & 0x0000FF0000000000) >> 24 | 
                (value & 0x00FF000000000000) >> 40 | (value & 0xFF00000000000000) >> 56;
        }

		SIMD_INLINE float Sigmoid(float value)
		{
			return 1.0f / (1.0f + ::exp(-value));
		}

		SIMD_INLINE float RoughSigmoid(float value)
		{
			float x = ::fabs(value);
			float x2 = x*x;
			float e = 1.0f + x + x2*0.555f + x2*x2*0.143f;
			return 1.0f / (1.0f + (value > 0 ? 1.0f / e : e));
		}

        SIMD_INLINE float DerivativeSigmoid(float function)
        {
            return (1.0f - function)*function;
        }

        SIMD_INLINE float Tanh(float value)
        {
            return ::tanh(value);
        }

        SIMD_INLINE float RoughTanh(float value)
        {
            float x = ::fabs(value);
            float x2 = x*x;
            float pe = 1.0f + x + x2*0.559f + x2*x2*0.148f;
            float ne = 1.0f/pe;
            return (value > 0 ? 1.0f : -1.0f)*(pe - ne)/(pe + ne);
        }

        SIMD_INLINE float DerivativeTanh(float function)
        {
            return (1.0f - function*function);
        }

        SIMD_INLINE void UpdateWeights(const float * x, size_t offset, float a, float b, float * d, float * w)
        {
            float _d = a*d[offset] + b*x[offset];
            d[offset] = _d;
            w[offset] += _d;
        }
	}

#ifdef SIMD_SSE_ENABLE
    namespace Sse
    {
        SIMD_INLINE __m128 Square(__m128 value)
        {
            return _mm_mul_ps(value, value);
        }

        SIMD_INLINE __m128 Combine(__m128 mask, __m128 positive, __m128 negative)
        {
            return _mm_or_ps(_mm_and_ps(mask, positive), _mm_andnot_ps(mask, negative));
        }

        SIMD_INLINE __m128 RightNotZero(size_t count)
        {
            union { uint32_t i[4]; float f[4]; } b;
            for (size_t i = 0; i < 4; ++i)
                b.i[i] = ((count >= 4 - i) ? -1 : 0);
            return _mm_loadu_ps(b.f);
        }
    }
#endif//SIMD_SSE_ENABLE

#ifdef SIMD_SSE2_ENABLE
	namespace Sse2
	{
		SIMD_INLINE __m128i SaturateI16ToU8(__m128i value)
		{
			return _mm_min_epi16(K16_00FF, _mm_max_epi16(value, K_ZERO));
		}

		SIMD_INLINE __m128i MaxI16(__m128i a, __m128i b, __m128i c)
		{
			return _mm_max_epi16(a, _mm_max_epi16(b, c));
		}

		SIMD_INLINE __m128i MinI16(__m128i a, __m128i b, __m128i c)
		{
			return _mm_min_epi16(a, _mm_min_epi16(b, c));
		}

		SIMD_INLINE void SortU8(__m128i & a, __m128i & b)
		{
			__m128i t = a;
			a = _mm_min_epu8(t, b);
			b = _mm_max_epu8(t, b);
		}

		SIMD_INLINE __m128i ShiftLeft(__m128i a, size_t shift)
		{
			__m128i t = a;
			if(shift & 8)
				t = _mm_slli_si128(t, 8);
			if(shift & 4)
				t = _mm_slli_si128(t, 4);
			if(shift & 2)
				t = _mm_slli_si128(t, 2);
			if(shift & 1)
				t = _mm_slli_si128(t, 1);
			return t;
		}

		SIMD_INLINE __m128i ShiftRight(__m128i a, size_t shift)
		{
			__m128i t = a;
			if(shift & 8)
				t = _mm_srli_si128(t, 8);
			if(shift & 4)
				t = _mm_srli_si128(t, 4);
			if(shift & 2)
				t = _mm_srli_si128(t, 2);
			if(shift & 1)
				t = _mm_srli_si128(t, 1);
			return t;
		}

		SIMD_INLINE __m128i HorizontalSum32(__m128i a)
		{
            return _mm_add_epi64(_mm_unpacklo_epi32(a, K_ZERO), _mm_unpackhi_epi32(a, K_ZERO));
		}

		SIMD_INLINE __m128i AbsDifferenceU8(__m128i a, __m128i b)
		{
			return _mm_sub_epi8(_mm_max_epu8(a, b), _mm_min_epu8(a, b));
		}

        SIMD_INLINE __m128i MulU8(__m128i a, __m128i b)
        {
            __m128i lo = _mm_mullo_epi16(_mm_unpacklo_epi8(a, K_ZERO), _mm_unpacklo_epi8(b, K_ZERO));
            __m128i hi = _mm_mullo_epi16(_mm_unpackhi_epi8(a, K_ZERO), _mm_unpackhi_epi8(b, K_ZERO));
            return _mm_packus_epi16(lo, hi);
        }

        SIMD_INLINE __m128i DivideI16By255(__m128i value)
        {
            return _mm_srli_epi16(_mm_add_epi16(_mm_add_epi16(value, K16_0001), _mm_srli_epi16(value, 8)), 8);
        }

        SIMD_INLINE __m128i BinomialSum16(const __m128i & a, const __m128i & b, const __m128i & c)
        {
            return _mm_add_epi16(_mm_add_epi16(a, c), _mm_add_epi16(b, b));
        }

        SIMD_INLINE __m128i BinomialSum16(const __m128i & a, const __m128i & b, const __m128i & c, const __m128i & d)
        {
            return _mm_add_epi16(_mm_add_epi16(a, d), _mm_mullo_epi16(_mm_add_epi16(b, c), K16_0003));
        }

        SIMD_INLINE __m128i Combine(__m128i mask, __m128i positive, __m128i negative)
        {
            return _mm_or_si128(_mm_and_si128(mask, positive), _mm_andnot_si128(mask, negative));
        }

        SIMD_INLINE __m128i AlphaBlendingI16(__m128i src, __m128i dst, __m128i alpha)
        {
            return DivideI16By255(_mm_add_epi16(_mm_mullo_epi16(src, alpha), _mm_mullo_epi16(dst, _mm_sub_epi16(K16_00FF, alpha))));
        }

        template <int part> SIMD_INLINE __m128i UnpackU8(__m128i a, __m128i b = K_ZERO);

        template <> SIMD_INLINE __m128i UnpackU8<0>(__m128i a, __m128i b)
        {
            return _mm_unpacklo_epi8(a, b); 
        }

        template <> SIMD_INLINE __m128i UnpackU8<1>(__m128i a, __m128i b)
        {
            return _mm_unpackhi_epi8(a, b); 
        }

		template <int part> SIMD_INLINE __m128i UnpackU16(__m128i a, __m128i b = K_ZERO);

		template <> SIMD_INLINE __m128i UnpackU16<0>(__m128i a, __m128i b)
		{
			return _mm_unpacklo_epi16(a, b);
		}

		template <> SIMD_INLINE __m128i UnpackU16<1>(__m128i a, __m128i b)
		{
			return _mm_unpackhi_epi16(a, b);
		}

        SIMD_INLINE __m128i DivideBy16(__m128i value)
        {
            return _mm_srli_epi16(_mm_add_epi16(value, K16_0008), 4);
        }
	}
#endif// SIMD_SSE2_ENABLE

#ifdef SIMD_SSSE3_ENABLE
    namespace Ssse3
    {
        using namespace Sse2;

        template <bool abs> __m128i ConditionalAbs(__m128i a);

        template <> SIMD_INLINE __m128i ConditionalAbs<true>(__m128i a)
        {
            return _mm_abs_epi16(a);
        }

        template <> SIMD_INLINE __m128i ConditionalAbs<false>(__m128i a)
        {
            return a;
        }

        template<int part> SIMD_INLINE __m128i SubUnpackedU8(__m128i a, __m128i b)
        {
            return _mm_maddubs_epi16(UnpackU8<part>(a, b), K8_01_FF);
        }
    }
#endif// SIMD_SSSE3_ENABLE

#ifdef SIMD_AVX_ENABLE
    namespace Avx
    {
        SIMD_INLINE __m256 Square(__m256 value)
        {
            return _mm256_mul_ps(value, value);
        }

        SIMD_INLINE __m256 Combine(__m256 mask, __m256 positive, __m256 negative)
        {
            return _mm256_or_ps(_mm256_and_ps(mask, positive), _mm256_andnot_ps(mask, negative));
        }

        SIMD_INLINE __m256 RightNotZero(size_t count)
        {
            union { uint32_t i[8]; float f[8]; } b;
            for (size_t i = 0; i < 8; ++i)
                b.i[i] = ((count >= 8 - i) ? -1 : 0);
            return _mm256_loadu_ps(b.f);
        }
    }
#endif//SIMD_AVX_ENABLE

#ifdef SIMD_AVX2_ENABLE
    namespace Avx2
    {
        SIMD_INLINE __m256i SaturateI16ToU8(__m256i value)
        {
            return _mm256_min_epi16(K16_00FF, _mm256_max_epi16(value, K_ZERO));
        }

        SIMD_INLINE __m256i MaxI16(__m256i a, __m256i b, __m256i c)
        {
            return _mm256_max_epi16(a, _mm256_max_epi16(b, c));
        }

        SIMD_INLINE __m256i MinI16(__m256i a, __m256i b, __m256i c)
        {
            return _mm256_min_epi16(a, _mm256_min_epi16(b, c));
        }

        SIMD_INLINE void SortU8(__m256i & a, __m256i & b)
        {
            __m256i t = a;
            a = _mm256_min_epu8(t, b);
            b = _mm256_max_epu8(t, b);
        }

        SIMD_INLINE __m256i HorizontalSum32(__m256i a)
        {
            return _mm256_add_epi64(_mm256_unpacklo_epi32(a, K_ZERO), _mm256_unpackhi_epi32(a, K_ZERO));
        }

        SIMD_INLINE __m256i AbsDifferenceU8(__m256i a, __m256i b)
        {
            return _mm256_sub_epi8(_mm256_max_epu8(a, b), _mm256_min_epu8(a, b));
        }

        SIMD_INLINE __m256i MulU8(__m256i a, __m256i b)
        {
            __m256i lo = _mm256_mullo_epi16(_mm256_unpacklo_epi8(a, K_ZERO), _mm256_unpacklo_epi8(b, K_ZERO));
            __m256i hi = _mm256_mullo_epi16(_mm256_unpackhi_epi8(a, K_ZERO), _mm256_unpackhi_epi8(b, K_ZERO));
            return _mm256_packus_epi16(lo, hi);
        }

        SIMD_INLINE __m256i DivideI16By255(__m256i value)
        {
            return _mm256_srli_epi16(_mm256_add_epi16(_mm256_add_epi16(value, K16_0001), _mm256_srli_epi16(value, 8)), 8);
        }

        SIMD_INLINE __m256i BinomialSum16(const __m256i & a, const __m256i & b, const __m256i & c)
        {
            return _mm256_add_epi16(_mm256_add_epi16(a, c), _mm256_add_epi16(b, b));
        }

        SIMD_INLINE __m256i Combine(__m256i mask, __m256i positive, __m256i negative)
        {
            return _mm256_or_si256(_mm256_and_si256(mask, positive), _mm256_andnot_si256(mask, negative));
        }

        template <bool abs> __m256i ConditionalAbs(__m256i a);

        template <> SIMD_INLINE __m256i ConditionalAbs<true>(__m256i a)
        {
            return _mm256_abs_epi16(a);
        }

        template <> SIMD_INLINE __m256i ConditionalAbs<false>(__m256i a)
        {
            return a;
        }

        template <int part> SIMD_INLINE __m256i UnpackU8(__m256i a, __m256i b = K_ZERO);

        template <> SIMD_INLINE __m256i UnpackU8<0>(__m256i a, __m256i b)
        {
            return _mm256_unpacklo_epi8(a, b); 
        }

        template <> SIMD_INLINE __m256i UnpackU8<1>(__m256i a, __m256i b)
        {
            return _mm256_unpackhi_epi8(a, b); 
        }

        template<int part> SIMD_INLINE __m256i SubUnpackedU8(__m256i a, __m256i b)
        {
            return _mm256_maddubs_epi16(UnpackU8<part>(a, b), K8_01_FF);
        }

        template <int part> SIMD_INLINE __m256i UnpackU16(__m256i a, __m256i b = K_ZERO);

        template <> SIMD_INLINE __m256i UnpackU16<0>(__m256i a, __m256i b)
        {
            return _mm256_unpacklo_epi16(a, b);
        }

        template <> SIMD_INLINE __m256i UnpackU16<1>(__m256i a, __m256i b)
        {
            return _mm256_unpackhi_epi16(a, b);
        }
    }
#endif// SIMD_AVX2_ENABLE

#ifdef SIMD_VMX_ENABLE
    namespace Vmx
    {
        SIMD_INLINE v128_u8 ShiftLeft(v128_u8 value, size_t shift)
        {
            return vec_perm(K8_00, value, vec_lvsr(shift, (uint8_t*)0));        
        }

        SIMD_INLINE v128_u16 ShiftLeft(v128_u16 value, size_t shift)
        {
            return (v128_u16)ShiftLeft((v128_u8)value, 2*shift);      
        }

        SIMD_INLINE v128_u8 ShiftRight(v128_u8 value, size_t shift)
        {
            return vec_perm(value, K8_00, vec_lvsl(shift, (uint8_t*)0));        
        }

        SIMD_INLINE v128_u16 MulHiU16(v128_u16 a, v128_u16 b)
        {
            return (v128_u16)vec_perm(vec_mule(a, b), vec_mulo(a, b), K8_PERM_MUL_HI_U16);        
        }

        SIMD_INLINE v128_u8 AbsDifferenceU8(v128_u8 a, v128_u8 b)
        {
            return vec_sub(vec_max(a, b), vec_min(a, b));
        }

        SIMD_INLINE v128_u16 SaturateI16ToU8(v128_s16 value)
        {
            return (v128_u16)vec_min((v128_s16)K16_00FF, vec_max(value, (v128_s16)K16_0000));
        }

        SIMD_INLINE void SortU8(v128_u8 & a, v128_u8 & b)
        {
            v128_u8 t = a;
            a = vec_min(t, b);
            b = vec_max(t, b);
        }

        SIMD_INLINE v128_u16 DivideBy255(v128_u16 value)
        {
            return vec_sr(vec_add(vec_add(value, K16_0001), vec_sr(value, K16_0008)), K16_0008);
        }

        SIMD_INLINE v128_u16 BinomialSum(const v128_u16 & a, const v128_u16 & b, const v128_u16 & c)
        {
            return vec_add(vec_add(a, c), vec_add(b, b));
        }

        template<class T> SIMD_INLINE T Max(const T & a, const T & b, const T & c)
        {
            return vec_max(a, vec_max(b, c));
        }

        template<class T> SIMD_INLINE T Min(const T & a, const T & b, const T & c)
        {
            return vec_min(a, vec_min(b, c));
        }

        template <bool abs> v128_u16 ConditionalAbs(v128_u16 a);

        template <> SIMD_INLINE v128_u16 ConditionalAbs<true>(v128_u16 a)
        {
            return (v128_u16)vec_abs((v128_s16)a);
        }

        template <> SIMD_INLINE v128_u16 ConditionalAbs<false>(v128_u16 a)
        {
            return a;
        }
    }
#endif//SIMD_VMX_ENABLE

#ifdef SIMD_NEON_ENABLE
	namespace Neon
	{
		SIMD_INLINE uint8x16_t ShiftLeft(uint8x16_t value, size_t shift)
		{
			if (shift & 8)
				value = vextq_u8(K8_00, value, 8);
			if (shift & 4)
				value = vextq_u8(K8_00, value, 12);
			if (shift & 2)
				value = vextq_u8(K8_00, value, 14);
			if (shift & 1)
				value = vextq_u8(K8_00, value, 15);
			return value;
		}

		SIMD_INLINE uint8x16_t ShiftRight(uint8x16_t value, size_t shift)
		{
			if (shift & 8)
				value = vextq_u8(value, K8_00, 8);
			if (shift & 4)
				value = vextq_u8(value, K8_00, 4);
			if (shift & 2)
				value = vextq_u8(value, K8_00, 2);
			if (shift & 1)
				value = vextq_u8(value, K8_00, 1);
			return value;
		}

		SIMD_INLINE void SortU8(uint8x16_t & a, uint8x16_t & b)
		{
			uint8x16_t t = a;
			a = vminq_u8(t, b);
			b = vmaxq_u8(t, b);
		}

		SIMD_INLINE uint16x8_t DivideI16By255(uint16x8_t value)
		{
			return vshrq_n_u16(vaddq_u16(vaddq_u16(value, K16_0001), vshrq_n_u16(value, 8)), 8);
		}

		SIMD_INLINE uint16x8_t BinomialSum16(const uint16x8_t & a, const uint16x8_t & b, const uint16x8_t & c)
		{
			return vaddq_u16(vaddq_u16(a, c), vaddq_u16(b, b));
		}

        SIMD_INLINE int16x8_t BinomialSum(const int16x8_t & a, const int16x8_t & b, const int16x8_t & c)
        {
            return vaddq_s16(vaddq_s16(a, c), vaddq_s16(b, b));
        }

		SIMD_INLINE uint16x8_t BinomialSum16(const uint16x8_t & a, const uint16x8_t & b, const uint16x8_t & c, const uint16x8_t & d)
		{
			return vaddq_u16(vaddq_u16(a, d), vmulq_u16(vaddq_u16(b, c), K16_0003));
		}

		SIMD_INLINE uint16x8_t DivideBy16(uint16x8_t value)
		{
			return vshrq_n_u16(vaddq_u16(value, K16_0008), 4);
		}

		template <int part> SIMD_INLINE uint8x8_t Half(uint8x16_t a);

		template <> SIMD_INLINE uint8x8_t Half<0>(uint8x16_t a)
		{
			return vget_low_u8(a);
		}

		template <> SIMD_INLINE uint8x8_t Half<1>(uint8x16_t a)
		{
			return vget_high_u8(a);
		}

		template <int part> SIMD_INLINE uint16x4_t Half(uint16x8_t a);

		template <> SIMD_INLINE uint16x4_t Half<0>(uint16x8_t a)
		{
			return vget_low_u16(a);
		}

		template <> SIMD_INLINE uint16x4_t Half<1>(uint16x8_t a)
		{
			return vget_high_u16(a);
		}

		template <int part> SIMD_INLINE int16x4_t Half(int16x8_t a);

		template <> SIMD_INLINE int16x4_t Half<0>(int16x8_t a)
		{
			return vget_low_s16(a);
		}

		template <> SIMD_INLINE int16x4_t Half<1>(int16x8_t a)
		{
			return vget_high_s16(a);
		}

		template <int part> SIMD_INLINE uint16x8_t UnpackU8(uint8x16_t a)
		{
			return vmovl_u8(Half<part>(a));
		}

		template <int part> SIMD_INLINE uint32x4_t UnpackU16(uint16x8_t a)
		{
			return vmovl_u16(Half<part>(a));
		}

		template <int part> SIMD_INLINE int32x4_t UnpackI16(int16x8_t a)
		{
			return vmovl_s16(Half<part>(a));
		}

		SIMD_INLINE uint8x16_t PackU16(uint16x8_t lo, uint16x8_t hi)
		{
			return vcombine_u8(vmovn_u16(lo), vmovn_u16(hi));
		}

		SIMD_INLINE uint8x16_t PackSaturatedI16(int16x8_t lo, int16x8_t hi)
		{
			return vcombine_u8(vqmovun_s16(lo), vqmovun_s16(hi));
		}

		SIMD_INLINE uint8x16_t PackSaturatedU16(uint16x8_t lo, uint16x8_t hi)
		{
			return vcombine_u8(vqmovn_u16(lo), vqmovn_u16(hi));
		}

		SIMD_INLINE uint16x8_t PackU32(uint32x4_t lo, uint32x4_t hi)
		{
			return vcombine_u16(vmovn_u32(lo), vmovn_u32(hi));
		}

		SIMD_INLINE int16x8_t PackI32(int32x4_t lo, int32x4_t hi)
		{
			return vcombine_s16(vmovn_s32(lo), vmovn_s32(hi));
		}

		SIMD_INLINE uint8x8x2_t Deinterleave(uint8x16_t value)
		{
			uint8_t buffer[A];
			vst1q_u8(buffer, value);
			return vld2_u8(buffer);
		}

		template <int part> SIMD_INLINE uint8x16_t Stretch2(uint8x16_t a)
		{
			return (uint8x16_t)vmulq_u16(UnpackU8<part>(a), K16_0101);
		}

		template <bool abs> int16x8_t ConditionalAbs(int16x8_t a);

		template <> SIMD_INLINE int16x8_t ConditionalAbs<true>(int16x8_t a)
		{
			return vabdq_s16(a, (int16x8_t)K16_0000);
		}

		template <> SIMD_INLINE int16x8_t ConditionalAbs<false>(int16x8_t a)
		{
			return a;
		}

		SIMD_INLINE int16x8_t SaturateByU8(int16x8_t a)
		{
			return (int16x8_t)vmovl_u8(vqmovun_s16(a));
		}

		template <int iter> SIMD_INLINE float32x4_t Reciprocal(const float32x4_t & a);

		template <> SIMD_INLINE float32x4_t Reciprocal<-1>(const float32x4_t & a)
		{
			float _a[4];
			vst1q_f32(_a, a);
			float r[4] = {1.0f/_a[0], 1.0f/_a[1], 1.0f/_a[2], 1.0f/_a[3]};
			return vld1q_f32(r);
		};

		template<> SIMD_INLINE float32x4_t Reciprocal<0>(const float32x4_t & a)
		{
			return vrecpeq_f32(a);
		}

		template<> SIMD_INLINE float32x4_t Reciprocal<1>(const float32x4_t & a)
		{
			float32x4_t r = vrecpeq_f32(a);
			return vmulq_f32(vrecpsq_f32(a, r), r);
		}

		template<> SIMD_INLINE float32x4_t Reciprocal<2>(const float32x4_t & a)
		{
			float32x4_t r = vrecpeq_f32(a);
			r = vmulq_f32(vrecpsq_f32(a, r), r);
			return vmulq_f32(vrecpsq_f32(a, r), r);
		}

		template <int iter> SIMD_INLINE float32x4_t Div(const float32x4_t & a, const float32x4_t & b)
		{
			return vmulq_f32(a, Reciprocal<iter>(b));
		}

		template <> SIMD_INLINE float32x4_t Div<-1>(const float32x4_t & a, const float32x4_t & b)
		{
			float _a[4], _b[4];
			vst1q_f32(_a, a);
			vst1q_f32(_b, b);
			float c[4] = {_a[0]/_b[0], _a[1]/_b[1], _a[2]/_b[2], _a[3]/_b[3]};
			return vld1q_f32(c);
		};

        template <int iter> SIMD_INLINE float32x4_t Sqrt(const float32x4_t & a);

        template <> SIMD_INLINE float32x4_t Sqrt<-1>(const float32x4_t & a)
        {
            float _a[4];
            vst1q_f32(_a, a);
            float r[4] = { ::sqrtf(_a[0]), ::sqrtf(_a[1]), ::sqrtf(_a[2]), ::sqrtf(_a[3]) };
            return vld1q_f32(r);
        }

        template<> SIMD_INLINE float32x4_t Sqrt<0>(const float32x4_t & a)
        {
            float32x4_t e = vrsqrteq_f32(a);
            return vmulq_f32(e, a);
        }

        template<> SIMD_INLINE float32x4_t Sqrt<1>(const float32x4_t & a)
        {
            float32x4_t e = vrsqrteq_f32(a);
            e = vmulq_f32(vrsqrtsq_f32(vmulq_f32(e, e), a), e);
            return vmulq_f32(e, a);
        }

        template<> SIMD_INLINE float32x4_t Sqrt<2>(const float32x4_t & a)
        {
            float32x4_t e = vrsqrteq_f32(a);
            e = vmulq_f32(vrsqrtsq_f32(vmulq_f32(e, e), a), e);
            e = vmulq_f32(vrsqrtsq_f32(vmulq_f32(e, e), a), e);
            return vmulq_f32(e, a);
        }

		template <int part> SIMD_INLINE int16x8_t Sub(uint8x16_t a, uint8x16_t b)
		{
			return (int16x8_t)vsubl_u8(Half<part>(a), Half<part>(b));
		}

        template <int part> SIMD_INLINE float32x4_t ToFloat(int16x8_t a)
        {
            return vcvtq_f32_s32(UnpackI16<part>(a));
        }

        template <int part> SIMD_INLINE float32x4_t ToFloat(uint16x8_t a)
        {
            return vcvtq_f32_u32(UnpackU16<part>(a));
        }
	}
#endif//SIMD_NEON_ENABLE
}
#endif//__SimdMath_h__
