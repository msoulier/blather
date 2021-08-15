#include "logger.hpp"

Blogger::Blogger()
{
    this->init();
}

Blogger::~Blogger() {}

void Blogger::init()
{
    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}

void Blogger::info(std::string msg)
{
    BOOST_LOG_TRIVIAL(info) << msg;
}
