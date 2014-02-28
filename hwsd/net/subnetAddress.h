#include "hostAddress.h"

namespace hwsd
{
namespace net
{

class SubnetAddress
{
public:
    SubnetAddress( const std::string& host );

    operator bool() const;

    bool isInSubnet( const HostAddress& address ) const;

private:
    HostAddress _prefix;
    unsigned int _bits;
};

}
}
