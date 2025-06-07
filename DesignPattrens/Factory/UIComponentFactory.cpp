#include<iostream>
#include <memory>
#include <string>
using namespace std;

class UIComponent
{
    public:
    virtual void select() = 0;
    virtual ~UIComponent(){}
};

class Button : public UIComponent
{
    public:
    void select()
    {
        cout << "Button is Selected" << endl;
    }
};

class TextField : public UIComponent
{
    public:
    void select()
    {
        cout << "TextField is Selected" << endl;
    }
};

class UIComponentFactory
{
    public:
    static unique_ptr <UIComponent> createComponent(const string& component)
    {
        if(component.compare("Button") == 0)
        return make_unique <Button> ();
        else if(component.compare("TextField") == 0)
        return make_unique <TextField> ();
        else
        throw invalid_argument("Component is Invalid");
    }
};

int main()
{
    auto comp = UIComponentFactory::createComponent("Button");
    comp ->select();
    return 0;
}