#include "engine_renderer.h"

#include "engine_buffers.h"
#include "engine_shaders.h"
#include "engine_textures.h"
#include "engine_entity.h"

#include <cglm/cglm.h>

static inline void getModelMatrix(vec2 pos, vec2 size, float rotation, mat4 model)
{
	glm_translate(model, (vec3) { pos[0], pos[1], 0.0f }); // move to world position
	glm_translate(model, (vec3) { 0.5f * size[0], 0.5f * size[1], 0.0f }); // move origin to center of quad
	glm_rotate(model, glm_rad(rotation), (vec3) { 0.0f, 0.0f, 1.0f }); // rotate over z-axis
	glm_translate(model, (vec3) { -0.5f * size[0], -0.5f * size[1], 0.0f }); // reset origin
	glm_scale(model, (vec3) { size[0], size[1], 1.0f }); // scale up to width,height
}

void engineRenderEntity(EngineContext* ctx, EngineEntityKey key)
{
	mat4 model;
	EngineEntity* ent = engineGetEntity(ctx, key);
	assert(ent != NULL);

	glm_mat4_identity(model);
	engineUseShader(ctx);
	getModelMatrix((vec2) { ent->x, ent->y }, (vec2) { ent->width, ent->height }, 0.0f, model);
	engineSetMatrix4fv(ctx, "model", model);

	glActiveTexture(GL_TEXTURE0);
	engineBindTexture(ctx, ent->texkey);

	assert(glIsVertexArray(ctx->vao) != 0);

	engineBindVAO(ctx->vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	engineBindVAO(0);
}