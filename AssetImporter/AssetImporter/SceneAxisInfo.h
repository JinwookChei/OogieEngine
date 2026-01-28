#pragma once

class SceneAxisInfo
{
public:
    friend class AssetImporter;
    SceneAxisInfo();
    ~SceneAxisInfo();

    void PrintSceneAxisInfo();

private:
    FbxAxisSystem::EUpVector UpAxis;
    int UpSign;        // +1 / -1
    FbxAxisSystem::EFrontVector FrontAxis;
    int FrontSign;    // +1 / -1
    FbxAxisSystem::ECoordSystem CoordSystem;
    double UnitScale; // cm 기준 스케일 (1.0 = cm)
};