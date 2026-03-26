#pragma once

#include <cstdint>

namespace gfx
{
    /**
     * @brief Off-screen render target (FBO) with a color texture and depth renderbuffer.
     *
     * Used for rendering a viewport into a texture that can be displayed in ImGui.
     */
    class RenderTarget final
    {
    public:
        RenderTarget() = default;
        ~RenderTarget();

        RenderTarget(const RenderTarget&) = delete;
        RenderTarget& operator=(const RenderTarget&) = delete;

        /** @brief Create the framebuffer object with color and depth attachments.
         *  @param width Framebuffer width in pixels, must be > 0.
         *  @param height Framebuffer height in pixels, must be > 0.
         *  @throws std::runtime_error if width/height <= 0 or framebuffer creation fails.
         *  @note Safe to call multiple times; subsequent calls are no-ops if already created.
         */
        void create(int width, int height);

        /** @brief Resize the framebuffer attachments to new dimensions.
         *  @param width New framebuffer width in pixels, must be > 0.
         *  @param height New framebuffer height in pixels, must be > 0.
         *  @throws std::runtime_error if framebuffer becomes incomplete.
         *  @note If not yet created, calls create() instead.
         */
        void resize(int width, int height);

        /** @brief Destroy the framebuffer object and all attachments.
         *  @note Safe to call multiple times; subsequent calls are no-ops.
         */
        void destroy();

        /** @brief Bind this render target as the active framebuffer. */
        void bind() const;

        /** @brief Unbind any active framebuffer (restore default). */
        static void unbind();

        /** @brief Get the current framebuffer width in pixels. */
        int width() const { return width_; }

        /** @brief Get the current framebuffer height in pixels. */
        int height() const { return height_; }

        /** @brief Get the OpenGL framebuffer object ID. */
        std::uint32_t fbo() const { return fbo_; }

        /** @brief Get the OpenGL color texture ID. */
        std::uint32_t colorTexture() const { return colorTex_; }

    private:
        void createAttachments(int width, int height);
        void destroyAttachments();

        std::uint32_t fbo_ = 0;         // Framebuffer object ID
        std::uint32_t colorTex_ = 0;    // Color texture ID
        std::uint32_t depthRbo_ = 0;    // Depth renderbuffer ID
        int width_ = 0;
        int height_ = 0;
        bool created_ = false;
    };
}
