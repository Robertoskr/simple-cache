#include "cache.hpp"
#include "unistd.h"
#include "time.h"

int main(){
    Cache<string> cache(5);
    cache.add("roberto", "guapeton");
    string val = cache.get("roberto");
    cout << val << endl;
    sleep(10);
    cache.add("roiberto", "ga;lkdj");
    cout << cache.get("roiberto");
    sleep(10);
    cout << cache.find("roberto") << endl << cache.find("roiberto") << endl;
}