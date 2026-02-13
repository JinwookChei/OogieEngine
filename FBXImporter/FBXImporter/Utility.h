#pragma once
#include <Common/MathType/MathType.h>
#include <fbxsdk.h>

inline Float4x4 ConvertFbxAMatrixToFloat4x4_SIMD(const fbxsdk::FbxAMatrix& mat)
{
    Float4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        __m128d v0 = _mm_loadu_pd(&mat[row][0]);
        __m128d v1 = _mm_loadu_pd(&mat[row][2]);

        __m128 f0 = _mm_cvtpd_ps(v0);
        __m128 f1 = _mm_cvtpd_ps(v1);

        __m128 rowVec = _mm_movelh_ps(f0, f1);

        // unaligned store
        _mm_storeu_ps(&result.r[row].X, rowVec);
    }

    return result;
}