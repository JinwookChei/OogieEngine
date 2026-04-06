#pragma once

class CameraComponent;
class WereWolf 
	: public Player
{
public:
	WereWolf();
	~WereWolf() override;

	void Tick(double deltaTime) override;
	void BeginPlay() override;
	void Render() override;

private:
	void CleanUp() override;

private:
	MovementComponent* pMovementComponent_;
	SkeletalMeshComponent* pSkeletalMeshComponent_;
	CameraComponent* pCameraComponent_;	
};
