#pragma once

#include "NodeType.h"
#include "../../graphics/Program.h"
#include <iostream>

namespace sacred
{
    namespace gui
    {
        template<typename T>
        class GeneratorNode : public Node
        {
            public:
                static const int TYPE = MNT_GENERATOR_NODE;

            public:
                GeneratorNode(const ImVec2 &pos) : Node()
                {
                    generator = std::make_shared<T>();
                    init(generator->getTypeName(), pos, "", "out", TYPE);
                    userID = TYPE;
                    user_ptr = generator.get();

                    m_oscFrequency = generator->m_oscFrequency;
                    m_oscAmplitude = generator->m_oscAmplitude;
                    m_oscPhase     = generator->m_oscPhase;

                    fields.addField(&m_oscFrequency, 1, "frequency", "signal freq", 1, 0.1, 10000.0, nullptr, false);
                    fields.addField(&m_oscAmplitude, 1, "amplitude",  "signal amp", 9, 0.0, 1.0, nullptr, false);
                    fields.addField(&m_oscPhase    , 1, "phase",    "signal phase", 9, 0.0, 2*M_PI, nullptr, false);
                }

                ~GeneratorNode()
                {
                }

                virtual void onEdited()
                {
                    generator->setFrequency(m_oscFrequency);
                    generator->changeFrequency(m_oscFrequency);
                    generator->setAmplitude(m_oscAmplitude);
                    generator->setPhase(m_oscPhase);
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
                    return "Generator info.";
                }

            private:
                float m_oscFrequency;
                float m_oscAmplitude;
                float m_oscPhase;
                std::shared_ptr<T> generator;
        };
    }
}
