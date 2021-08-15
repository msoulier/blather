#include "logger.h"

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
