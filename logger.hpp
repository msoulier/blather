#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace logging = boost::log;

class Blogger
{
public:
    Blogger();
    ~Blogger();
    void info(std::string msg);

private:
    void init();
};
