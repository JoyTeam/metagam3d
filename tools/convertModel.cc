#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <iostream>

int main(int argc, const char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input> <output>" << std::endl;
        return 1;
    }
    osg::Node *node = osgDB::readNodeFile(argv[1]);
    if (!node) {
        std::cerr << "Error loading " << argv[1] << std::endl;
        return 1;
    }
    if (!osgDB::writeNodeFile(*node, argv[2])) {
        std::cerr << "Error saving " << argv[2] << std::endl;
        return 1;
    }
    return 0;
}
