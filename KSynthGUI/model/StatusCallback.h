#ifndef STATUSCALLBACK_H
#define STATUSCALLBACK_H

#include <algorithm>

typedef std::function< void (const std::string& txt, float val) > StatusCallback;

#endif // STATUSCALLBACK_H
