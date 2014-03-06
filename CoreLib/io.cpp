#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "io.hpp"

using namespace std;
using namespace boost;
using namespace CoreLib;

namespace BFS = boost::filesystem;

bool IO::DirExists(const string &path)
{
    try {
        return BFS::exists(path);
    }
    catch(const BFS::filesystem_error &ex) {
        (void)ex;
    }
    catch(...) {
    }

    return false;
}

bool IO::FileExists(const string &path)
{
    ifstream file(path.c_str());
    if (file.good())
        return true;
    else
        return false;
}

size_t IO::FileSize(const string &path)
{
    if (FileExists(path.c_str())) {
        ifstream::pos_type begin;
        ifstream::pos_type end;
        ifstream file(path.c_str());
        begin = file.tellg();
        file.seekg(0, ios::end);
        end = file.tellg();
        file.close();
        return static_cast<size_t>(end - begin);
    } else {
        return 0;
    }
}

bool IO::EraseDir(const string &path)
{
    try {
        BFS::remove_all(path);

        return true;
    }
    catch(const BFS::filesystem_error &ex) {
        (void)ex;
    }
    catch(...) {
    }

    return false;
}

bool IO::EraseFile(const string &path)
{
    try {
        BFS::remove(path);

        return true;
    }
    catch(const BFS::filesystem_error &ex) {
        (void)ex;
    }
    catch(...) {
    }

    return false;
}

