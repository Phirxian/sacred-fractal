#include "Editor.h"
#include <imgui_internal.h>
#include <iostream>

#include "sounds/operators/Mono.h"
#include "sounds/operators/Inverser.h"
#include "sounds/operators/SignalsAdditioner.h"
#include "sounds/operators/SignalsMultiplier.h"
#include "sounds/operators/SignalsSubstractor.h"

#include "sounds/generator/BounceGenerator.h"
#include "sounds/generator/SawGenerator.h"
#include "sounds/generator/SquareGenerator.h"
#include "sounds/generator/SineGenerator.h"
#include "sounds/operators/SignalsAdditioner.h"
#include "sounds/generator/TriangleGenerator.h"

using namespace ImGui;
using namespace sacred::sounds;

#include "nodes/RenderNode.h"
#include "nodes/GeneratorNode.h"
#include "nodes/UnaryOperator.h"
#include "nodes/BinaryOperator.h"

namespace sacred
{
    namespace gui
    {
        Editor::Editor(osg::Camera *cam, osg::Group *grp) noexcept
            : opened(true), camera(cam), root(grp),
              nge(true, true, true)
        {
            nge.setNodeCallback(this);
            nge.setLinkCallback(this);
            nge.setNodeEditedCallbackTimeThreshold(0);
            nge.show_style_editor = true;
            nge.show_load_save_buttons = true;

            gen[0] = new GeneratorNode<BounceGenerator>(ImVec2(40,100));
            gen[1] = new GeneratorNode<SawGenerator>(ImVec2(40,200));
            gen[2] = new GeneratorNode<SineGenerator>(ImVec2(40,300));
            gen[3] = new GeneratorNode<SquareGenerator>(ImVec2(40,400));
            gen[4] = new GeneratorNode<TriangleGenerator>(ImVec2(40,500));

            gen[5] = new UnaryNode<Mono>(ImVec2(300,100));
            gen[6] = new UnaryNode<Inverser>(ImVec2(300,200));
            gen[7] = new BinaryNode<SignalsAdditioner>(ImVec2(300,300));
            gen[8] = new BinaryNode<SignalsMultiplier>(ImVec2(300,400));
            gen[9] = new BinaryNode<SignalsSubstractor>(ImVec2(300,500));

            node = new RenderNode(root, camera, ImVec2(650,150));
        }

        Editor::~Editor() noexcept
        {
        }

        void Editor::operator()(ImGui::Node *&node, ImGui::NodeGraphEditor::NodeState state, ImGui::NodeGraphEditor &editor)
        {
            if(state == ImGui::NodeGraphEditor::NS_DELETED)
            {
            }
            else if(state == ImGui::NodeGraphEditor::NS_EDITED)
            {
            }
        }

        void Editor::operator()(const ImGui::NodeLink &link, ImGui::NodeGraphEditor::LinkState state, ImGui::NodeGraphEditor &editor)
        {
            if(state == ImGui::NodeGraphEditor::LS_ADDED)
            {
                auto out =(SignalSource*)link.InputNode->user_ptr;
                auto in =(SignalSource*)link.OutputNode->user_ptr;
                in->get(link.OutputSlot) = out->getptr();
            }

            if(state == ImGui::NodeGraphEditor::LS_DELETED)
            {
                auto sig =(SignalSource*)link.OutputNode->user_ptr;
                sig->get(link.OutputSlot) = nullptr;
            }
        }

        void Editor::operator()() noexcept
        {
            ImGui::SetNextWindowSize(ImVec2(700,600), ImGuiSetCond_FirstUseEver);

            if(ImGui::Begin("Example: Custom Node Graph", NULL))
            {
                if(nge.isInited())
                {
                    nge.addNode(node);
                    for(int i = 0; i<10; ++i)
                        nge.addNode(gen[i]);
                    nge.addLink(gen[0], 0, node, 0);
                }

                nge.render();
            }

            ImGui::End();
        }
    }
}
