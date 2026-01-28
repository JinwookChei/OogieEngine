#include "stdafx.h"
#include <string>
#include <sstream>
#include <string_view>
#include "SceneAxisInfo.h"

SceneAxisInfo::SceneAxisInfo()
{
}

SceneAxisInfo::~SceneAxisInfo()
{
}

void SceneAxisInfo::PrintSceneAxisInfo()
{
    // 1. Up 축 결정
    std::string upStr = (UpSign > 0 ? "+" : "-");
    int upIdx = 0; // 0:X, 1:Y, 2:Z
    if (UpAxis == FbxAxisSystem::eXAxis) { upStr += "X"; upIdx = 0; }
    else if (UpAxis == FbxAxisSystem::eYAxis) { upStr += "Y"; upIdx = 1; }
    else { upStr += "Z"; upIdx = 2; }

    // 2. Front 축 계산 (UpAxis와 Parity 조합)
    // ParityEven(1) 이면 Up의 다음 축, ParityOdd(2) 이면 Up의 다음다음 축
    int frontIdx = 0;
    if (FrontAxis == FbxAxisSystem::eParityEven)
    {
        frontIdx = (upIdx + 1) % 3;
    }
    else
    {
        frontIdx = (upIdx + 2) % 3;
    }

    std::string frontStr = (FrontSign > 0 ? "+" : "-");
    if (frontIdx == 0) frontStr += "X";
    else if (frontIdx == 1) frontStr += "Y";
    else frontStr += "Z";

    auto AxisVector = [](int idx, int sign) {
        FbxVector4 v(0, 0, 0);
        v[idx] = (double)sign;
        return v;
        };

    FbxVector4 upVec = AxisVector(upIdx, UpSign);
    FbxVector4 frontVec = AxisVector(frontIdx, FrontSign);
    FbxVector4 rightVec;

    if (CoordSystem == FbxAxisSystem::eRightHanded) {
        rightVec = frontVec.CrossProduct(upVec);
    }
    else {
        rightVec = upVec.CrossProduct(frontVec);
    }

    int rightIdx = 0;
    int rightSign = 1;

    if (fabs(rightVec[0]) > 0.5) { rightIdx = 0; rightSign = (rightVec[0] > 0) ? 1 : -1; }
    else if (fabs(rightVec[1]) > 0.5) { rightIdx = 1; rightSign = (rightVec[1] > 0) ? 1 : -1; }
    else { rightIdx = 2; rightSign = (rightVec[2] > 0) ? 1 : -1; }

    std::string rightStr = (rightSign > 0 ? "+" : "-");
    if (rightIdx == 0) rightStr += "X";
    else if (rightIdx == 1) rightStr += "Y";
    else rightStr += "Z";

    // --- 직관적인 출력 ---
    std::stringstream ss;
    ss << "\n====================================\n";
    ss << " [FBX Scene World Orientation] \n";
    ss << "------------------------------------\n";
    ss << "  위쪽 (UP)    : " << upStr << "\n";
    ss << "  앞쪽 (FRONT) : " << frontStr << "\n";
    ss << "  오른쪽 (RIGHT) : " << rightStr << "\n";
    ss << "------------------------------------\n";
    ss << "  좌표계: " << (CoordSystem == FbxAxisSystem::eRightHanded ? "오른손(Right-Handed)" : "왼손(Left-Handed)") << "\n";
    ss << "  단위: " << UnitScale << " cm\n";
    ss << "====================================\n";

    OutputDebugStringA(ss.str().c_str());
}
