The are a number of constants through the project which were part of the requirements:
	-Resolution is hard set to 800 by 600
	-Resizing of the Window has been disabled
	-Number of rectangles are random and between 3 and 9 inclusive
	-Heights of rectangles are random and between 24 and 64 pixels inclusive
	-Widths of the rectangles are random and between 16 and 48 pixels inclusive
	-Colors of the rectangles are 25% lighter than the background and random
	-Background color is gray
	-Moving directions are randomly chosen at the start
	-Starting positions are randomly chosen at the start
	-The speed value for each rectangle is constant and chosen randomly at the start between 10 and 100 pixels per second inclusive.

All these are adjusted through constants so they are easily adjustable.

TODO: add a render function to the rectangle class and call that from the scene class instead 