#include <Filters/JwtFilter.hpp>

void JwtFilter::doFilter(
    const HttpRequestPtr& req,
    FilterCallback&& fcb,
    FilterChainCallback&& fccb
)
{
    auto authHeader = req->getHeader("Authorization");
    
    if(authHeader.empty())
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        return fcb(response);
    }

    try
    {
        // Pure token with "Bearer " removed
        auto token = authHeader.substr(7);
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256("secret"))
            .with_issuer("auth0");

        verifier.verify(decoded);

        fccb();
    }
    catch(...)
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);
        
        fcb(response);
    }
}
