#include "subnetAddress.h"

#include <sstream>

namespace hwsd
{
namespace net
{

namespace
{

bool _isInSubnet( const in_addr& host, const in_addr& prefix,
                  const unsigned int bits )
{
    const unsigned int mask = -1 << ( 32 - bits );
    // All bits covered by the mask must be equal.
    return !(( ntohl( host.s_addr ) ^ ntohl( prefix.s_addr )) & mask );
}

bool _isInSubnet( const in6_addr& host, const in6_addr& prefix,
                  const unsigned int bits )
{
    // Network byte order is Big-endian.
    // mask[0] is the most significant 4 bytes.
    uint32_t mask[4];
    for (unsigned int i = 0; i != 4; ++i)
        mask[i] = htonl(
            -1 << ( 32 - ( bits > 32 * i ?
                           std::min( 32u, bits - 32 * i) : 0 )));
    bool subnet = true;
    for (unsigned int i = 0; i != 4 && subnet; ++i)
    {
        uint32_t hostw = ((uint32_t*)host.s6_addr)[i];
        uint32_t prefixw = ((uint32_t*)prefix.s6_addr)[i];
        subnet = !((hostw ^ prefixw) & mask[i]);
    }
    return subnet;
}

}

SubnetAddress::SubnetAddress( const std::string& host )
{
    std::stringstream mask( host );
    std::string prefix;
    std::getline( mask, prefix, '/' );
    _prefix = prefix;
    if (!_prefix)
        return;
    mask >> _bits;
    // Check the network mask and apply it
    if( mask.fail() || !mask.eof() ||
        ( _prefix._type == HostAddress::IP4 && _bits > 32 ) ||
        ( _prefix._type == HostAddress::IP6 && _bits > 128 ) )
    {
        _prefix._type = HostAddress::INVALID;
    }
}

SubnetAddress::operator bool() const
{
    return (bool)_prefix;
}

bool SubnetAddress::isInSubnet( const HostAddress& address ) const
{
    if( _prefix._type == HostAddress::INVALID ||
        address._type != _prefix._type )
        return false;

    if( address._type == HostAddress::IP4 )
        return _isInSubnet( address._ip4, _prefix._ip4, _bits );
    else // address._type == HostAddress::IP6
        return _isInSubnet( address._ip6, _prefix._ip6, _bits );
}

}
}
