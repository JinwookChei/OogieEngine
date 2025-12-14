#pragma once

class Label
{
public:
	Label();
	virtual ~Label();

	void SetName(const std::string& name) { name_ = name; }
	std::string& GetName() { return name_; }

private:
	std::string name_;
};