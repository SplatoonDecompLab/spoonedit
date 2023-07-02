#include"virintox/gcore/Window.h"

class Demo : public Graphics::Window{
public:
    Demo();
    void Update() override;
    inline static Demo* THIS;    
};