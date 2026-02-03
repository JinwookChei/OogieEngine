#pragma once
class Wukong : public Player
{
public:
	Wukong();
	~Wukong() override;

	void Tick(double deltaTime) override;
	void BeginPlay() override;
	void Render() override;

private:
	void CleanUp() override;

private:

};
