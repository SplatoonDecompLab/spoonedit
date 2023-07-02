
#include "virintox/gcore/Graphics.h"
#include "Demo.h"
int main(){
    Graphics::Init();
    Graphics::setWindow<Demo>();
    Graphics::BeginLoop();
    Graphics::Terminate();
}