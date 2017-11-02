class ColorDecider
{
public:
    virtual Color GetNextColor() = 0;

    virtual bool GetInFade() = 0;
    
    virtual void Push() = 0;
};
