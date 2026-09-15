#include "common.h"

#include <glm/glm.hpp>

HGE_Vec3 HGE_GetForwardVector(HGE_Vec3 _rotator)
{
	glm::vec3 forward;
	forward.x = cos(glm::radians(_rotator.x)) * cos(glm::radians(_rotator.y));
	forward.y = sin(glm::radians(_rotator.x));
	forward.z = cos(glm::radians(_rotator.x)) * sin(glm::radians(_rotator.y));
	forward = glm::normalize(forward);
	return {forward.x, forward.y, forward.z};
}

HGE_Vec3 HGE_GetRightVector(HGE_Vec3 _rotator)
{
	HGE_Vec3 f = HGE_GetForwardVector(_rotator);
	glm::vec3 forward{f.x, f.y, f.z};

	glm::vec3 right = glm::normalize(glm::cross(
		forward,
		glm::vec3(0.0f, 1.0f, 0.0f)
	));
	return {right.x, right.y, right.z};
}

HGE_Vec3 HGE_GetUpVector(HGE_Vec3 _rotator)
{
	HGE_Vec3 f = HGE_GetForwardVector(_rotator);
	glm::vec3 forward{f.x, f.y, f.z};
	HGE_Vec3 r = HGE_GetRightVector(_rotator);
	glm::vec3 right{r.x, r.y, r.z};

	glm::vec3 up = glm::normalize(glm::cross(
		right,
		forward
	));

	return {up.x, up.y, up.z};
}