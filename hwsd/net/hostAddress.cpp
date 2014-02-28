#include "hostAddress.h"

namespace hwsd
{
namespace net
{

HostAddress::HostAddress()
    : _type( INVALID )
{}

HostAddress::HostAddress( const std::string& host )
    : _type( INVALID )
{
    _init( host );
}

HostAddress& HostAddress::operator = ( const std::string& host )
{
    _type = INVALID;
    _init( host );
    return *this;
}

HostAddress::operator bool() const
{
    return _type != INVALID;
}

void HostAddress::_init( const std::string& host )
{
    if( inet_pton(AF_INET, host.c_str(), &( _ip4 )))
    {
        _type = IP4;
        return;
    }
    if( inet_pton(AF_INET6, host.c_str(), &( _ip6 )))
    {
        _type = IP6;
        return;
    }
}

}
}
