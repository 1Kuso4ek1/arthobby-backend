#pragma once
#include <drogon/HttpFilter.h>

#include <jwt-cpp/jwt.h>

using namespace drogon;

namespace Filters
{

class JwtFilter final : public HttpFilter<JwtFilter>
{
public:
    void doFilter(
        const HttpRequestPtr& req,
        FilterCallback&& fcb,
        FilterChainCallback&& fccb
    ) override;

};

}
