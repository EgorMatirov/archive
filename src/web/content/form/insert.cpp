#include "bacs/archive/web/content/form/insert.hpp"

namespace bacs{namespace archive{namespace web{namespace content{namespace form
{
    insert::insert()
    {
        archive.message(cppcms::locale::translate("Archive"));
        submit.message(cppcms::locale::translate("Insert"));
        add(config);
        add(archive);
        add(submit);
    }
}}}}}