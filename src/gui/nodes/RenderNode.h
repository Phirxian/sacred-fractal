#pragma once

#include "NodeType.h"
#include "../../graphics/Program.h"
#include <iostream>

namespace sacred
{
    namespace gui
    {
        static const char *FragmentShaderNodeValue[] = {"base.frag", "forcefield.frag", "spheremarching.frag", "swr.frag", "warp.frag", "sample.frag", "tentacle.frag", "voronoi.frag"};
        static const char *VertexShaderNodeValue[] = {"base.vert", "move.vert", "scale.vert", "test.vert"};

        class RenderNode : public Node
        {
            public:
                static const int TYPE = MNT_RENDER_NODE;
            public:
                RenderNode(osg::Group *grp, osg::Camera *cam, const ImVec2 &pos) : Node(),
                    root(grp),        camera(cam),
                    fragmentIndex(0), vertexIndex(0)
                {
                    static int numFrag = (int)(sizeof(FragmentShaderNodeValue)/sizeof(FragmentShaderNodeValue[0]));
                    static int numVert = (int)(sizeof(VertexShaderNodeValue)/sizeof(VertexShaderNodeValue[0]));

                    program = std::make_shared<graphics::Program>(camera);
                    program->setShader("shaders/frag/base.frag");
                    program->setShader("shaders/vert/test.vert", graphics::EST_VERTEX);
                    root->addChild(program->getNode());

                    init(program->getTypeName(), pos, "in", "", TYPE);

                    fields.addFieldEnum(
                        &fragmentIndex, numFrag,
                        &FragmentTextFromEnumIndex,
                        "Fragment", "Choose your favourite"
                    );

                    fields.addFieldEnum(
                        &vertexIndex, numVert,
                        &VertexTextFromEnumIndex,
                        "Vertex", "Choose your favourite"
                    );

                    userID = TYPE;
                    user_ptr = program.get();
                }

                ~RenderNode()
                {
                    root->removeChild(program->getNode());
                }

                virtual void onEdited()
                {
                    std::string filename("shaders/frag/");
                    filename += FragmentShaderNodeValue[fragmentIndex];
                    program->setShader(filename.c_str());

                    filename = "shaders/vert/";
                    filename += VertexShaderNodeValue[vertexIndex];
                    program->setShader(filename.c_str(), graphics::EST_VERTEX);
                }
            protected:

                // Support static method for enumIndex (the signature is the same used by ImGui::Combo(...))
                static bool FragmentTextFromEnumIndex(void *, int value, const char **pTxt)
                {
                    if(!pTxt)
                        return false;

                    static int numValues = (int)(sizeof(FragmentShaderNodeValue)/sizeof(FragmentShaderNodeValue[0]));

                    if(value>=0 && value<numValues)
                        *pTxt = FragmentShaderNodeValue[value];
                    else *pTxt = "UNKNOWN";

                    return true;
                }
                static bool VertexTextFromEnumIndex(void *, int value, const char **pTxt)
                {
                    if(!pTxt)
                        return false;

                    static int numValues = (int)(sizeof(VertexShaderNodeValue)/sizeof(VertexShaderNodeValue[0]));

                    if(value>=0 && value<numValues)
                        *pTxt = VertexShaderNodeValue[value];
                    else *pTxt = "UNKNOWN";

                    return true;
                }

                virtual void getTooltip() const
                {
                    ImGui::BeginTooltip();

                        ImGui::PlotLines("left", program->getData(0), sounds::FRAME_SIZE, 0, "", -1.0f, 1.0f, ImVec2(0,80));
                        if(program->isStereo())
                            ImGui::PlotLines("right", program->getData(1), sounds::FRAME_SIZE, 0, "", -1.0f, 1.0f, ImVec2(0,80));

                    ImGui::EndTooltip();
                }

                virtual const char *getInfo() const
                {
                    return "ShaderNode info.\n\nThis help to select fragment shader source.";
                }
            private:
                osg::Group *root;
                osg::Camera *camera;
                std::shared_ptr<graphics::Program> program;
                int fragmentIndex;
                int vertexIndex;
        };
    }
}
