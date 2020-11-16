#ifndef __SACRED_GUI_EDITOR__
#define __SACRED_GUI_EDITOR__

#include "warpper/ImGuiHandler.h"
#include "addons/imguinodegrapheditor/imguinodegrapheditor.h"

namespace sacred
{
    namespace gui
    {
        class Editor :
            public GuiCallback,
            public ImGui::NodeGraphEditor::NodeCallback,
            public ImGui::NodeGraphEditor::LinkCallback
        {
            public:
                Editor(osg::Camera*, osg::Group*) noexcept;
                virtual ~Editor() noexcept;

                virtual void operator()(ImGui::Node *&node, ImGui::NodeGraphEditor::NodeState state, ImGui::NodeGraphEditor &editor);
                virtual void operator()(const ImGui::NodeLink &link, ImGui::NodeGraphEditor::LinkState state, ImGui::NodeGraphEditor &editor);

                virtual void operator()() noexcept;

            protected:
                ImGui::NodeGraphEditor nge;
                ImGui::Node *node;
                ImGui::Node *gen[10];

            private:
                bool opened;
                osg::Group *root;
                osg::Camera *camera;
        };
    }
}

#endif
