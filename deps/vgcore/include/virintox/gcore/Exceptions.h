#pragma once
#include<exception>
#include<string>

namespace Graphics::Exceptions{
    class SegmentationFault : std::exception{
    public:
        const char* what() const noexcept override;
    private:
        const char* where;
    };
}