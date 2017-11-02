struct Extra
{
  byte main;
  byte secondary;
  byte dimShift;
};

const int BOTTOM_FIRST = 0;
const int BOTTOM_LAST = 9;

const int MIDDLE_FIRST = 10;
const int MIDDLE_LAST = 19;

const int TOP_FIRST = 34;
const int TOP_LAST = 43;

const int SMILE_LEFT = 23;
const int SMILE_RIGHT = 30;

const int NOSE = 31;
const int LEFT_EYE = 32;
const int RIGHT_EYE = 33;

const int BUTTON_BOTTOM = 20;
const int BUTTON_MIDDLE = 21;
const int BUTTON_TOP = 22;

const int SMILE_FADE_INTENSITY = 20;
const int NOSE_FADE_INTENSITY = 25;
const int EYE_FADE_INTENSITY = 40;

const int MAIN_SIZE = 31;
const int EXTRA_SIZE = 14;

const byte main[MAIN_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                  34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44};

const Extra extra[EXTRA_SIZE] = 
                { {14, BUTTON_BOTTOM, 0},
                  {16, BUTTON_MIDDLE, 0},
                  {18, BUTTON_TOP, 0},
                  {34, SMILE_LEFT + 0, 0},
                  {35, SMILE_LEFT + 1, 0},
                  {36, SMILE_LEFT + 2, 0},
                  {37, SMILE_LEFT + 3, 0},
                  {38, SMILE_LEFT + 4, 0},
                  {39, SMILE_LEFT + 5, 0},
                  {40, SMILE_LEFT + 6, 0},
                  {41, SMILE_LEFT + 7, 0},
                  {42, LEFT_EYE, 0},
                  {43, NOSE, 0},
                  {44, RIGHT_EYE, 0} };

Color targets[] = {           Color(255, 0, 0), 
                                Color(0, 255, 0),
                                Color(0, 0, 255),
                                Color(255, 255, 0),
                                Color(255, 0, 255),
                                Color(0, 255, 255),

                                Color(255, 127,   0), 
                                Color(255,   0, 127), 
                                Color(127, 255,   0),
                                Color(  0, 255, 127),
                                Color(127,   0, 255),
                                Color(  0, 127, 255)
                            };


