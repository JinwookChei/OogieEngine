#pragma once
class WereWolf : public Player
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

};
