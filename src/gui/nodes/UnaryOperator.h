#pragma once

#include "NodeType.h"
#include "../../graphics/Program.h"
#include <iostream>

namespace sacred
{
    namespace gui
    {
        template<typename T>
        class UnaryNode : public ImGui::Node
        {
            public:
                static const int TYPE = MNT_UNARY_NODE;

            public:
                UnaryNode(const ImVec2 &pos) : Node()
                {
                    generator = std::make_shared<T>();
                    init(generator->getTypeName(), pos, "in", "out", TYPE);
                    userID = TYPE;
                    user_ptr = generator.get();
                }

                ~UnaryNode()
                {
                }

                virtual void onEdited()
                {
                }
            protected:
                virtual void getTooltip() const
                {
                    ImGui::BeginTooltip();

                        ImGui::PlotLines("left", generator->getFrame()->samples[0], sounds::FRAME_SIZE, 0, "", -1.0f, 1.0f, ImVec2(0,80));
                        if(generator->isStereo())
                            ImGui::PlotLines("right", generator->getFrame()->samples[1], sounds::FRAME_SIZE, 0, "", -1.0f, 1.0f, ImVec2(0,80));

                    ImGui::EndTooltip();
                }

                virtual const char *getInfo() const
                {
                    return "UnaryNode info.";
                }

            private:
                std::shared_ptr<T> generator;
        };
    }
}
