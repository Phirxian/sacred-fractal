#include "ImGuiDrawer.h"
#include "../../openvr/device/Texture.h"
#include <osg/GL>

static const GLchar *vertex_shader =
    "#version 330\n"
    "uniform mat4 ProjMtx;\n"
    ""
    "in vec2 Position;\n"
    "in vec2 UV;\n"
    "in vec4 Color;\n"
    ""
    "out vec2 Frag_UV;\n"
    "out vec4 Frag_Color;\n"
    ""
    "void main()\n"
    "{\n"
    "	Frag_UV = UV;\n"
    "	Frag_Color = Color;\n"
    "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
    "}\n";

static const GLchar* fragment_shader =
    "#version 330\n"
    "uniform sampler2D Texture;\n"
    ""
    "in vec2 Frag_UV;\n"
    "in vec4 Frag_Color;\n"
    ""
    "out vec4 Out_Color;\n"
    ""
    "void main()\n"
    "{\n"
    "	Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
    "}\n";

namespace sacred
{
    namespace gui
    {
        ImGuiRenderDrawLists::ImGuiRenderDrawLists()
        {
            g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
            g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
            g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
            g_VboHandle = 0, g_VaoHandle = 0, g_ElementsHandle = 0;
        }

        bool ImGuiRenderDrawLists::init(osg::RenderInfo &renderInfo)
        {
            osg::State* state = renderInfo.getState();
            auto *glExt = getGLExtensions(*state);

            // Backup GL state
            GLint last_texture, last_array_buffer, last_vertex_array;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

            g_ShaderHandle = glExt->glCreateProgram();
            g_VertHandle = glExt->glCreateShader(GL_VERTEX_SHADER);
            g_FragHandle = glExt->glCreateShader(GL_FRAGMENT_SHADER);

            glExt->glShaderSource(g_VertHandle, 1, &vertex_shader, 0);
            glExt->glShaderSource(g_FragHandle, 1, &fragment_shader, 0);
            glExt->glCompileShader(g_VertHandle);
            glExt->glCompileShader(g_FragHandle);
            glExt->glAttachShader(g_ShaderHandle, g_VertHandle);
            glExt->glAttachShader(g_ShaderHandle, g_FragHandle);
            glExt->glLinkProgram(g_ShaderHandle);

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);

            g_AttribLocationTex = glExt->glGetUniformLocation(g_ShaderHandle, "Texture");
            g_AttribLocationProjMtx = glExt->glGetUniformLocation(g_ShaderHandle, "ProjMtx");
            g_AttribLocationPosition = glExt->glGetAttribLocation(g_ShaderHandle, "Position");
            g_AttribLocationUV = glExt->glGetAttribLocation(g_ShaderHandle, "UV");
            g_AttribLocationColor = glExt->glGetAttribLocation(g_ShaderHandle, "Color");

            glExt->glGenBuffers(1, &g_VboHandle);
            glExt->glGenBuffers(1, &g_ElementsHandle);

            glExt->glGenVertexArrays(1, &g_VaoHandle);
            glExt->glBindVertexArray(g_VaoHandle);
            glExt->glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
            glExt->glEnableVertexAttribArray(g_AttribLocationPosition);
            glExt->glEnableVertexAttribArray(g_AttribLocationUV);
            glExt->glEnableVertexAttribArray(g_AttribLocationColor);

            #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
                glExt->glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
                glExt->glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
                glExt->glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
            #undef OFFSETOF

            // Restore modified GL state
            glBindTexture(GL_TEXTURE_2D, last_texture);
            glExt->glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
            glExt->glBindVertexArray(last_vertex_array);
        }

        ImGuiRenderDrawLists::~ImGuiRenderDrawLists()
        {
//            if (g_VaoHandle)      glDeleteVertexArrays(1, &g_VaoHandle);
//            if (g_VboHandle)      glDeleteBuffers(1, &g_VboHandle);
//            if (g_ElementsHandle) glDeleteBuffers(1, &g_ElementsHandle);

//            g_VaoHandle = g_VboHandle = g_ElementsHandle = 0;
//
//            glDetachShader(g_ShaderHandle, g_VertHandle);
//            glDeleteShader(g_VertHandle);
//            g_VertHandle = 0;
//
//            glDetachShader(g_ShaderHandle, g_FragHandle);
//            glDeleteShader(g_FragHandle);
//            g_FragHandle = 0;
//
//            glDeleteProgram(g_ShaderHandle);
//            g_ShaderHandle = 0;
        }

        void ImGuiRenderDrawLists::operator() (ImDrawData *draw) noexcept
        {
            draw_data = draw;
        }

        void ImGuiRenderDrawLists::render(osg::RenderInfo &renderInfo) noexcept
        {
            osg::State* state = renderInfo.getState();
            auto *glExt = getGLExtensions(*state);

            // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
            ImGuiIO& io = ImGui::GetIO();
            int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
            int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
            draw_data->ScaleClipRects(io.DisplayFramebufferScale);

            // Backup GL state
            GLint last_program;              glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
            GLint last_texture;              glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            GLint last_active_texture;       glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
            GLint last_array_buffer;         glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
            GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
            GLint last_vertex_array;         glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
            GLint last_blend_src;            glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
            GLint last_blend_dst;            glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
            GLint last_blend_equation_rgb;   glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
            GLint last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);

            GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
            GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
            GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
            GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

            // save the current state
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled

            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_SCISSOR_TEST);
            glActiveTexture(GL_TEXTURE0);
            glDisable(GL_LIGHTING);

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);

            const float ortho_projection[4][4] =
            {
                { 2.0f/io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
                { 0.0f,                  2.0f/-io.DisplaySize.y, 0.0f, 0.0f },
                { 0.0f,                  0.0f,                  -1.0f, 0.0f },
                {-1.0f,                  1.0f,                   0.0f, 1.0f },
            };

            glExt->glUseProgram(g_ShaderHandle);
            glExt->glUniform1i(g_AttribLocationTex, 0);
            glExt->glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
            glExt->glBindVertexArray(g_VaoHandle);

            // Render command lists
            #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

            for(int n = 0; n < draw_data->CmdListsCount; n++)
            {
                #if 0
                const ImDrawList *cmd_list = draw_data->CmdLists[n];
                const unsigned char *vtx_buffer = (const unsigned char *)&cmd_list->VtxBuffer.front();
                const ImDrawIdx *idx_buffer = &cmd_list->IdxBuffer.front();

                glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void *)(vtx_buffer + OFFSETOF(ImDrawVert, pos)));
                glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void *)(vtx_buffer + OFFSETOF(ImDrawVert, uv)));
                glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void *)(vtx_buffer + OFFSETOF(ImDrawVert, col)));
                #else
                const ImDrawList* cmd_list = draw_data->CmdLists[n];
                const ImDrawIdx* idx_buffer = 0;

                glExt->glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
                glExt->glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

                glExt->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
                glExt->glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);
                #endif

                for(int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
                {
                    const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];

                    if(pcmd->UserCallback)
                        pcmd->UserCallback(cmd_list, pcmd);
                    else
                    {
                        glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                        glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                        glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer);
                    }

                    idx_buffer += pcmd->ElemCount;
                }
            }

            #undef OFFSETOF

            // Restore modified state
            glBindTexture(GL_TEXTURE_2D, last_texture);

            // Restore modified GL state
            glExt->glUseProgram(last_program);
            glActiveTexture(last_active_texture);
            glBindTexture(GL_TEXTURE_2D, last_texture);
            glExt->glBindVertexArray(last_vertex_array);
            glExt->glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
            glExt->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
            glExt->glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
            glBlendFunc(last_blend_src, last_blend_dst);

            if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
            if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
            if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
            if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
        }
    }
}
