#include "graphics/ob_render.h"
#include "graphics/ob_shader.h"

#include "assets/ob_asset.h"

#include "ecs/ob_ecs.h"

#include <glad/glad.h>

#include <cglm/cglm.h>

#include <stdio.h>

extern struct obsidian_component __ob_ecs_getentcomp(const ob_entity_t, const enum obsidian_component_type, uint32_t*);

extern bool __ob_buf_bindvao(uint32_t);
extern void __ob_buf_unbindvao(void);
extern bool __ob_tex_bindtex(uint32_t);
extern void __ob_tex_unbindtex(void);

extern ob_entity_t* __ob_ents;

static bool __ob_render_isentitydrawready(const ob_entity_t);
static void __ob_render_prepmodelmatrix(const float*, const float*, const float, mat4*);

void OBRNDRdrawEntities()
{

}

void OBRNDRdrawEntity(const ob_entity_t entity)
{
    if (!__ob_render_isentitydrawready(entity)) return;

    const struct obsidian_asset* texture = OBECSgetComponentData(entity, OB_TEXTURE_COMPONENT);
    const struct obsidian_asset* model = OBECSgetComponentData(entity, OB_MODEL_COMPONENT);
    const float* pos = OBECSgetComponentData(entity, OB_POSITION_COMPONENT);
    const float* scale = OBECSgetComponentData(entity, OB_SCALE_COMPONENT);
    const float rotation = *(float*)OBECSgetComponentData(entity, OB_ROTATION_COMPONENT);

    mat4 matrix;
    __ob_render_prepmodelmatrix(pos, scale, rotation, &matrix);

    // TODO: allow entities to have linked shader programs
    OBSHDRsetmat4f(0, "model", matrix);

    glActiveTexture(GL_TEXTURE0);
    __ob_buf_bindvao(model->mdlprim.vaoi);
    __ob_tex_bindtex(texture->texture.texi);
    // TODO: don't hardcode param #2
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    __ob_buf_unbindvao();
    __ob_tex_unbindtex();
}

static bool __ob_render_isentitydrawready(const ob_entity_t ent)
{
    bool has_tex, has_mdl, has_pos, has_scale, has_rot;
    has_tex = OBECShasComponent(ent, OB_TEXTURE_COMPONENT);
    has_mdl = OBECShasComponent(ent, OB_MODEL_COMPONENT);
    has_pos = OBECShasComponent(ent, OB_POSITION_COMPONENT);
    has_scale = OBECShasComponent(ent, OB_SCALE_COMPONENT);
    has_rot = OBECShasComponent(ent, OB_ROTATION_COMPONENT);
    return has_tex && has_mdl && has_pos && has_scale && has_rot;
}

static void __ob_render_prepmodelmatrix(const float* pos, 
                                        const float* scale, 
                                        const float rot, 
                                        mat4* model)
{
    glm_mat4_identity(*model);
    glm_translate(*model, (float*)pos);
    glm_translate(*model, (vec3) { 0.5f * scale[0], 0.5f * scale[1], 0.0f });
    glm_rotate(*model, glm_rad(rot), (vec3) { 0.0f, 0.0f, 1.0f });
    glm_translate(*model, (vec3) { -0.5f * scale[0], -0.5f * scale[1], 0.0f });
    glm_scale(*model, (vec3) { scale[0], scale[1], 1.0f });
}