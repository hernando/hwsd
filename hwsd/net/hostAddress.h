#include <arpa/inet.h>
#include <string>

namespace hwsd
{
namespace net
{

class HostAddress
{
public:
    friend class SubnetAddress;

    HostAddress();

    HostAddress( const std::string& host );

    HostAddress& operator = ( const std::string& host );

    operator bool() const;

private:
    enum Type { INVALID = 0, IP4, IP6 };

    Type _type;

    union
    {
        struct in_addr _ip4;
        struct in6_addr _ip6;
    };

    void _init( const std::string& host );
};

}
}
