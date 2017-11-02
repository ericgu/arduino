class Color
{
public:
  unsigned short red;
  unsigned short green;
  unsigned short blue;

  Color(){}

  Color(unsigned short redValue, unsigned short greenValue, unsigned short blueValue)
  {
    red = redValue;
    green = greenValue;
    blue = blueValue;
  }

  Color ShiftUp()
  {
    return Color(
                        red << 8,
                        green << 8,
                        blue << 8);
  }
  
  Color ShiftDown()
  {
    return Color(
                        red >> 8,
                        green >> 8,
                        blue >> 8);
  }
  
  Color Dim(byte amount)
  {
     return Color(red >> amount, green >> amount, blue >> amount);
  }
};

