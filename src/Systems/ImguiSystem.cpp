
#include "ECS/Systems/ImguiSystem.hpp"

void ImguiSystem::update(EntityManager& em, ComponentManager& cm) {
    std::string mouse_coordinates = "(monitor)  " + std::to_string((int)ImGui::GetIO().MousePos.x) + ", " + std::to_string((int)ImGui::GetIO().MousePos.y);
    // std::string mouse_coordinates2= "(world)    " + std::to_string((int)this->cursorSystem.world_pos.x) + ", " + std::to_string((int)this->cursorSystem.world_pos.y);
    std::string entities_amount   = "(entities) " + std::to_string(em.getAllEntities().size());

    ImGui::Begin("Debug");
    if (ImGui::Button("Spawn ninja")) {
        buildNinja(cm, em);
        //buildNinja(cm, em, this->player->playerView.getCenter().x, this->player->playerView.getCenter().y);
    }

    ImGui::TextColored({0.0, 0.5, 1.0, 1.0}, "Information:");
    ImGui::Text("Focus: %d, Hovered: %d", ImGui::IsWindowFocused(), ImGui::IsWindowHovered());
    ImGui::BeginChild("Scrolling");
    ImGui::Text("            mouse x,y");
    ImGui::Text(mouse_coordinates.c_str());
    // ImGui::Text(mouse_coordinates2.c_str());
    ImGui::Text(entities_amount.c_str());
    ImGui::EndChild();
    ImGui::End();
}