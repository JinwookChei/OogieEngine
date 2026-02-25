#pragma once

class SkeletalMeshComponent;

class TestPlayer : public Player
{
public:
	TestPlayer();
	~TestPlayer() override;

	void Tick(double deltaTime) override;
	void BeginPlay() override;
	void Render() override;

private:
	void CleanUp() override;

private:
	
	SkeletalMeshComponent* pSkeletalMeshComponent_;
};
