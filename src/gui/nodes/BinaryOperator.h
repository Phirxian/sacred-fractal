#pragma once

#include "NodeType.h"
#include "../../graphics/Program.h"
#include <iostream>

namespace sacred
{
    namespace gui
    {
        template<typename T>
        class BinaryNode : public Node
        {
            public:
                static const int TYPE = MNT_BINARY_NODE;

            public:
                BinaryNode(const ImVec2 &pos) : Node()
                {
                    generator = std::make_shared<T>();
                    init(generator->getTypeName(), pos, "in1;in2", "out", TYPE);
                    userID = TYPE;
                    user_ptr = generator.get();
                }

                ~BinaryNode()
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
                    return "BinaryNode info.";
                }

            private:
                std::shared_ptr<T> generator;
        };
    }
}
