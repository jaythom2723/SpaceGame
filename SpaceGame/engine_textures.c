#include "engine_textures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

static EngineTexturePair* ptr = NULL;
static EngineTexturePair* lastElem = NULL;
static EngineTexturePair* end = NULL;

static EngineTexturePair* _engineGetTexturePair(EngineContext* ctx, unsigned int key, unsigned int* index);
static EngineTexturePair* _engineGetTexturePairIndex(EngineContext* ctx, unsigned int index);

const EBOOL engineCreateTexture(EngineContext* ctx, unsigned int key)
{
	if (ptr == NULL)
	{
		ptr = ctx->textures;
		lastElem = ctx->textures;
		end = ctx->textures + ENGINE_MAX_TEXTURES;
	}

	// do nothing if we're at the max amount of textures
	if (ptr >= end)
		return EFALSE;

	EngineTexture* texture = malloc(sizeof(EngineTexture));
	if (texture == NULL)
	{
		fprintf(stderr, "Could not allocate enough memory for a texture!\n");
		return EFALSE;
	}

	glGenTextures(1, &texture->id);

	EngineTexturePair pair = {
		.key = key,
		.value = texture
	};

	(*ptr) = pair;
	ctx->ntextures++;

	if (ptr != lastElem)
	{
		ptr = lastElem;
	}
	else
	{
		ptr++;
		lastElem++;
	}

	return ETRUE;
}

const EBOOL engineGenerateTexture(EngineContext* ctx, unsigned int key, const char* path, const EBOOL alpha)
{
	EngineTexturePair* pair = NULL;

	for (int i = 0; i < (int)(ptr - ctx->textures); i++)
	{
		if (ctx->textures[i].key != key)
			continue;
		pair = &ctx->textures[i];
		break;
	}

	if (pair == NULL)
		return EFALSE;

	pair->value->width = 0;
	pair->value->height = 0;
	pair->value->internalformat = GL_RGB;
	pair->value->imageformat = GL_RGB;
	pair->value->wraps = GL_REPEAT;
	pair->value->wrapt = GL_REPEAT;
	pair->value->filtermax = GL_LINEAR;
	pair->value->filtermin = GL_LINEAR;

	if (alpha)
	{
		pair->value->internalformat = GL_RGBA;
		pair->value->imageformat = GL_RGBA;
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data == NULL)
	{
		fprintf(stderr, "Could not find texture file!\n");
		return EFALSE;
	}

	glBindTexture(GL_TEXTURE_2D, pair->value->id);
	glTexImage2D(GL_TEXTURE_2D, 0, pair->value->internalformat, width, height, 0, pair->value->imageformat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, pair->value->wraps);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, pair->value->wrapt);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pair->value->filtermin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pair->value->filtermax);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	
	return ETRUE;
}

void engineGenTextureCheckMissing(EngineContext* ctx, unsigned int key, const char* path, const EBOOL alpha)
{
	if (engineCreateTexture(ctx, key) != EFALSE)
		if (engineGenerateTexture(ctx, key, path, alpha) == EFALSE)
			engineGenerateTexture(ctx, ENGINE_MISSING_TEXTURE_KEY, "Textures/_missing_texture.png", EFALSE);
}

void engineBindTexture(EngineContext* ctx, unsigned int key)
{
	int index;
	EngineTexturePair* pair = _engineGetTexturePair(ctx, key, &index);
	assert(pair != NULL);
	assert(pair->value != NULL);
	assert(glIsTexture(pair->value->id) != 0);
	glBindTexture(GL_TEXTURE_2D, pair->value->id);
}

void engineDestroyAllTextures(EngineContext* ctx)
{
	for (int i = 0; i < (int)(ptr - ctx->textures); i++)
	{
		EngineTexturePair* pair = _engineGetTexturePairIndex(ctx, i);
		if (pair->value == NULL)
			continue;

		glDeleteTextures(1, &pair->value->id);
		free(pair->value);
		pair->value = NULL;
		pair->key = 0;
	}

	ptr = ctx->textures;
	lastElem = ctx->textures;
	ctx->ntextures = 0;
}

void engineDestroyTexture(EngineContext* ctx, unsigned int key)
{
	int index;
	EngineTexturePair* pair = _engineGetTexturePair(ctx, key, &index);

	glDeleteTextures(1, &pair->value->id);
	free(pair->value);
	pair->value = NULL;
	pair->key = 0;

	ctx->ntextures--;

	if ((EngineTexturePair*)(ctx->textures + index) == lastElem)
	{
		lastElem--;
		ptr--;
	}
	else
	{
		ptr = (EngineTexturePair*)(ctx->textures + index);
	}
}

// internal functions
EngineTexturePair* _engineGetTexturePair(EngineContext* ctx, unsigned int key, unsigned int* index)
{
	for (int i = 0; i < (int)(ptr - ctx->textures); i++)
	{
		if (ctx->textures[i].key == key)
		{
			(*index) = i;
			return &ctx->textures[i];
		}
	}
	return NULL;
}

EngineTexturePair* _engineGetTexturePairIndex(EngineContext* ctx, unsigned int index)
{
	return &ctx->textures[index];
}

