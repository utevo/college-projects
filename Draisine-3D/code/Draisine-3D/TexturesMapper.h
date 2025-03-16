#pragma once

#include <vector>
#include <utility>

#include "Utilities.h"
#include "Texture.h"
#include "Shader.h"


class TexturesMapper {
public:
    TexturesMapper(std::vector<std::pair<std::string, std::shared_ptr<Texture> > > mapping, Shader & shader);
    ~TexturesMapper();

    void bind();
    void unbind() const;

    std::vector<std::pair<std::string, std::shared_ptr<Texture> > >& getMapping()  { return _mapping; }

private:
    std::vector<std::pair<std::string, std::shared_ptr<Texture> > > _mapping;
    Shader _shader;
};

