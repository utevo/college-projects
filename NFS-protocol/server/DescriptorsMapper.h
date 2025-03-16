#ifndef NFS_DESCRIPTORSMAPPER_H
#define NFS_DESCRIPTORSMAPPER_H

#include <vector>
#include <map>

template <typename T>
class DescriptorsMapper {
public:
    T operator[](int fd) {
        if(mapper.find(fd) != mapper.end())
            return mapper[fd];
        else
            throw std::out_of_range("Descriptor does not exists");
    }
    int addDescriptor(T fd) {
        if(fd <= 0)
            throw std::invalid_argument("Descriptor does not exists");
        int lowestFreeFd = getLowestFreeDescriptor();
        mapper[lowestFreeFd] = fd;
        return lowestFreeFd;
    }
    void removeDescriptor(int fd) {
        if(mapper.find(fd) != mapper.end())
            mapper.erase(fd);
        else
            throw std::out_of_range("Descriptor does not exists");
    }

private:
    // first -> descriptor on client, second -> descriptor on server
    std::map<int, T> mapper;
    int getLowestFreeDescriptor() {
        int i = 1;
        while(mapper.find(i) != mapper.end())
            ++i;
        return i;
    }
};
#endif //NFS_DESCRIPTORSMAPPER_H
