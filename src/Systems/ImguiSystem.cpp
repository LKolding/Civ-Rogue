#include "ECS/Systems/ImguiSystem.hpp"


void ImguiSystem::update(const sf::RenderWindow& ren, EntityManager& em, ComponentManager& cm) {
    const auto mouse_pos = ImGui::GetIO().MousePos;
    const auto wmouse_pos = ren.mapPixelToCoords({(int)mouse_pos.x, (int)mouse_pos.y}); //world coordinates

    std::string tmouse_coordinates = "(monitor)  " + std::to_string(mouse_pos.x) + ", " + std::to_string(mouse_pos.y);
    std::string mouse_coordinates2 = "(world)    " + std::to_string(wmouse_pos.x) + ", " + std::to_string(wmouse_pos.y);
    std::string tentities_amount   = "(entities) " + std::to_string(em.getAllEntities().size());

    ImGui::Begin("Debug");
    if (ImGui::Button("Spawn ninja"))
        buildNinja(cm, em, ren.getView().getCenter().x, ren.getView().getCenter().y);

    if (ImGui::Button("Spawn well"))
        buildWell(cm, em, ren.getView().getCenter().x, ren.getView().getCenter().y);

    if (ImGui::Button("Spawn tree"))
        buildTree(cm, em, ren.getView().getCenter().x, ren.getView().getCenter().y);
    

    ImGui::TextColored({0.0, 0.5, 1.0, 1.0}, "Information:");
    ImGui::Text("Focus: %d, Hovered: %d", ImGui::IsWindowFocused(), ImGui::IsWindowHovered());
    ImGui::BeginChild("Scrolling");
    ImGui::Text(tmouse_coordinates.c_str());
    ImGui::Text(mouse_coordinates2.c_str());
    ImGui::Text(tentities_amount.c_str());
    ImGui::EndChild();
    ImGui::End();
}