#ifndef BASE_H
#define BASE_H
#include <iostream>

class base
{
public:
    int layer;

    base();

    virtual void update()
    {
        std::cout << "I'm base\n";
    }

    virtual void draw()
    {

    }
    virtual ~base();
protected:
private:
};

#endif // BASE_H
