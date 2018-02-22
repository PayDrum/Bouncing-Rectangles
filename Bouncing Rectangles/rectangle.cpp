#include "rectangle.hpp"

//We put the screen dimensions in class member constants, so we save cpu cycles.
Rect::Rect():screenWidth(Settings::getInstance().screenWidth),screenHeight(Settings::getInstance().screenHeight)
{
	generateRandomArguments();

	createGeometry();
	
	updateModelMat();
}

//This function fills up all the random arguments of the rectangle
void Rect::generateRandomArguments()
{
	std::random_device rd;				//Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd());				//Standard mersenne_twister_engine seeded with rd()

	std::uniform_int_distribution<> widthDistrib(minwidth, maxWidth);
	width = widthDistrib(gen);			//Rectangle Width in pixels

	std::uniform_real_distribution<> xDistrib(0.5f*(-screenWidth + width), 0.5f*(screenWidth - width));
	currentX = xDistrib(gen);			//Rectangle Initial X in pixels

	std::uniform_int_distribution<> heightDistrib(minHeight, maxHeight);
	height = heightDistrib(gen);		//Rectangle Height in pixels

	std::uniform_real_distribution<> yDistrib(0.5f*(-screenHeight + height), 0.5f*(screenHeight - height));
	currentY = yDistrib(gen);			//Rectangle Initial Y in pixels

	std::uniform_int_distribution<> speedDistrib(minSpeed, maxSpeed);
	speed = speedDistrib(gen);			//Speed value in pixels per second
	std::uniform_real_distribution<> directionDistrib(0.0f, DirectX::XM_2PI);
	direction = directionDistrib(gen);	//Direction based on Radian angle
	vx = speed * DirectX::XMScalarCos(direction);
	vy = speed * DirectX::XMScalarSin(direction);
	
	std::uniform_real_distribution<> colorDistrib(0.45f, 1.0f);
	red = colorDistrib(gen);			//red value
	green = colorDistrib(gen);			//green value
	blue = colorDistrib(gen);			//blue value
}

void Rect::createGeometry()
{
	//change to normal coordinates
	float widthF = width / screenWidth;
	float heightF = height / screenHeight;

	vertices[0] = { -widthF, heightF, 0.0f, red, green, blue, 1.0f };
	vertices[1] = { widthF, -heightF, 0.0f, red, green, blue, 1.0f };
	vertices[2] = { -widthF, -heightF, 0.0f, red, green, blue, 1.0f};
	vertices[3] = { widthF, heightF, 0.0f,red, green, blue, 1.0f };
}

//Update position based on timer and check for collision
void Rect::updateCurrentPosition(FLOAT delta)
{
	currentX += vx * delta;
	if (abs(currentX)+(width/2.0f) > screenWidth* 0.5f)
	{
		currentX = (currentX / abs(currentX))* (screenWidth - width) - currentX;
		vx *= -1;
	}

	currentY += vy * delta;
	if (abs(currentY)+(height/2.0f) > screenHeight * 0.5f)
	{
		currentY = (currentY / abs(currentY)) * (screenHeight - height) - currentY;
		vy *= -1;
	}

	updateModelMat();
}

void Rect::updateModelMat()
{
	modelMat = DirectX::XMMatrixTranslation((2.0f * currentX) / screenWidth, (2.0f *currentY )/ screenHeight, 0.0f);
}

Rect::~Rect()
{
	//cleanup
	cbo->Release();
	vbo->Release();
	ibo->Release();
}