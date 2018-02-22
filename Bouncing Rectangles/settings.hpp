#pragma once

//Singleton settings class
class Settings 
{
public:
	static Settings& getInstance()
	{
		static Settings instance;
		return instance;
	}

	const int screenWidth = 800;
	const int screenHeight = 600;

private:
	Settings() {}     
	
public:
	Settings(Settings const&) = delete;
	void operator=(Settings const&) = delete;

};