#include "editor.h"
#include "editor/ui/editor_ui.h"
#include "editor/ui/world_ui.h"
#include "editor/ui/property_ui.h"
#include "editor/ui/game_ui.h"
#include "editor/ui/asset_ui.h"
#include "editor/ui/log_ui.h"
#include "runtime/engine.h"
#include "runtime/core/base/macro.h"
#include "runtime/function/render/window_system.h"
#include "runtime/resource/asset/asset_manager.h"
#include "runtime/function/render/render_system.h"

namespace Bamboo
{
    void Editor::init(Engine *engine)
    {
        m_engine = engine;

        g_runtime_context.windowSystem()->registerOnDropFunc(std::bind(&Editor::onDrop, this, 
            std::placeholders::_1, std::placeholders::_2));

        // create editor ui
        std::shared_ptr<EditorUI> world_ui = std::make_shared<WorldUI>();
        std::shared_ptr<EditorUI> property_ui = std::make_shared<PropertyUI>();
        std::shared_ptr<EditorUI> game_ui = std::make_shared<GameUI>();
        std::shared_ptr<EditorUI> asset_ui = std::make_shared<AssetUI>();
        std::shared_ptr<EditorUI> log_ui = std::make_shared<LogUI>();
        m_editor_uis = { world_ui, property_ui, game_ui, asset_ui, log_ui };

        // set construct ui function to UIPass through RenderSystem
        g_runtime_context.renderSystem()->setConstructUIFunc([this]() {
            for (auto& editor_ui : m_editor_uis)
            {
                editor_ui->construct();
            }
            });
    }

    void Editor::destroy()
    {
    }

    void Editor::run()
    {
        while (true)
        {
            float delta_time = m_engine->calcDeltaTime();
            if (!m_engine->tick(delta_time))
            {
                return;
            }
        }
    }

	void Editor::onDrop(int n, const char** filenames)
	{
        for (int i = 0; i < n; ++i)
        {
            g_runtime_context.assetManager()->importAsset(filenames[i], "asset/temp");
        }
	}

}