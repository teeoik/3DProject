#include "gfx/RenderTarget.h"

#include <stdexcept>

#include <glad/glad.h>

namespace gfx
{
    RenderTarget::~RenderTarget()
    {
        destroy();
    }

    void RenderTarget::create(int width, int height)
    {
        if (created_)
            return;

        if (width <= 0 || height <= 0)
            throw std::runtime_error("RenderTarget::create: invalid size");

        glGenFramebuffers(1, &fbo_);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

        createAttachments(width, height);

        const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            destroy();
            throw std::runtime_error("RenderTarget::create: framebuffer incomplete");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        width_ = width;
        height_ = height;
        created_ = true;
    }

    void RenderTarget::resize(int width, int height)
    {
        if (!created_)
        {
            create(width, height);
            return;
        }

        if (width == width_ && height == height_)
            return;

        if (width <= 0 || height <= 0)
            return;

        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

        destroyAttachments();
        createAttachments(width, height);

        const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            throw std::runtime_error("RenderTarget::resize: framebuffer incomplete");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        width_ = width;
        height_ = height;
    }

    void RenderTarget::destroy()
    {
        if (!created_)
            return;

        destroyAttachments();

        if (fbo_ != 0)
        {
            glDeleteFramebuffers(1, &fbo_);
            fbo_ = 0;
        }

        width_ = 0;
        height_ = 0;
        created_ = false;
    }

    void RenderTarget::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    }

    void RenderTarget::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderTarget::createAttachments(int width, int height)
    {
        // Create color texture
        glGenTextures(1, &colorTex_);
        glBindTexture(GL_TEXTURE_2D, colorTex_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex_, 0);

        // Create depth/stencil renderbuffer
        glGenRenderbuffers(1, &depthRbo_);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRbo_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRbo_);

        // Unbind textures and renderbuffers
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void RenderTarget::destroyAttachments()
    {
        if (colorTex_ != 0)
        {
            glDeleteTextures(1, &colorTex_);
            colorTex_ = 0;
        }

        if (depthRbo_ != 0)
        {
            glDeleteRenderbuffers(1, &depthRbo_);
            depthRbo_ = 0;
        }
    }
}
