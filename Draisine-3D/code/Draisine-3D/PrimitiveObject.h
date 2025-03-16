#pragma once

#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include "Utilities.h"
#include "Object.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"


// All implementaion of class PrimitiveObjcet MUST:
// - implement initVertexBuffer()
// - implement initIndexBuffer()
// - call method init() in all constructors
// - call constructor PrimitiveObject(const std::string& textureFilePath, const glm::vec3& possition, const glm::vec3& rotation, const glm::vec3& size) in all contructors
class PrimitiveObject : public Object {
public:
	PrimitiveObject(std::shared_ptr<Texture> texture,
		const glm::vec3& possition = { 0.0, 0.0, 0.0 },
		const glm::vec3& rotation = { 0.0, 0.0, 0.0 },
		const glm::vec3& size = { 1.0f, 1.0f, 1.0f })
		: _texture(texture), _possition(possition), _rotation(rotation), _size(size) {
	}

	void move(const glm::vec3& vec) {
		_possition += vec;
	};

	void rotate(const glm::vec3& vec) {
		_rotation += vec;
	};

	void scale(const glm::vec3& vec) {
		_size += vec;
	};

	void reposition(const glm::vec3& vec)
	{
		_possition = vec;
	};
	
	void render(std::shared_ptr<Shader> shader, bool tex=true) {
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, _possition);
		model = glm::rotate(model, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, _size);

		shader->bind();
		shader->setUniformMat4("MODEL", model);

		if (tex)
		{
			_texture->bind(0);
			shader->setUniformInt("TEXTURE", 0);
		}
		_vertexArray->bind();
		_indexBuffer->bind();
		GLCall(glDrawElements(GL_TRIANGLES, _indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr));
		_indexBuffer->unbind();
		_vertexArray->unbind();
	}

	std::unique_ptr<VertexBufferLayout> initVertexBufferLayout() {
		auto vertexBufferLayout = std::make_unique<VertexBufferLayout> ();
		// vertex location
		vertexBufferLayout->addFloat(3);
		// vertex normal
		vertexBufferLayout->addFloat(3);
		// texture coordinate
		vertexBufferLayout->addFloat(2);

		return vertexBufferLayout;
	};

	const glm::vec3 getPos()
	{
		return _possition;
	}
	
	const glm::vec3 getAngle()
	{
		return _rotation;
	}

protected:
	virtual std::unique_ptr<VertexBuffer> initVertexBuffer() = 0;
	virtual std::unique_ptr<IndexBuffer> initIndexBuffer() = 0;

	void init() {
		_vertexBuffer = initVertexBuffer();
		_vertexBufferLayout = initVertexBufferLayout();

		_vertexArray = std::make_unique<VertexArray>();
		_vertexArray->link(*_vertexBuffer, *_vertexBufferLayout);

		_indexBuffer = initIndexBuffer();
	}

private:
	std::shared_ptr<Texture> _texture; //if necessary it can be shared resource

	std::unique_ptr<VertexBuffer> _vertexBuffer;
	std::unique_ptr <IndexBuffer> _indexBuffer;
	std::unique_ptr<VertexArray> _vertexArray;

	std::unique_ptr<VertexBufferLayout> _vertexBufferLayout; //if necessary it can be shared resource

	glm::vec3 _possition;
	glm::vec3 _rotation;
	glm::vec3 _size;

};