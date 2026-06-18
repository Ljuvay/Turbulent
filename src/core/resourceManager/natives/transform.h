#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
	glm::vec3 translation = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::mat4 getWorldTransform() const
	{
		glm::mat4 worldTransform = glm::mat4(1.0f);

		worldTransform = glm::translate(worldTransform, translation);

		worldTransform = glm::rotate(worldTransform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		worldTransform = glm::rotate(worldTransform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		worldTransform = glm::rotate(worldTransform, glm::radians(rotation.z), glm::vec3(0, 0, 1));

		worldTransform = glm::scale(worldTransform, scale);
		return worldTransform;
	}
};

#endif // !1
